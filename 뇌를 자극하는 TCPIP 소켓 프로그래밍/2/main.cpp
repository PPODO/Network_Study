#include <iostream>
#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#else if
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

int main() {
#ifdef _WIN32
	WSADATA WinSockData;
	if (WSAStartup(WINSOCK_VERSION, &WinSockData) != 0) {
		std::cout << "Failed To Initialize WinSock Application! : " << WSAGetLastError() << std::endl;
		return -1;
	}

	SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else if
	int ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
	sockaddr_in ServerAddr;
	memset(&ServerAddr, 0x00, sizeof(sockaddr_in));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = 3550;

	if (connect(ClientSocket, reinterpret_cast<sockaddr*>(&ServerAddr), sizeof(sockaddr_in)) == -1) {
		std::cout << "failed to connect server\n";
		return -1;
	}

	if (send(ClientSocket, "HIHI", 5, 0) == -1) {
		std::cout << "failed to send\n";
		return -1;
	}

	char MessageBuffer[1024] = { "\0" };
	if (recv(ClientSocket, MessageBuffer, 1024, 0) == -1) {
		std::cout << "failed to recv\n";
		return -1;
	}
	std::cout << MessageBuffer << std::endl;

#ifdef _WIN32
	closesocket(ClientSocket);

	if (WSACleanup() != 0) {
		std::cout << "Failed To Cleanup WinSock Application! : " << WSAGetLastError() << std::endl;
		return -1;
	}
#else if
	close(ClientSocket);
#endif
	return 0;
}