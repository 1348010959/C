#pragma once
#include "PerSocketContext.h"
#include <atomic>

#define EXIT_CODE    NULL
#define DEFAULT_IP   "127.0.0.1"
#define DEFAULT_PORT 8888

#define RELEASE_ARRAY(x) {if(x != nullptr) {delete[] x; x=nullptr;}}
#define RELEASE_POINTER(x) {if(x != nullptr) {delete x; x=nullptr;}}
#define RELEASE_HANDLE(x) {if(x != nullptr && x!=INVALID_HANDLE_VALUE)\
	{CloseHandle(x);x=nullptr;}}

class IocpModel;

typedef void (*Log_Func)(const string& strInfo);

struct WorkerThreadParam
{
	IocpModel*     m_pIocpModel;
	int32_t        m_threadNo;   //线程编号
	int32_t        m_threadId;   //线程ID
};

class IocpModel
{
public:
	IocpModel();
	~IocpModel();

	bool LoadSocketLib();

	void UnloadSocketLib() noexcept
	{
		WSACleanup();
	}

	bool Start(int32_t port = DEFAULT_PORT);

	void Stop();

	bool SendData(SocketContext*sp_SoContext, char* data, int32_t size);
	bool SendData(SocketContext*sp_SoContext, IoContext* sp_IoContext);

	bool RecvData(SocketContext* p_SoContext, IoContext* p_IoContext);

	uint32_t GetConnectCount()
	{
		return m_connect_count;
	}
	int32_t GetPort() const
	{
		return m_local_port;
	}

	virtual void OnConnectionAccept(SocketContext* sp_SoContext) {};
	virtual void OnConnectionClosed(SocketContext* sp_SoContext) {};
	virtual void OnConnectionError(SocketContext* sp_SoContext, int error) {};
	virtual void OnRecvCompleted(SocketContext* sp_SoContext, IoContext* sp_IoContext)
	{
		SendData(sp_SoContext, sp_IoContext);
	}
	virtual void OnSendCompleted(SocketContext* p_SoContext, IoContext* p_IoContext)
	{
		RecvData(p_SoContext, p_IoContext);
	}

	void SetLogFunc(const Log_Func func)
	{
		m_Log_Func = func;
	}

protected:

	//初始化IOCP
	bool _InitializeIOCP();
	//初始化socket
	bool _InitializeListenSocket();
	//释放资源
	void _DeInitialize();
	//投递AcceptEx请求
	bool _PostAccept(IoContext* sp_IoContext);
	//在有客户端连入的时候，进行处理
	bool _DoAccept(SocketContext*sp_SoContext, IoContext* sp_IoContext);
	//bool _DoAccept(SocketContext* sp_SoContext, IoContext* sp_IoContext);
	
	//投递recv
	bool _PostRecv(SocketContext*sp_SoContext, IoContext* sp_IoContext);
	//数据到达，数组存放于IoContext
	bool _DoRecv(SocketContext*sp_SoContext, IoContext* sp_IoContext);

	//投递send
	bool _PostSend(SocketContext*sp_SoContext, IoContext* sp_IoContext);
	bool _DoSend(SocketContext*sp_SoContext, IoContext* sp_IoContext);
	bool _DoClose(SocketContext*sp_SoContext);
	//将客户端的数据存储
	void _AddToContextList(SocketContext* sp_SoContext);
	//将客户端的数据移出
	void _RemoveContext(SocketContext*sp_SoContext);

	//将句柄绑定至完成端口
	bool _AssociateWithIOCP(SocketContext*sp_SoContext);

	int32_t _GetNumberOfProcessors() noexcept;
	//线程函数
	static DWORD WINAPI _WorkerThread(LPVOID lpParam);

	virtual void _ShowMessage(const char* format, ...);

protected:
	Log_Func                         m_Log_Func;

private:
	HANDLE                           m_ShutdownEvent;               //通知子线程，统一退出
	HANDLE                           m_IOCompletionPort;            //完成端口句柄
	HANDLE*                          m_pWorkerThreads;              //工作线程的句柄指针
	int32_t                          m_threads;                     //子线程数量
	string                           m_local_ip;                    //服务器ip
	int32_t                          m_local_port;                  //服务器端口号
	CRITICAL_SECTION                 m_csContextList;               //临界区
	set<SocketContext*>              m_array_client_context;        //客户端socket相关信息
	SocketContext*                   m_p_listen_context;           //监听socket
	atomic<int>                      m_accept_post_count;           //当前投递的accept数量
	atomic<int>                      m_connect_count;               //当前连接数量
	atomic<int>                      m_error_count;                 //错误数量?
																    
	LPFN_GETACCEPTEXSOCKADDRS        m_lpFunGetAcceptExSockAddrs;   //扩展函数的函数指针

	LPFN_ACCEPTEX                    m_lpFunAcceptEx;
};
