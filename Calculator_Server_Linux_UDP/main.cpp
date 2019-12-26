#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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

int main() {
    int ServerSocket;
    if((ServerSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        std::cout << "Failed To Create Socket!\n";
        return -1;
    }

    sockaddr_in BindAddress;
    memset(&BindAddress, 0x00, sizeof(BindAddress));
    BindAddress.sin_family = AF_INET;
    BindAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    BindAddress.sin_port = htons(3550);
    if(bind(ServerSocket, reinterpret_cast<sockaddr*>(&BindAddress), sizeof(BindAddress)) == -1) {
        std::cout << "Failed To Bind Socket!\n";
        return -1;
    }

    socklen_t AddrLen = sizeof(sockaddr_in);
    while(true) {
        sockaddr_in ClientAddress;
        char MessageBuffer[BUFFER_SIZE] = { "\0" };
        auto RecvBytes = recvfrom(ServerSocket, MessageBuffer, BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&ClientAddress), &AddrLen);
        auto Data = reinterpret_cast<CALCULATE_DATA*>(MessageBuffer);

        if(RecvBytes < 0 || !Data) {
            std::cout << "Failed To Recv Data!\n";
            continue;
        }

        CALCULATE_DATA SendData;
        int Result = 0;
        int LValue = ntohl(Data->m_LValue);
        int RValue = ntohl(Data->m_RValue);
        switch(Data->m_Operator) {
        case '+':
            Result = LValue + RValue;
            break;
        case '-':
            Result = LValue - RValue;
            break;
        case '*':
            Result = LValue * RValue;
            break;
        case '/':
            if(RValue == 0) {
                SendData.m_Error = htons(1);
                break;
            }
            Result = LValue / RValue;
            break;
        default:
           SendData.m_Error = htons(2);
           break;
        }

        SendData.m_Result = htonl(Result);
        if(sendto(ServerSocket, reinterpret_cast<const char*>(&SendData), sizeof(SendData), 0, reinterpret_cast<sockaddr*>(&ClientAddress), AddrLen) < 0) {
            std::cout << "Failed To Send Data!\n";
            continue;
        }
    }
    close(ServerSocket);
    return 0;
}