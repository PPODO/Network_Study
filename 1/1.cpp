#include "1.h"

int main() {
#ifdef _WIN32
	WSADATA WinSockData;
	if (WSAStartup(WINSOCK_VERSION, &WinSockData) == SOCKET_ERROR) {
		std::cout << "Failed To Initialize WinSock Application! : " << WSAGetLastError() << std::endl;
	}
#endif


#ifdef _WIN32
	if (WSACleanup() == SOCKET_ERROR) {
		std::cout << "Failed To Cleanup WinSock Application! : " << WSAGetLastError() << std::endl;
	}
#endif
	return 0;
}