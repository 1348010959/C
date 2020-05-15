#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <list>
#include <string>
#include <iostream>

using namespace std;

CRITICAL_SECTION g_Critical;
HANDLE           g_Semaphore = nullptr;
list<string>     g_MsgList;

DWORD __stdcall ProdMsgThread(LPVOID args)
{
	DWORD threadId = GetCurrentThreadId();

	while (true)
	{
		int count = rand() % 4 + 1;

		EnterCriticalSection(&g_Critical);

		for (int i = 0; i < count; ++i)
		{
			SYSTEMTIME st;
			GetLocalTime(&st);

			char buf[64] = { 0 };
			sprintf(buf, "[%04d-%02d-%02d %02d:%02d:%02d %03d]", st.wYear, st.wMonth, st.wDay,
				st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
			string time(buf);
			g_MsgList.emplace_back(time);
		}

		ReleaseSemaphore(g_Semaphore, count, nullptr);

		LeaveCriticalSection(&g_Critical);
	}

	return 0;
}

DWORD __stdcall ParseMsgThread(LPVOID args)
{
	DWORD threadId = GetCurrentThreadId();

	while (true)
	{
		if (WaitForSingleObject(g_Semaphore, INFINITE) == WAIT_OBJECT_0)
		{
			EnterCriticalSection(&g_Critical);
			while (!g_MsgList.empty())
			{
				cout << "ThreadId is :" << threadId << " Msg is :" << g_MsgList.front() << endl;
				g_MsgList.pop_front();
			}

			LeaveCriticalSection(&g_Critical);
		}
	}

	return 0;
}

int main()
{
	srand(time(nullptr));
	InitializeCriticalSection(&g_Critical);

	g_Semaphore = CreateSemaphore(nullptr, 0, INT_MAX, nullptr);

	HANDLE hProdThread = CreateThread(nullptr, 0, ProdMsgThread, nullptr, 0, nullptr);

	HANDLE hWorkerThreads[4];

	for (int i = 0; i < 4; ++i)
	{
		hWorkerThreads[i] = CreateThread(nullptr, 0, ParseMsgThread, nullptr, 0, nullptr);
	}

	for (int i = 0; i < 4; ++i)
	{
		WaitForSingleObject(hWorkerThreads[i], INFINITE);
		CloseHandle(hWorkerThreads[i]);
	}

	WaitForSingleObject(hProdThread, INFINITE);
	CloseHandle(g_Semaphore);

	DeleteCriticalSection(&g_Critical);
	return 0;
}
