#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

const size_t BUFFER_SIZE = 1024;

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Usage : " << argv[0] << " [Server IP Address]\n";
        return -1;
    }

    int ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(ClientSocket == -1) {
        std::cout << "Failed To Create Socket!\n";
        return -1;
    }

    sockaddr_in ConnectAddress;
    memset(&ConnectAddress, 0x00, sizeof(ConnectAddress));
    ConnectAddress.sin_family = AF_INET;
    ConnectAddress.sin_addr.s_addr = inet_addr(argv[1]);
    ConnectAddress.sin_port = htons(3550);
    if(connect(ClientSocket, reinterpret_cast<sockaddr*>(&ConnectAddress), sizeof(sockaddr_in)) == -1) {
        std::cout << "Failed To Connect To Server!\n";
        return -1;
    }

    char MessageBuffer[BUFFER_SIZE] = { "\0" };
    std::cin >> MessageBuffer;
    
    if(send(ClientSocket, MessageBuffer, strlen(MessageBuffer), 0) < 0) {
        std::cout << "Failed To Send Data!\n";
        return -1;
    }

    memset(MessageBuffer, 0x00, BUFFER_SIZE);
    if(recv(ClientSocket, MessageBuffer, BUFFER_SIZE, 0) < 0) {
        std::cout << "Failed To Recv Data!\n";
        return -1;
    }
    std::cout << "Recv Data : " << MessageBuffer << std::endl;

    return 0;
}