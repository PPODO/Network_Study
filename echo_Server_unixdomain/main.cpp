#include <iostream>
#include <sys/un.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

const size_t BUFFER_SIZE = 1024;

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Usage : " << argv[0] << " [Socket file name]\n";
        return -1;
    }
    // 인터넷 소켓 프로그램에서 원격 프로세스의 위치는 IP와 포트번호로 결정되지만, 
    // 유닉스 도메인 소켓에서는 소켓파일의 경로로 위치가 결정됨.
    // 그러므로 기존의 사용하던 소켓 파일이 있는지는 검사해야함.
    // 아래의 함수가 그 역할을 맡음.
    if(access(argv[0], F_OK) == 0) {
        // 기존의 사용하던 파일이 있을 경우 삭제를 해야함.
        // 아래의 함수가 그 역할을 맡음.
        unlink(argv[0]);
    }

    int ListenSocket;
    if((ListenSocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        std::cout << "Failed To Create Listen Socket!\n";
        return -1;
    }

    sockaddr_un Address;
    memset(&Address, 0x00, sizeof(sockaddr_un));
    Address.sun_family = AF_UNIX;
    strncpy(Address.sun_path, argv[1], strlen(argv[1]));
    if(bind(ListenSocket, reinterpret_cast<sockaddr*>(&Address), sizeof(sockaddr_un)) == -1) {
        std::cout << "Failed To Bind Socket!\n";
        return -1;
    }
    if(listen(ListenSocket, SOMAXCONN) == -1) {
        std::cout << "Failed To Listen Socket!\n";
        return -1;
    }

    socklen_t AddrLen = sizeof(sockaddr_un);
    while(true) {
        sockaddr_un ClientAddress;
        int ClientSocket = accept(ListenSocket, reinterpret_cast<sockaddr*>(&ClientAddress), &AddrLen);
        if(ClientSocket == -1) {
            std::cout << "Failed To Accept New Client!\n";
            continue;
        }
        char MessageBuffer[BUFFER_SIZE] = { "\0" };
        size_t RecvBytes = 0;
        if((RecvBytes = recv(ClientSocket, MessageBuffer, BUFFER_SIZE, 0)) < 0) {
            std::cout << "Failed To Recv Data!\n";
            close(ClientSocket);
            continue;
        }
        std::cout << "==> " << MessageBuffer << std::endl;
        send(ClientSocket, MessageBuffer, RecvBytes, 0);
        close(ClientSocket);
    }
    close(ListenSocket);
    return 0;
}