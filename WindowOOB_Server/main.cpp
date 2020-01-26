#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

int main() {
	WSADATA WinSockData;
	if (WSAStartup(WINSOCK_VERSION, &WinSockData) != 0) {
		std::cout << "Failed To Start WSA!\n";
		return -1;
	}

	SOCKET ListenSocket;
	if ((ListenSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR) {
		std::cout << "Failed To Create Socket!\n";
		return -1;
	}

	sockaddr_in BindAddress;
	BindAddress.sin_family = AF_INET;
	BindAddress.sin_port = htons(3550);
	BindAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (bind(ListenSocket, reinterpret_cast<sockaddr*>(&BindAddress), sizeof(BindAddress)) == SOCKET_ERROR) {
		std::cout << "Failed To Bind Socket!\n";
		return -1;
	}
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Failed To Listen!\n";
		return -1;
	}
	
	sockaddr ClientAddr;
	int AddrLen = sizeof(sockaddr_in);
	while (true) {
		SOCKET ClientSocket = accept(ListenSocket, &ClientAddr, &AddrLen);
		int i = 1;
		while (true) {
			std::string InputBuffer;
			std::getline(std::cin, InputBuffer);
			if (send(ClientSocket, InputBuffer.c_str(), InputBuffer.length(), 0) > 0) {
				if ((i % 3) == 0) {
					send(ClientSocket, "9", sizeof(char), MSG_OOB);
				}
				i++;
			}
		}
	}

	WSACleanup();
	return 0;
}