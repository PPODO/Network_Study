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
    int ListenSocket = 0;
    if((ListenSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cout << "Failed To Create Socket!\n";
        return -1;
    }

    sockaddr_in BindAddress;
    memset(&BindAddress, 0x00, sizeof(sockaddr_in));
    BindAddress.sin_family = AF_INET;
    BindAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    BindAddress.sin_port = htons(3550);
    if(bind(ListenSocket, reinterpret_cast<sockaddr*>(&BindAddress), sizeof(sockaddr_in)) == -1) {
        std::cout << "Failed To Bind Socket!\n";
        return -1;
    }

    if(listen(ListenSocket, SOMAXCONN) == -1) { 
        std::cout << "Failed To Listen Socket!\n";
        return -1;
    }

    while(true) {
        int ClientSocket;
        sockaddr_in ClientAddress;
        socklen_t AddrLen = sizeof(sockaddr_in);
        if((ClientSocket = accept(ListenSocket, reinterpret_cast<sockaddr*>(&ClientAddress), &AddrLen)) == -1) {
            std::cout << "Failed To Accept Client!\n";
            continue;
        }

        char MessageBuffer[BUFFER_SIZE] = { "\0" };
        int RecvBytes = recv(ClientSocket, MessageBuffer, BUFFER_SIZE, 0);
        CALCULATE_DATA* RecvData = reinterpret_cast<CALCULATE_DATA*>(MessageBuffer);

        if(RecvBytes == -1 || !RecvData) {
            std::cout << "Failed To Recv Data!\n";
            close(ClientSocket);
            continue;
        }
    
        auto LValue = ntohl(RecvData->m_LValue);
        auto RValue = ntohl(RecvData->m_RValue);
        CALCULATE_DATA SendData;
        
        switch(RecvData->m_Operator) {
        case '+':
            SendData.m_Result = LValue + RValue;
            break;
        case '-':
            SendData.m_Result = LValue - RValue;
            break;
        case '*':
            SendData.m_Result = LValue * RValue;
            break;
        case '/':
            if(RValue == 0) {
                SendData.m_Error = 2;
                break;
            }
            SendData.m_Result = LValue / RValue;
            break;
        default:
            SendData.m_Error = 1;
            break;
        }
        
        if(send(ClientSocket, reinterpret_cast<char*>(&SendData), sizeof(SendData), 0) == -1) {
            std::cout << "Failed To Send Data!\n";
        }
        close(ClientSocket);
    }
    close(ListenSocket);
    return 0;
}