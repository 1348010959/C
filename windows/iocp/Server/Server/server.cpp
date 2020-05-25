#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "server.h"

IocpModel::IocpModel() :
	m_threads(0),
	m_ShutdownEvent(nullptr),
	m_IOCompletionPort(nullptr),
	m_pWorkerThreads(nullptr),
	m_local_ip(DEFAULT_IP),
	m_local_port(DEFAULT_PORT),
	m_lpFunAcceptEx(nullptr),
	m_lpFunGetAcceptExSockAddrs(nullptr),
	m_p_listen_context(nullptr),
	m_accept_post_count(0),
	m_connect_count(0),
	m_error_count(0)
{
	m_Log_Func = nullptr;
	this->LoadSocketLib();
}

IocpModel::~IocpModel()
{
	UnloadSocketLib();
}

DWORD WINAPI IocpModel::_WorkerThread(LPVOID lpParam)
{
	WorkerThreadParam* pParam = (WorkerThreadParam*)lpParam;
	IocpModel* p_IocpModel = (IocpModel*)pParam->m_pIocpModel;

	const int32_t threadNo = pParam->m_threadNo;
	const int32_t threadId = pParam->m_threadId;

	p_IocpModel->_ShowMessage("worker thread, No: %d, ID: %d", threadNo, threadId);

	while (WAIT_OBJECT_0 != WaitForSingleObject(p_IocpModel->m_ShutdownEvent, 0))
	{
		DWORD dwBytesTransfered = 0;
		OVERLAPPED* pOverlapped = nullptr;
		SocketContext* p_SoContext = nullptr;
		const bool bRet = GetQueuedCompletionStatus(p_IocpModel->m_IOCompletionPort,
			&dwBytesTransfered, (PULONG_PTR)&p_SoContext, &pOverlapped, INFINITE);
		IoContext* p_IoContext = CONTAINING_RECORD(pOverlapped,
			IoContext, m_Overlapped);  //我感觉这宏就是硬凑上来的

		if (EXIT_CODE == (DWORD)p_SoContext)
		{
			break;
		}
		if (!bRet)
		{
			const DWORD dwErr = GetLastError();
			//TODO
			continue;
		}
		else
		{
			if ((0 == dwBytesTransfered) &&
				(PostType::RECV == p_IoContext->m_post_type
					|| PostType::SEND == p_IoContext->m_post_type))
			{
				p_IocpModel->OnConnectionClosed(p_SoContext);
				p_IocpModel->_DoClose(p_SoContext);
				continue;
			}
			else
			{
				switch (p_IoContext->m_post_type)
				{
				case PostType::ACCEPT:
				{
					p_IoContext->m_total_bytes = dwBytesTransfered;
					p_IocpModel->_DoAccept(p_SoContext, p_IoContext);
				}
				break;
				case PostType::RECV:
				{
					p_IoContext->m_total_bytes = dwBytesTransfered;
					p_IocpModel->_DoRecv(p_SoContext, p_IoContext);
				}
				break;
				case PostType::SEND:
				{
					p_IoContext->m_sent_bytes += dwBytesTransfered;
					p_IocpModel->_DoSend(p_SoContext, p_IoContext);
				}
				break;
				default:
					break;
				}
			}
		}
	}

	return 0;
}

bool IocpModel::LoadSocketLib()
{
	WSADATA wsaData{ 0 };
	const int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (NO_ERROR != nRet)
	{
		cout << "initiailize winsock 2.2 failed" << endl;
		return false;
	}

	return true;
}

bool IocpModel::Start(int32_t port /* = DEFAULT_PORT*/)
{
	m_local_port = port;

	//初始化临界区
	InitializeCriticalSection(&m_csContextList);
	//建立系统退出的事件通知
	m_ShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (!_InitializeIOCP())
	{
		cout << "Initialize IOCP failed " << endl;
		return false;
	}
	else
	{
		this->_ShowMessage("Initialize IOCP successfully");
	}

	if (!_InitializeListenSocket())
	{
		this->_ShowMessage("Initialize Listen socket failed");
		return false;
	}
	else
	{
		this->_ShowMessage("Initialize Listen socket successfully");
	}

	return true;
}

void IocpModel::Stop()
{
	if (m_p_listen_context != nullptr && m_p_listen_context->m_socket != INVALID_SOCKET)
	{
		SetEvent(m_ShutdownEvent);

		for (int i = 0; i < m_threads; ++i)
		{
			PostQueuedCompletionStatus(m_IOCompletionPort, 0, (DWORD)EXIT_CODE, NULL);
		}

		WaitForMultipleObjects(m_threads, m_pWorkerThreads, TRUE, INFINITE);

		this->_DeInitialize();
	}
	else
	{
		m_p_listen_context = nullptr;
	}
}

bool IocpModel::SendData(SocketContext*sp_SoContext, char* data, int32_t size)
{
	this->_ShowMessage("SendData : s = %p, d = %p", sp_SoContext, data);
	if (!sp_SoContext || data || size <= 0 || size > MAX_BUFFER_LEN)
	{
		this->_ShowMessage("Send data Param error");
		return false;
	}

	IoContext* sp_IoContext = sp_SoContext->GetNewIoContext();
	sp_IoContext->m_accept_socket = sp_SoContext->m_socket;
	sp_IoContext->m_post_type = PostType::SEND;
	sp_IoContext->m_total_bytes = size;
	sp_IoContext->m_WSABuf.len = size;
	memcpy(sp_IoContext->m_WSABuf.buf, data, size);
	if (!this->SendData(sp_SoContext, sp_IoContext))
	{
		return false;
	}

	return true;
}

bool IocpModel::SendData(SocketContext*sp_SoContext, IoContext* sp_IoContext)
{
	return _PostSend(sp_SoContext, sp_IoContext);
}

bool IocpModel::RecvData(SocketContext* p_SoContext, IoContext* p_IoContext)
{
	return this->_PostRecv(p_SoContext, p_IoContext);
}

bool IocpModel::_InitializeIOCP()
{
	this->_ShowMessage("Initialize IOCP");

	m_IOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL,
		0, 0);

	if (m_IOCompletionPort == nullptr)
	{
		this->_ShowMessage("Build IOCP failed");
		return false;
	}

	//根据CPU数量，初始化对应数量的线程
	//m_threads = 2 * _GetNumberOfProcessors();
	m_threads = 2;

	m_pWorkerThreads = new HANDLE[m_threads];

	DWORD thread_id = 0;
	for (int i = 0; i < m_threads; ++i)
	{
		WorkerThreadParam* p_thread_params = new WorkerThreadParam;

		p_thread_params->m_pIocpModel = this;
		p_thread_params->m_threadNo = i + 1;
		m_pWorkerThreads[i] = CreateThread(0, 0, _WorkerThread, (void*)p_thread_params, 0, &thread_id);
		p_thread_params->m_threadId = thread_id;
	}

	this->_ShowMessage("Build worker thread %d", m_threads);

	return true;
}

bool IocpModel::_InitializeListenSocket()
{
	this->_ShowMessage("Initialize Socket");

	m_p_listen_context = new SocketContext;

	//重叠IO必须使用WSA建立的socket
	m_p_listen_context->m_socket = WSASocket(AF_INET, SOCK_STREAM,
		IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == m_p_listen_context->m_socket)
	{
		this->_ShowMessage("WSASocket error, error = %d", WSAGetLastError());
		this->_DeInitialize();
		return false;
	}
	else
	{
		this->_ShowMessage("WSASocket build listen socket successfully");
	}

	if (NULL == CreateIoCompletionPort((HANDLE)m_p_listen_context->m_socket,
		m_IOCompletionPort, (ULONG_PTR)m_p_listen_context, 0))
	{
		this->_ShowMessage("bind completion port failed, err = %d", WSAGetLastError());
		this->_DeInitialize();
		return false;
	}
	else
	{
		this->_ShowMessage("bind completion port successfully! socket : %d", m_p_listen_context->m_socket);
	}

	//填充地址信息
	SOCKADDR_IN serverAddress;
	ZeroMemory((char*)&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(m_local_port);

	if (bind(m_p_listen_context->m_socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress))== SOCKET_ERROR)
	{
		this->_ShowMessage("bind error");
		this->_DeInitialize();
		return false;
	}
	else
	{
		this->_ShowMessage("bind successfully");
	}

	if (SOCKET_ERROR == listen(m_p_listen_context->m_socket, SOMAXCONN))
	{
		this->_ShowMessage("listen error");
		this->_DeInitialize();
		return false;
	}
	else
	{
		this->_ShowMessage("listen successfully");
	}

	//使用完成端口玩骚的花活之一，写好AcceptEx调用
	//这是属于微软WinSock2规范之外的微软另外提供的扩展函数
	//需要额外的获取函数指针，九个参数，真的花，就是AcceptEx函数指针
	DWORD dwBytes = 0;
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	if(SOCKET_ERROR == WSAIoctl(m_p_listen_context->m_socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptEx,
		sizeof(GuidAcceptEx), &m_lpFunAcceptEx,
		sizeof(m_lpFunAcceptEx), &dwBytes, NULL, NULL))
	{
		this->_ShowMessage("WSAIoctl Get AcceptEx function pointer failed, err = %d",
			WSAGetLastError());
		this->_DeInitialize();
		return false;
	}

	GUID GuidAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
	//需要以同样的手法获取GetAcceptExSockAddr函数指针
	if (SOCKET_ERROR == WSAIoctl(m_p_listen_context->m_socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptExSockAddrs,
		sizeof(GuidAcceptExSockAddrs), &m_lpFunGetAcceptExSockAddrs,
		sizeof(m_lpFunGetAcceptExSockAddrs), &dwBytes, NULL, NULL))
	{
		this->_ShowMessage("WSAIoctl Get AcceptExSockaddrs function pointer failed, err = %d",
			WSAGetLastError());
		this->_DeInitialize();
		return false;
	}

	//拿到了AcceptEx的函数指针，为它准备参数
	//创建10个套接字，投递AcceptEx请求，即有10个套接字进行accept
	for (size_t i = 0; i < 10; ++i)
	{
		IoContext* sp_IoContext = m_p_listen_context->GetNewIoContext();
		if (sp_IoContext && !this->_PostAccept(sp_IoContext))
		{
			m_p_listen_context->RemoveContext(sp_IoContext);
			return false;
		}
		//this->_ShowMessage("size: %d", m_p_listen_context->m_arrayIoContext.size());
	}


	this->_ShowMessage("Build 10 AcceptEx Post, size %d  socket: %d", m_p_listen_context->m_arrayIoContext.size(), m_p_listen_context->m_socket);

	return true;
}

void IocpModel::_DeInitialize()
{
	DeleteCriticalSection(&m_csContextList);

	RELEASE_HANDLE(m_ShutdownEvent);

	for (int i = 0; i < m_threads; ++i)
	{
		RELEASE_HANDLE(m_pWorkerThreads[i]);
	}

	RELEASE_ARRAY(m_pWorkerThreads);

	RELEASE_HANDLE(m_IOCompletionPort);

	RELEASE_POINTER(m_p_listen_context);
	this->_ShowMessage("Release resource");
}

bool IocpModel::_PostAccept(IoContext* sp_IoContext)
{
	if (m_p_listen_context == NULL || m_p_listen_context->m_socket == INVALID_SOCKET)
	{
		throw "_PostAccept, Error!";
	}

	sp_IoContext->ResetBuffer();
	sp_IoContext->m_post_type = PostType::ACCEPT;

	//精粹：提前准备好客户端的socket
	sp_IoContext->m_accept_socket = WSASocket(AF_INET, SOCK_STREAM,
		IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == sp_IoContext->m_accept_socket)
	{
		this->_ShowMessage("Build accept socket failed err = %d", WSAGetLastError());
		return false;
	}

	DWORD dwBytes = 0, dwAddrLen = (sizeof(SOCKADDR_IN) + 16);
	WSABUF* pWSABuf = &sp_IoContext->m_WSABuf;
	if (!m_lpFunAcceptEx(m_p_listen_context->m_socket,
		sp_IoContext->m_accept_socket, pWSABuf->buf,
		0,
		dwAddrLen, dwAddrLen, &dwBytes,
		&sp_IoContext->m_Overlapped))
	{
		int nErr = WSAGetLastError();
		if (WSA_IO_PENDING != nErr)
		{
			this->_ShowMessage("Accept error, err = %d", nErr);
			return false;
		}
	}

	++m_accept_post_count;
	return true;
}

#include <mstcpip.h>
bool IocpModel::_DoAccept(SocketContext*sp_SoContext, IoContext* sp_IoContext)
{
	++m_connect_count;
	--m_accept_post_count;
	//1.获取客户端地址
	SOCKADDR_IN* clientAddr = NULL, *localAddr = NULL;
	DWORD dwAddrLen = (sizeof(SOCKADDR_IN) + 16);
	int remoteLen = 0, localLen = 0;
	this->m_lpFunGetAcceptExSockAddrs(sp_IoContext->m_WSABuf.buf, 0,
		dwAddrLen, dwAddrLen, (LPSOCKADDR*)&localAddr, &localLen,
		(LPSOCKADDR*)&clientAddr, &remoteLen);
	/*this->m_LPFunGetAcceptExSockAddrs(sp_IoContext->m_WSABuf.buf, 
		sp_IoContext->m_WSABuf.len - (dwAddrLen)*2,
		dwAddrLen, dwAddrLen, (LPSOCKADDR*)&localAddr, &localLen,
		(LPSOCKADDR*)&clientAddr, &remoteLen);*/

	//2.新建socket context，统一管理
	SocketContext*sp_NewSoContext = new SocketContext();
	this->_AddToContextList(sp_NewSoContext);
	sp_NewSoContext->m_socket = sp_IoContext->m_accept_socket;

	memcpy(&(sp_NewSoContext->m_client_addr), clientAddr, sizeof(SOCKADDR_IN));
	this->_ShowMessage("客户端 %s:%d 连入了!", inet_ntoa(clientAddr->sin_addr),
		ntohs(clientAddr->sin_port));


	//3.将listenSocketContext重置后   投递新的AcceptEx
	if (!_PostAccept(sp_IoContext))
	{
		sp_SoContext->RemoveContext(sp_IoContext);
	}

	//4.将新的socket和完成端口绑定
	if (!this->_AssociateWithIOCP(sp_NewSoContext))
	{
		return false;
	}

	//TODO
	//tcp_keepalive alive_in{ 0 }, alive_out{ 0 };
	//alive_in.keepalivetime = 1000 * 60;
	//alive_in.keepaliveinterval = 1000 * 10;
	//alive_in.onoff = TRUE;
	OnConnectionAccept(sp_NewSoContext);

	//5.建立recv操作所需的ioContext，在新连接的socket投递recv请求
	IoContext* sp_NewIoContext = sp_SoContext->GetNewIoContext();
	if (sp_NewIoContext != nullptr)
	{
		sp_NewIoContext->m_post_type = PostType::RECV;
		sp_NewIoContext->m_accept_socket = sp_NewSoContext->m_socket;

		return _PostRecv(sp_NewSoContext, sp_NewIoContext);
	}
	else
	{
		_DoClose(sp_NewSoContext);
		return false;
	}
}

bool IocpModel::_PostRecv(SocketContext*sp_SoContext, IoContext* sp_IoContext)
{
	sp_IoContext->ResetBuffer();
	sp_IoContext->m_post_type = PostType::RECV;
	sp_IoContext->m_total_bytes = 0;
	sp_IoContext->m_sent_bytes = 0;

	//初始化变量
	DWORD dwFlags = 0, dwBytes = 0;
	//投递recv
	const int nBytesRecv = WSARecv(sp_IoContext->m_accept_socket,
		&sp_IoContext->m_WSABuf, 1, &dwBytes, &dwFlags,
		&sp_IoContext->m_Overlapped, NULL);

	if (SOCKET_ERROR == nBytesRecv)
	{
		int nErr = WSAGetLastError();
		if (WSA_IO_PENDING != nErr)
		{
			this->_ShowMessage("Post recv failed, err = %d", nErr);
			this->_DoClose(sp_SoContext);
			return false;
		}
	}
	return true;
}

bool IocpModel::_DoRecv(SocketContext*sp_SoContext, IoContext* sp_IoContext)
{
	//SOCKADDR_IN* clientAddr = &sp_SoContext->m_client_addr;

	this->OnRecvCompleted(sp_SoContext, sp_IoContext);

	return true;
}

bool IocpModel::_PostSend(SocketContext*sp_SoContext, IoContext* sp_IoContext)
{
	sp_IoContext->m_post_type = PostType::SEND;
	sp_IoContext->m_total_bytes = 0;
	sp_IoContext->m_sent_bytes = 0;

	//投递send请求
	const DWORD dwFlags = 0;
	DWORD dwSendNumBytes = 0;
	const int nRet = WSASend(sp_IoContext->m_accept_socket, &sp_IoContext->m_WSABuf,
		1, &dwSendNumBytes, dwFlags, &sp_IoContext->m_Overlapped, NULL);

	if (SOCKET_ERROR == nRet)
	{
		int nErr = WSAGetLastError();
		if (nErr != WSA_IO_PENDING)
		{
			this->_ShowMessage("Post WSASend failed, err=%d", nErr);
			this->_DoClose(sp_SoContext);
			return false;
		}
	}

	return true;
}

bool IocpModel::_DoSend(SocketContext* p_SoContext, IoContext* p_IoContext)
{
	if (p_IoContext->m_sent_bytes < p_IoContext->m_total_bytes)
	{
		p_IoContext->m_WSABuf.buf = p_IoContext->m_buff + p_IoContext->m_sent_bytes;
		p_IoContext->m_WSABuf.len = p_IoContext->m_total_bytes - p_IoContext->m_sent_bytes;

		return this->_PostSend(p_SoContext, p_IoContext);
	}
	else
	{
		this->OnSendCompleted(p_SoContext, p_IoContext);
		return true;
	}
}

bool IocpModel::_DoClose(SocketContext* sp_SoContext)
{
	if (sp_SoContext != m_p_listen_context)
	{
		--m_connect_count;
		this->_RemoveContext(sp_SoContext);
		return true;
	}
	++m_error_count;
	return false;

}

void IocpModel::_AddToContextList(SocketContext* sp_SoContext)
{
	EnterCriticalSection(&m_csContextList);
	m_array_client_context.insert(sp_SoContext);
	LeaveCriticalSection(&m_csContextList);
}

void IocpModel::_RemoveContext(SocketContext*sp_SoContext)
{
	EnterCriticalSection(&m_csContextList);
	auto it = m_array_client_context.find(sp_SoContext);
	if (it != m_array_client_context.end())
	{
		m_array_client_context.erase(it);
	}
	LeaveCriticalSection(&m_csContextList);
}

bool IocpModel::_AssociateWithIOCP(SocketContext* sp_SoContext)
{
	HANDLE hTemp = CreateIoCompletionPort((HANDLE)sp_SoContext->m_socket,
		m_IOCompletionPort, (ULONG_PTR)sp_SoContext, 0);

	if (nullptr == hTemp)
	{
		this->_ShowMessage("Associate IOCP failed, err = %d", GetLastError());
		this->_DoClose(sp_SoContext);
		return false;
	}
	return true;
}

//获取本机中CPU的数量
int32_t IocpModel::_GetNumberOfProcessors() noexcept
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	return si.dwNumberOfProcessors;
}

void IocpModel::_ShowMessage(const char* format, ...)
{
	if (m_Log_Func)
	{
		char buff[256] = { 0 };
		va_list arglist;

		va_start(arglist, format);
		vsnprintf(buff, sizeof(buff), format, arglist);
		va_end(arglist);

		m_Log_Func(string(buff));
	}
}
