#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

constexpr size_t BUFFER_SIZE = 1024;

int SetNonblockSocket(int Socket) {
    int Flags = 0;
    if((Flags = fcntl(Socket, F_GETFL, 0)) == -1) {
        Flags = 0;
    }
    return fcntl(Socket, F_SETFL, Flags | O_NONBLOCK);
}

int main() {
    int ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(ListenSocket == -1) {
        std::cout << "Failed To Create Listen Socket!\n";
        return -1;
    }

    sockaddr_in BindAddress;
    BindAddress.sin_family = AF_INET;
    BindAddress.sin_port = htons(3550);
    BindAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(bind(ListenSocket, reinterpret_cast<sockaddr*>(&BindAddress), sizeof(sockaddr_in)) == -1 || listen(ListenSocket, SOMAXCONN) == -1) {
        std::cout << "Failed To Bind or Listen!\n";
        return -1;
    }

    SetNonblockSocket(ListenSocket);
    socklen_t AddrLength = sizeof(sockaddr);
    int MaxSocketNumber = ListenSocket;
    int ClientSockets[1024];
    while(true) {
        sockaddr ClientAddress;
        int ClientSocket = accept(ListenSocket, &ClientAddress, &AddrLength);
        if(ClientSocket == -1) {
            if(errno != EAGAIN) {
                std::cout << "Accept Failure!\n";
            }
        }
        else {
            std::cout << "New Client!\n";
            SetNonblockSocket(ClientSocket);
            if(ClientSocket > MaxSocketNumber) {
                MaxSocketNumber = ClientSocket;
            }
            ClientSockets[ClientSocket] = 1;
        }

        for(int i = 0; i < MaxSocketNumber + 1; i++) {
            if(ClientSockets[i] != 1) { continue; }
            char MessageBuffer[BUFFER_SIZE] = { "\0" };
            int ReadBytes = recv(i, MessageBuffer, BUFFER_SIZE, 0);
            if(ReadBytes < 0) {
                if(errno != EAGAIN) {
                    std::cout << "Disconnect Socket!\n";
                    close(i);
                    ClientSockets[i] = -1;
                }
            }
            else if(ReadBytes == 0) {
                std::cout << "Disconnect Socket!\n";
                close(i);
                ClientSockets[i] = -1;
            }
            else {
                std::cout << "Recv Data : " << MessageBuffer << std::endl;
                send(i, MessageBuffer, ReadBytes, 0);
            }
        }
    }
    close(ListenSocket);
    return 0;
}