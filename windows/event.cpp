#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

bool		g_TaskComplated = false;
string		g_bTaskResult;
HANDLE		g_hTaskEvent = nullptr;

DWORD __stdcall workThreadProc(LPVOID lpThreadParameter)
{
	Sleep(3000);
	g_bTaskResult = "task completed";
	g_TaskComplated = true;

	SetEvent(g_hTaskEvent);

	return 0;
}

int main()
{
	g_hTaskEvent = CreateEvent(NULL, TRUE, FALSE, nullptr);
	HANDLE hworkerThread = CreateThread(NULL, 0, workThreadProc, NULL, 0, NULL);

	DWORD dwResult = WaitForSingleObject(g_hTaskEvent, INFINITE);

	if (dwResult == WAIT_OBJECT_0)
	{
		cout << g_bTaskResult << endl;
	}

	CloseHandle(hworkerThread);
	CloseHandle(g_hTaskEvent);
	return 0;
}
