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
	int32_t        m_threadNo;   //�̱߳��
	int32_t        m_threadId;   //�߳�ID
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

	//��ʼ��IOCP
	bool _InitializeIOCP();
	//��ʼ��socket
	bool _InitializeListenSocket();
	//�ͷ���Դ
	void _DeInitialize();
	//Ͷ��AcceptEx����
	bool _PostAccept(IoContext* sp_IoContext);
	//���пͻ��������ʱ�򣬽��д���
	bool _DoAccept(SocketContext*sp_SoContext, IoContext* sp_IoContext);
	//bool _DoAccept(SocketContext* sp_SoContext, IoContext* sp_IoContext);
	
	//Ͷ��recv
	bool _PostRecv(SocketContext*sp_SoContext, IoContext* sp_IoContext);
	//���ݵ����������IoContext
	bool _DoRecv(SocketContext*sp_SoContext, IoContext* sp_IoContext);

	//Ͷ��send
	bool _PostSend(SocketContext*sp_SoContext, IoContext* sp_IoContext);
	bool _DoSend(SocketContext*sp_SoContext, IoContext* sp_IoContext);
	bool _DoClose(SocketContext*sp_SoContext);
	//���ͻ��˵����ݴ洢
	void _AddToContextList(SocketContext* sp_SoContext);
	//���ͻ��˵������Ƴ�
	void _RemoveContext(SocketContext*sp_SoContext);

	//�����������ɶ˿�
	bool _AssociateWithIOCP(SocketContext*sp_SoContext);

	int32_t _GetNumberOfProcessors() noexcept;
	//�̺߳���
	static DWORD WINAPI _WorkerThread(LPVOID lpParam);

	virtual void _ShowMessage(const char* format, ...);

protected:
	Log_Func                         m_Log_Func;

private:
	HANDLE                           m_ShutdownEvent;               //֪ͨ���̣߳�ͳһ�˳�
	HANDLE                           m_IOCompletionPort;            //��ɶ˿ھ��
	HANDLE*                          m_pWorkerThreads;              //�����̵߳ľ��ָ��
	int32_t                          m_threads;                     //���߳�����
	string                           m_local_ip;                    //������ip
	int32_t                          m_local_port;                  //�������˿ں�
	CRITICAL_SECTION                 m_csContextList;               //�ٽ���
	set<SocketContext*>              m_array_client_context;        //�ͻ���socket�����Ϣ
	SocketContext*                   m_p_listen_context;           //����socket
	atomic<int>                      m_accept_post_count;           //��ǰͶ�ݵ�accept����
	atomic<int>                      m_connect_count;               //��ǰ��������
	atomic<int>                      m_error_count;                 //��������?
																    
	LPFN_GETACCEPTEXSOCKADDRS        m_lpFunGetAcceptExSockAddrs;   //��չ�����ĺ���ָ��

	LPFN_ACCEPTEX                    m_lpFunAcceptEx;
};
