#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

const size_t BUFFER_SIZE = 1024;

int main() {
	WSADATA WinSockData;
	if (WSAStartup(WINSOCK_VERSION, &WinSockData) != 0) {
		std::cout << "Failed To Start WSA!\n";
		return -1;
	}

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET) {
		std::cout << "Failed To Create Socket!\n";
		return -1;
	}

	sockaddr_in BindAdderss;
	BindAdderss.sin_family = AF_INET;
	BindAdderss.sin_port = htons(3550);
	BindAdderss.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (bind(ListenSocket, reinterpret_cast<sockaddr*>(&BindAdderss), sizeof(sockaddr)) == SOCKET_ERROR) {
		std::cout << "Failed To Bind Socket!\n";
		return -1;
	}
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Failed To Listen Socket!\n";
		return -1;
	}

	fd_set ReadFDS;
	FD_ZERO(&ReadFDS);
	FD_SET(ListenSocket, &ReadFDS);

	while (true) {
		fd_set TempFDS = ReadFDS;
		int Result = select(0, &TempFDS, NULL, NULL, NULL);
		if (FD_ISSET(ListenSocket, &TempFDS)) {
			int AddrLength = sizeof(sockaddr);
			sockaddr ClientAddress;
			SOCKET ClientSocket = accept(ListenSocket, &ClientAddress, &AddrLength);
			if (ClientSocket == INVALID_SOCKET) {
				std::cout << "Accept Failure!\n";
				continue;
			}
			std::cout << "New Client!\n";
			FD_SET(ClientSocket, &ReadFDS);
			continue;
		}

		for (int i = 0; i < TempFDS.fd_count; i++) {
			SOCKET TempSock = TempFDS.fd_array[i];
			if (TempSock == ListenSocket) {
				continue;
			}
			char MessageBuffer[BUFFER_SIZE] = { "\0" };
			if (recv(TempSock, MessageBuffer, BUFFER_SIZE, 0) > 0) {
				std::cout << "Recv Data : " << MessageBuffer << std::endl;
				send(TempSock, MessageBuffer, strlen(MessageBuffer), 0);
				continue;
			}
			std::cout << "Disconnect Client!\n";
			closesocket(TempSock);
			FD_CLR(TempSock, &ReadFDS);
		}
	}
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}