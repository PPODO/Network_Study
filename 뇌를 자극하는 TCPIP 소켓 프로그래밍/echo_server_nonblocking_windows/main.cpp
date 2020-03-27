#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <vector>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

constexpr size_t BUFFER_SIZE = 1024;

int main() {
	WSADATA WinSockData;
	if (WSAStartup(WINSOCK_VERSION, &WinSockData) != 0) {
		std::cout << "Failed To Startup WSA!\n";
		return -1;
	}

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET) {
		std::cout << "Failed To Create Listen Socket!\n";
		return -1;
	}

	sockaddr_in BindAddress;
	BindAddress.sin_family = AF_INET;
	BindAddress.sin_port = htons(3550);
	BindAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (bind(ListenSocket, reinterpret_cast<sockaddr*>(&BindAddress), sizeof(BindAddress)) == SOCKET_ERROR || listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Failed To Bind or Listen!\n";
		return -1;
	}

	u_long iMode = 1;
	if (ioctlsocket(ListenSocket, FIONBIO, &iMode) != 0) {
		std::cout << "FIONBIO Setting Failure!\n";
		return -1;
	}

	std::vector<SOCKET> Clients;
	int AddrLength = sizeof(sockaddr);
	while (true) {
		sockaddr ClientAddress;
		SOCKET Client = accept(ListenSocket, &ClientAddress, &AddrLength);
		if (Client == INVALID_SOCKET) {
			if (WSAGetLastError() != WSAEWOULDBLOCK) {
				std::cout << "Accept Failure!\n";
			}
		}
		else {
			std::cout << "New Client!\n";
			ioctlsocket(Client, FIONBIO, &iMode);
			Clients.emplace_back(Client);
		}

		for (auto Client = Clients.begin(); Client != Clients.end(); Client++) {
			char MessageBuffer[BUFFER_SIZE] = { "\0" };
			int RecvBytes = recv(*Client, MessageBuffer, BUFFER_SIZE, 0);
			if (RecvBytes < 0) {
				if (WSAGetLastError() != WSAEWOULDBLOCK) {
					std::cout << "Disconnect Client!\n";
					closesocket(*Client);
					Client = Clients.erase(Client);
					if (Clients.size() == 0) { break; }
				}
			}
			else if (RecvBytes == 0) {
				std::cout << "Disconnect Client!\n";
				closesocket(*Client);
				Client = Clients.erase(Client);
				if (Clients.size() == 0) { break; }
			}
			else {
				std::cout << "Recv Bytes : " << MessageBuffer << std::endl;
				send(*Client, MessageBuffer, RecvBytes, 0);
			}
		}
	}
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}