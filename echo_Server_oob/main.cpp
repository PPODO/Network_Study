#include <iostream>
#include <sys/socket.h>
#include <sys/signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int ClientSocket;
void urg_handler(int Signal) {
    char Buffer;
    recv(ClientSocket, &Buffer, sizeof(char), MSG_OOB);
    std::cout << "OOB Data : " << Buffer << '\t' << Signal << std::endl;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Usage : " << argv[0] << " [Port Number]\n";
        return -1;
    }

    std::cout << "pid : " << getpid() << std::endl;

    int ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(ListenSocket == -1) {
        std::cout << "Failed To Create Socket!\n";
        return -1;
    }

    sockaddr_in BindAddress;
    BindAddress.sin_family = AF_INET;
    BindAddress.sin_port = htons(atoi(argv[1]));
    BindAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(bind(ListenSocket, reinterpret_cast<sockaddr*>(&BindAddress), sizeof(BindAddress)) == -1) {
        std::cout << "Failed To Bind Socket!\n";
        return -1;
    }
    if(listen(ListenSocket, SOMAXCONN) == -1) {
        std::cout << "Failed To Listen Socket!\n";
        return -1;
    }

    socklen_t AddrLength = sizeof(sockaddr_in);
    while(true) {
        sockaddr_in ClientAddress;
        ClientSocket = accept(ListenSocket, reinterpret_cast<sockaddr*>(&ClientAddress), &AddrLength);
        if(ClientSocket == -1){
            std::cout << "Failed To Accept!\n";
            continue;
        }
        else {
            std::cout << "Accept!\n";
        }

        signal(SIGURG, urg_handler);
        fcntl(ClientSocket, F_SETOWN, getpid());        

        while(true) {
            char MessageBuffer[1024] = { "\0" };
            if(read(ClientSocket, MessageBuffer, 1024) <= 0) {
                std::cout << "Failed To Recv Data!\n";
                break;
            }

            if(write(ClientSocket, MessageBuffer, strlen(MessageBuffer)) <= 0){
                std::cout << "Failed To Write Data!\n";
                break;
            }
        }
        close(ClientSocket);
    }

    close(ListenSocket);
    return 0;
}