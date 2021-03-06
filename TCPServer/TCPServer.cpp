#include "pch.h"
#include <iostream>
#include "winsock2.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <WS2tcpip.h> //for inet_pton

#pragma comment (lib,"ws2_32.lib")

using namespace std;

int main()
{
	WSADATA wsaData;
	int wsaTest;
	SOCKET serverSocket;

	int port = 13000;
	char IP[10] = "127.0.0.1";
	SOCKADDR_IN serverAddr;
	int addrLen = sizeof(serverAddr);

	wsaTest = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (wsaTest != NO_ERROR)
	{
		cout << "WSAStartup error: " << WSAGetLastError() << endl;
		return 1;
	}

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	if (serverSocket == INVALID_SOCKET)
	{
		cout << "SOcket initializatin problem: " << WSAGetLastError() << endl;
		return 1;
	}

	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, IP, &serverAddr.sin_addr.s_addr);
	serverAddr.sin_port = htons(port);

	if (bind(serverSocket, (SOCKADDR*)&serverAddr, addrLen) == SOCKET_ERROR)
	{
		cout << "bind() failed" << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}

	if (listen(serverSocket, 1) == SOCKET_ERROR)
	{
		cout << "Listen error." << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}

	while (1)
	{
		cout << "Wait for clinet to connect..." << endl;
		SOCKET acceptSocket;
		acceptSocket = accept(serverSocket, NULL, NULL);
		if (acceptSocket == INVALID_SOCKET)
		{
			cout << "Accept failed: " << WSAGetLastError() << endl;
			closesocket(serverSocket);
			WSACleanup();
			return 1;
		}
		else
		{
			cout << "Client connected" << endl;
		}

		cout << "Receiving ...." << endl;
		const int recvBufLength = 1024;
		char recvBufData[recvBufLength];
		recvfrom(acceptSocket, recvBufData, recvBufLength, 0, 0, 0);

		cout << "The message is: " << recvBufData << endl;

		cout << "Sending..." << endl;
		const int sendBufLength = 1024;
		char sendBufData[sendBufLength];
		strcpy_s(sendBufData, "Valasz!");
		send(acceptSocket, sendBufData, sendBufLength, 0);

	}
		cout << "Exit" << endl;
		WSACleanup();
		return 0;
	
}
