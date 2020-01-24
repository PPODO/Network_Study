#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
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

	sockaddr_in ConnectAddress;
	ConnectAddress.sin_family = AF_INET;
	ConnectAddress.sin_port = htons(3550);
	ConnectAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(ListenSocket, reinterpret_cast<sockaddr*>(&ConnectAddress), sizeof(sockaddr_in)) == SOCKET_ERROR) {
		std::cout << "Connect Failure!\n";
		return -1;
	}
	
	HANDLE hEventSelect = WSACreateEvent();
	if (WSAEventSelect(ListenSocket, hEventSelect, FD_READ | FD_OOB) == SOCKET_ERROR) {
		std::cout << "Failed To Create Event Select!\n";
		return -1;
	}

	while (true) {
		auto Result = WaitForSingleObject(hEventSelect, INFINITE);
		switch (Result) {
		case WAIT_OBJECT_0:
		{
			WSANETWORKEVENTS NetworkEvent;
			if (WSAEnumNetworkEvents(ListenSocket, hEventSelect, &NetworkEvent) == SOCKET_ERROR) {
				std::cout << "Event Type Error\n";
			}
			else if (NetworkEvent.lNetworkEvents & FD_READ) {
				char MessageBuffer[256] = { "\0" };
				recv(ListenSocket, MessageBuffer, 256, 0);
				std::cout << "Recv Data : " << MessageBuffer << std::endl;
				if (NetworkEvent.lNetworkEvents & FD_OOB) {
					char Message = 0;
					recv(ListenSocket, &Message, sizeof(char), MSG_OOB);
					std::cout << "OOB Data : " << Message << std::endl;
				}
			}
			
		}
			break;
		default:
			std::cout << "Error\n";
			continue;
		}

	}
	
	WSACleanup();
	return 0;
}