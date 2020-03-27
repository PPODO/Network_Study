#include <iostream>
#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <cstdlib>
#else
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

struct CALCULATE_DATA {
public:
    int m_LValue{ 0 };
    int m_RValue{ 0 };
    char m_Operator{ 0 };
    int m_Result{ 0 };
    short m_Error{ 0 };

public:
    CALCULATE_DATA() {};
    CALCULATE_DATA(int LValue, char Operator, int RValue) : m_LValue(htonl(LValue)), m_Operator(Operator), m_RValue(htonl(RValue)) {};
    CALCULATE_DATA(int Result, short Error = 0) : m_Result(htonl(Result)), m_Error(htons(Error)) {}

};

const size_t BUFFER_SIZE = 1024;

int main(int argc, char** argv) {
    if(argc != 4) {
        std::cout << "Usage : " << argv[0] << " [num1] [operator] [num2]\n";
        return -1;
    }    

    #ifdef _WIN32
    WSADATA WinSockData;
    if(WSAStartup(WINSOCK_VERSION, &WinSockData) != 0) {
        std::cout << "Failde To Startup WSA! - " << WSAGetLastError() << std::endl;
        return -1;
    }

    SOCKET ConnectSocket;
    #else
    int ConnectSocket;
    #endif

    if((ConnectSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1){ 
        std::cout << "Failed To Create Socket!\n";
        return -1;
    }

    sockaddr_in ConnectAddress;
    memset(&ConnectAddress, 0x00, sizeof(sockaddr_in));
    ConnectAddress.sin_family = AF_INET;
    ConnectAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    ConnectAddress.sin_port = htons(3550);
    if(connect(ConnectSocket, reinterpret_cast<sockaddr*>(&ConnectAddress), sizeof(sockaddr_in)) == -1) {
        std::cout << "Failed Connect To Server!\n";
        return -1;
    }

    CALCULATE_DATA SendData(atoi(argv[1]), *argv[2], atoi(argv[3]));
    if(send(ConnectSocket, reinterpret_cast<char*>(&SendData), sizeof(SendData), 0) == -1) {
        std::cout << "Failed To Send Data!\n";
        return -1;
    }

    char MessageBuffer[BUFFER_SIZE] = { "\0" };
    int RecvBytes = recv(ConnectSocket, MessageBuffer, BUFFER_SIZE, 0);
    auto RecvData = reinterpret_cast<CALCULATE_DATA*>(MessageBuffer);
    if(RecvBytes == -1 || !RecvData) {
        std::cout << "Failed To Recv Data!\n";
        return -1;
    }

    if(ntohs(RecvData->m_Error) != 0) {
        std::cout << "Calculate Error : " << RecvData->m_Error << std::endl;
    }
    else {
        std::cout << ntohl(SendData.m_LValue) << '\t' << SendData.m_Operator << '\t' << ntohl(SendData.m_RValue) << "\t : " << RecvData->m_Result << std::endl;
    }
    
    #ifdef _WIN32
    closesocket(ConnectSocket);
    WSACleanup();
    #else
    close(ConnectSocket);
    #endif
    return 0;
}