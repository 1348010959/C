#include <Windows.h>
#include <iostream>

using namespace std;

HANDLE g_Mutex = nullptr;
int    g_Count = 0;

DWORD __stdcall workThreadProc(LPVOID lpPramater)
{
	DWORD threadId = GetCurrentThreadId();
	while(true)
	{
		if (WaitForSingleObject(g_Mutex, INFINITE) == WAIT_OBJECT_0)
		{
			cout << "Thread Id is : " << threadId << " .Sequence: " << ++g_Count << endl;
			ReleaseMutex(g_Mutex);
		}
		Sleep(1000);
	}

	return 0;
}

int main()
{
	g_Mutex = CreateMutex(nullptr, FALSE, nullptr);

	HANDLE hWorkThreads[5];
	for (int i = 0; i < 5; ++i)
	{
		hWorkThreads[i] = CreateThread(nullptr, 0, workThreadProc, nullptr, 0, nullptr);
	}

	for (int i = 0; i < 5; ++i)
	{
		WaitForSingleObject(hWorkThreads[i], INFINITE);
		CloseHandle(hWorkThreads[i]);
	}

	CloseHandle(g_Mutex);

	return 0;
}
