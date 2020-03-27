#include "1.h"
#include <string.h>

int main() {
#ifdef _WIN32
	WSADATA WinSockData;
	if (WSAStartup(WINSOCK_VERSION, &WinSockData) != 0) {
		std::cout << "Failed To Initialize WinSock Application! : " << WSAGetLastError() << std::endl;
		return -1;
	}

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else if
	int ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
	if (ListenSocket == -1) {
		std::cout << "failed to create socket\n";
		return -1;
	}

	sockaddr_in BindAddress;
	BindAddress.sin_family = AF_INET;
	BindAddress.sin_addr.s_addr = INADDR_ANY;
	BindAddress.sin_port = 3550;
	if (bind(ListenSocket, reinterpret_cast<sockaddr*>(&BindAddress), sizeof(sockaddr_in)) == -1) {
		std::cout << "failed to bind socket\n";
		return -1;
	}

	if (listen(ListenSocket, SOMAXCONN) == -1) {
		std::cout << "failed to listen socket\n";
		return -1;
	}

	sockaddr_in ClientAddr;
	memset(&ClientAddr, 0x00, sizeof(ClientAddr));

#ifdef _WIN32
	int AddrLen = sizeof(sockaddr_in);
	SOCKET ClientSock = accept(ListenSocket, reinterpret_cast<sockaddr*>(&ClientAddr), &AddrLen);
#else if
	uint32_t AddrLen = sizeof(sockaddr_in);
	int ClientSock = accept(ListenSocket, reinterpret_cast<sockaddr*>(&ClientAddr), &AddrLen);
#endif
	if (ClientSock == -1) {
		std::cout << "failed to accept client\n";
		return -1;
	}

	char MessageBuffer[1024] = { "\0" };
	if (recv(ClientSock, MessageBuffer, 1024, 0) == -1) {
		std::cout << "failed to recv\n";
		return -1;
	}

	if (send(ClientSock, MessageBuffer, 1024, 0) == -1) {
		std::cout << "failed to send\n";
		return -1;
	}

#ifdef _WIN32
	closesocket(ListenSocket);
	closesocket(ClientSock);

	if (WSACleanup() != 0) {
		std::cout << "Failed To Cleanup WinSock Application! : " << WSAGetLastError() << std::endl;
		return -1;
	}
#else if
	close(ListenSocket);
	close(ClientSock);
#endif
	return 0;
}