#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

constexpr unsigned short PORT_NUMBER = 3550;
constexpr size_t BUFFER_SIZE = 1024;

void* Thread_Function(void* argv) {
    int* ClientSocket = reinterpret_cast<int*>(argv);
    char MessageBuffer[BUFFER_SIZE] = { "\0" };
    ssize_t RecvBytes = 0;

    while((RecvBytes = recv(*ClientSocket, MessageBuffer, BUFFER_SIZE, 0)) > 0) {
        std::cout << "Recv Data : " << MessageBuffer << std::endl;
        send(*ClientSocket, MessageBuffer, RecvBytes, 0);
    }
    close(*ClientSocket);
    std::cout << "Worker Thread End\n";

    return nullptr;
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
        std::cout << "Failed To Bind or Listen Socket!\n";
        return -1;
    }

    socklen_t AddrLength = sizeof(sockaddr);
    while(true) {
        sockaddr ClientAddress;
        int ClientSocket = accept(ListenSocket, &ClientAddress, &AddrLength);
        if(ClientSocket == -1) {
            std::cout << "Failed To Accept New Client!\n";
            continue;
        }
        else {
            std::cout << "New Client!\n";
            pthread_t Thread;
            pthread_create(&Thread, NULL, Thread_Function, &ClientSocket);
            pthread_detach(Thread);
        }
    }

    return 0;
}