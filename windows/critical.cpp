#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <list>
#include <iostream>
#include <string>

//定义一个临界区变量
CRITICAL_SECTION g_cs;
int              g_number = 0;

/*
 windows下的线程需要采用__stdcall方式调用
*/
DWORD __stdcall workerThreadProc(LPVOID lpThreadParameter)
{
	DWORD dwThreadId = GetCurrentThreadId();

	while (true)
	{
		EnterCriticalSection(&g_cs);

		std::cout << "EnterCriticalSection, ThreadId: " << dwThreadId << std::endl;
		g_number++;
		SYSTEMTIME st;
		GetLocalTime(&st);

		char szMsg[64] = { 0 };
		sprintf(szMsg, "[%04d-%02d-%02d %02d:%02d:%2d:%03d]NO.%d, ThreadID: %d",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, g_number, dwThreadId);

		std::cout << szMsg << std::endl;
		std::cout << "LeaveCriticalSection, ThreadId: " << dwThreadId << std::endl;
		LeaveCriticalSection(&g_cs);

		Sleep(1000);
	}

	return 0;
}

int main()
{
	//初始化临界区
	InitializeCriticalSection(&g_cs);

	//创建线程
	HANDLE hworkerThread1 = CreateThread(NULL, 0, workerThreadProc, NULL, 0, NULL);
	HANDLE hworkerThread2 = CreateThread(NULL, 0, workerThreadProc, NULL, 0, NULL);

	//等待一个对象，对象类型可为：线程，进程，事件，互斥体，信号量
	WaitForSingleObject(hworkerThread1, INFINITE);
	WaitForSingleObject(hworkerThread2, INFINITE);

	CloseHandle(hworkerThread1);
	CloseHandle(hworkerThread2);

	DeleteCriticalSection(&g_cs);

	return 0;
}