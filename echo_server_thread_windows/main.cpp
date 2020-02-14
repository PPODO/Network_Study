#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")

constexpr short PORT_NUMBER = 3550;
constexpr size_t BUFFER_SIZE = 1024;

void Thread_Function(void* Data) {
	SOCKET* ClientSocket = reinterpret_cast<SOCKET*>(Data);
	char MessageBuffer[BUFFER_SIZE] = { "\0" };
	int ReadBytes;

	std::cout << "Thread Start : " << GetCurrentThreadId() << std::endl;
	while ((ReadBytes = recv(*ClientSocket, MessageBuffer, BUFFER_SIZE, 0)) > 0) {
		std::cout << "Recv Data : " << GetCurrentThreadId() << '\t' << MessageBuffer << std::endl;
		send(*ClientSocket, MessageBuffer, ReadBytes, 0);
	}
}

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
	BindAddress.sin_port = htons(PORT_NUMBER);
	BindAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (bind(ListenSocket, reinterpret_cast<sockaddr*>(&BindAddress), sizeof(BindAddress)) == SOCKET_ERROR || listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Failed To Bind or Listen!\n";
		return -1;
	}

	int AddrLength = sizeof(sockaddr_in);
	while (true) {
		sockaddr ClientAddress;
		SOCKET ClientSocket = accept(ListenSocket, &ClientAddress, &AddrLength);
		if (ClientSocket == INVALID_SOCKET) {
			std::cout << "Failed To Accept New Client!\n";
			continue;
		}
		else {
			std::cout << "Accept New Client!\n";
			HANDLE Thread = reinterpret_cast<HANDLE>(_beginthread(Thread_Function, 0, &ClientSocket));
			CloseHandle(Thread);
		}
	}
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}