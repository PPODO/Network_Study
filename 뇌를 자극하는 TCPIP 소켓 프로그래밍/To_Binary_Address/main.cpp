#include <iostream>
#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Usage : " << argv[0] << '\t' << "[Internet Address]\n";
        return -1;
    }

#ifdef _WIN32
    WSADATA WinSockData

    if(WSAStartup(WINSOCK_VERSION, &WinSockData) != 0) {
        std::cout << "WSAStartup Failed!\n";
        return -1;
    }
#endif

    auto IPAddress = inet_addr(argv[1]);
    std::cout << "inet_addr() : " << argv[1] << " -> " << IPAddress << std::endl;
    std::cout << "inet_addr() : " << IPAddress << " -> " << inet_ntoa(*reinterpret_cast<in_addr*>(&IPAddress)) << std::endl;

    if(in_addr AddrInfo; inet_aton(argv[1], &AddrInfo) == -1) {
        std::cout << "Error!\n";
#ifdef _WIN32
    WSACleanup();
#endif
        return -1;
    }
    else {
        std::cout << "inet_aton() : " << argv[1] << " -> " << AddrInfo.s_addr << std::endl;
        std::cout << "inet_aton() : " << IPAddress << " -> " << inet_ntoa(AddrInfo) << std::endl;
    }

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}