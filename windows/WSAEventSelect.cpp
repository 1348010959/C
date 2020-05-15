#include <WinSock2.h>
#include <cstdio>
#include <iostream>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

int main(INT argc, CHAR* argv[])
{
	using namespace std;
	//初始化套接字库
	WORD vVersionRequested;
	WSADATA wsaData;
	vVersionRequested = MAKEWORD(1, 1);
	int nError = WSAStartup(vVersionRequested, &wsaData);
	if (nError != 0)
		return -1;

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}

	//创建套接字
	SOCKET sockSer = socket(AF_INET, SOCK_STREAM, 0);
	if (sockSer == SOCKET_ERROR)
	{
		WSACleanup();
		return -1;
	}

	SOCKADDR_IN addrSer;
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(8888);
	addrSer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (bind(sockSer, (SOCKADDR*)&addrSer, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		closesocket(sockSer);
		WSACleanup();
		return -1;
	}
	
	if (listen(sockSer, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(sockSer);
		WSACleanup();
		return -1;
	}

	WSAEVENT hListenEvent = WSACreateEvent();
	if (WSAEventSelect(sockSer, hListenEvent, FD_ACCEPT) == SOCKET_ERROR)
	{
		WSACloseEvent(hListenEvent);
		closesocket(sockSer);
		WSACleanup();
		return -1;
	}

	WSAEVENT* pEvents = new WSAEVENT[1];
	pEvents[0] = hListenEvent;
	SOCKET* pSockets = new SOCKET[1];
	pSockets[0] = sockSer;
	DWORD dwCount = 1;
	bool bNeedToMove;

	while (true)
	{
		bNeedToMove = false;
		DWORD dwResult = WSAWaitForMultipleEvents(dwCount, pEvents, FALSE, WSA_INFINITE, FALSE);
		if (dwResult == WSA_WAIT_FAILED)
			continue;

		DWORD dwIndex = dwResult - WSA_WAIT_EVENT_0;
		for (DWORD i = 0; i <= dwIndex; ++i)
		{
			WSANETWORKEVENTS triggeredEvents;
			if (WSAEnumNetworkEvents(pSockets[i], pEvents[i], &triggeredEvents) == SOCKET_ERROR)
				continue;

			if (triggeredEvents.lNetworkEvents & FD_ACCEPT)
			{
				if (triggeredEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
					continue;

				//accept
				SOCKADDR_IN addClient;
				int len = sizeof(SOCKADDR);

				SOCKET hSocketClient = accept(sockSer, (SOCKADDR*)&addClient, &len);
				if (hSocketClient != SOCKET_ERROR)
				{
					//监听客户端socket的可读和关闭事件
					WSAEVENT hClientEvent = WSACreateEvent();
					if (WSAEventSelect(hSocketClient, hClientEvent, FD_READ | FD_CLOSE) == SOCKET_ERROR)
					{
						WSACloseEvent(hClientEvent);
						closesocket(hSocketClient);
						continue;
					}

					WSAEVENT* pEvents2 = new WSAEVENT[dwCount + 1];
					SOCKET* pSockets2 = new SOCKET[dwCount + 1];
					memcpy(pEvents2, pEvents, dwCount * sizeof(WSAEVENT));
					pEvents2[dwCount] = hClientEvent;
					memcpy(pSockets2, pSockets, dwCount * sizeof(SOCKET));
					pSockets2[dwCount] = hSocketClient;
					delete[] pEvents;
					delete[] pSockets;
					pEvents = pEvents2;
					pSockets = pSockets2;

					dwCount++;

					cout << "New Client Connected , socket: " << hSocketClient << " current: " << dwCount - 1 << endl;
				}
			}
			else if (triggeredEvents.lNetworkEvents & FD_READ)
			{
				if (triggeredEvents.iErrorCode[FD_READ_BIT] != 0)
					continue;

				char szBuff[64] = { 0 };
				int nRet = recv(pSockets[i], szBuff, 64, 0);
				if (nRet > 0)
				{
					cout << "recv data : " << szBuff << "client: " << pSockets[i] << endl;
				}
			}
			else if (triggeredEvents.lNetworkEvents & FD_CLOSE)
			{
				cout << "One client disconnected, socket :" << pSockets[i] << ", current " << (int)pSockets[i] << endl;

				WSACloseEvent(pEvents[i]);
				closesocket(pSockets[i]);

				pSockets[i] = INVALID_SOCKET;

				bNeedToMove = true;
			}
		}

		if (bNeedToMove)
		{
			vector<SOCKET> vValidSockets;
			vector<HANDLE> vValidEvents;
			for (size_t i = 0; i < dwCount; ++i)
			{
				if (pSockets[i] != INVALID_SOCKET)
				{
					vValidSockets.push_back(pSockets[i]);
					vValidEvents.push_back(pEvents[i]);
				}
			}

			size_t validSize = vValidSockets.size();
			if (validSize > 0)
			{
				WSAEVENT* pEvents2 = new WSAEVENT[validSize];
				SOCKET* pSockets2 = new SOCKET[validSize];
				memcpy(pEvents2, &vValidEvents[0], validSize * sizeof(WSAEVENT));
				memcpy(pSockets2, &vValidSockets[0], validSize * sizeof(SOCKET));
				delete[] pEvents;
				delete[] pSockets;
				pEvents = pEvents2;
				pSockets = pSockets2;

				dwCount = validSize;
			}
		}
	}

	closesocket(sockSer);

	WSACleanup();

	return 0;
}
