#include <iostream>
#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#else
#include <netdb.h>
#include <arpa/inet.h>
#endif

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Usage : " << argv[0] << '\t' << "[Domain Name]\n";
        return -1;
    }

#ifdef _WIN32
    WSADATA WinSockData

    if(WSAStartup(WINSOCK_VERSION, &WinSockData) != 0) {
        std::cout << "WSAStartup Failed!\n";
        return -1;
    }
#endif

    if(auto hostname = gethostbyname(argv[1])) {
        std::cout << "Host Name : " << hostname->h_name << std::endl;

        while(*hostname->h_addr_list != nullptr) {
            std::cout << inet_ntoa(*reinterpret_cast<in_addr*>(hostname->h_addr_list)) << std::endl;
            hostname->h_addr_list++;
        }
    }
    else {
        std::cout << "Error!\n";
#ifdef _WIN32
    WSACleanup();
#endif
        return -1;
    }

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}