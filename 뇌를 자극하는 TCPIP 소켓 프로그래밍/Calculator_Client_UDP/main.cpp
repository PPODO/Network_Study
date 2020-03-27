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
const size_t COMMAND_BUFFER_SIZE = 256;

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Usage : " << argv[0] << " [Server IP Address]\n";
        return -1;
    }

    int ClientSocket = 0;
    if((ClientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        std::cout << "Failed To Create Socket!\n";
        return -1;
    }

    sockaddr_in ServerAddress;
    memset(&ServerAddress, 0x00, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
    ServerAddress.sin_port = htons(3550);
    socklen_t AddrLen = sizeof(ServerAddress);
    while(true) {
        char CommandMessage[COMMAND_BUFFER_SIZE] = { "\0" };
        std::cout << "> ";
        std::cin >> CommandMessage;
        if(std::cin.fail()) {
            std::cout << "Command Error!\n";
            std::cin.clear();
            std::cin.ignore(COMMAND_BUFFER_SIZE, '\n');
            continue;
        }
        else if(strncmp(CommandMessage, "quit\n", 5) == 0) {
            break;
        }
        else {
            char Operator = 0;
            int LValue = 0, RValue = 0;
            std::cin >> LValue >> Operator >> RValue;
        
            CALCULATE_DATA SendData(LValue, Operator, RValue);
            if(sendto(ClientSocket, reinterpret_cast<const char*>(&SendData), sizeof(SendData), 0, reinterpret_cast<sockaddr*>(&ServerAddress), AddrLen) < 0) {
                std::cout << "Failed To Send Data!\n";
                continue;
            }

            char MessageBuffer[BUFFER_SIZE] = { "\0" };
            sockaddr_in RecvAddress;
            auto RecvBytes = recvfrom(ClientSocket, MessageBuffer, BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&RecvAddress), &AddrLen);
            auto RecvData = reinterpret_cast<CALCULATE_DATA*>(MessageBuffer);
            if(RecvBytes < 0 || !RecvData) {
                std::cout << "Failed To Recv Data!\n";
                continue;
            }

            if(RecvData->m_Error == 0) {
                std::cout << ntohl(SendData.m_LValue) << '\t' << SendData.m_Operator << '\t' << ntohl(SendData.m_RValue) << "\t:\t" << ntohl(RecvData->m_Result) << std::endl;
            }
            else {
                std::cout << "Error : " << ntohs(RecvData->m_Error) << std::endl;
            }
        }
    }
    close(ClientSocket);
    return 0;
}