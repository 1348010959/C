#include "server.h"
#pragma comment(lib, "ws2_32.lib")

class ServerApp : public IocpModel
{
public:
	ServerApp()
	{
		InitializeCriticalSection(&m_csLog);
	}

	~ServerApp()
	{
		DeleteCriticalSection(&m_csLog);
	}

	void _ShowMessage(const char* format, ...)
	{
		EnterCriticalSection(&m_csLog);

		SYSTEMTIME sysTime{ 0 };
		GetLocalTime(&sysTime);

		printf("%4d-%02d-%02d %02d:%02d:%02d.%03d£º",
			sysTime.wYear, sysTime.wMonth, sysTime.wDay,
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond,
			sysTime.wMilliseconds);

		va_list arglist;
		va_start(arglist, format);
		vprintf(format, arglist);
		va_end(arglist);

		printf("\n");

		LeaveCriticalSection(&m_csLog);
		return;
	}

	virtual void OnConnectionAccept(SocketContext* p_SoContext) 
	{
		_ShowMessage("New Connection is accept, Current connections: %d",
			GetConnectCount());
	}

	virtual void OnConnectionClosed(SocketContext* p_SoContext) 
	{
		_ShowMessage("A connect is closed, Current connections: %d", GetConnectCount());
	}
	virtual void OnConnectionError(SocketContext* p_SoContext, int error) {};
	virtual void OnRecvCompleted(SocketContext* p_SoContext, IoContext* p_IoContext)
	{
		SendData(p_SoContext, p_IoContext);
	}

private:
	CRITICAL_SECTION m_csLog;
};

int main()
{
	shared_ptr<ServerApp> sp_ServerApp = make_shared<ServerApp>();

	if (sp_ServerApp->Start())
	{
		sp_ServerApp->_ShowMessage("Server start successful on port: %d", sp_ServerApp->GetPort());
	}
	else
	{
		sp_ServerApp->_ShowMessage("Server start failed!");
		return 0;
	}

	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, L"ShutdownEvent");
	if (hEvent != NULL)
	{
		WaitForSingleObject(hEvent, INFINITE);
		CloseHandle(hEvent);
	}

	sp_ServerApp->Stop();
	sp_ServerApp->_ShowMessage("Server Closed\n");
	return 0;
}
