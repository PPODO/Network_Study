#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <arpa/inet.h>

const size_t BUFFER_SIZE = 1024;

int main() {
    int ListenSocket;
    if((ListenSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cout << "Failed To Create Listen Socket!\n";
        return -1;
    }

    sockaddr_in BindAddress;
    memset(&BindAddress, 0x00, sizeof(BindAddress));
    BindAddress.sin_family = AF_INET;
    BindAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    BindAddress.sin_port = htons(3550);
    if(bind(ListenSocket, reinterpret_cast<sockaddr*>(&BindAddress), sizeof(BindAddress)) == -1) {
        std::cout << "Failed To Bind Listen Socket!\n";
        return -1;
    }
    if(listen(ListenSocket, SOMAXCONN) == -1) {
        std::cout << "Failed To Create Listen Queue!\n";
        return -1;
    }

    // 자식프로세스는 wait함수를 호출해서 종료코드를 받아와야 좀비프로세스가 안됨.
    // 근데 wait함수를 호출해주면 멀티프로세스를 쓰는 이유가 없어짐(자식프로세스가 종료될때까지 기다려야하기 때문)
    // signal(SIGCHLD <- 자식프로세스와 관련된 플래그인듯함, SIG_IGN <- 무시하겠다는 플래그 같음).
    // 를 해주면 자식 프로세스를 기다리지 않아도 알아서 종료됨 -> 좀비프로세스 없음! 와! 개꿀!
    signal(SIGCHLD, SIG_IGN);
    socklen_t AddrLen = sizeof(sockaddr_in);
    while(true) {
        sockaddr_in ClientAddress;
        int ClientSocket = accept(ListenSocket, reinterpret_cast<sockaddr*>(&ClientAddress), &AddrLen);
        if(ClientSocket == -1) {
            std::cout << "Failed To Accept Client!\n";
            continue;
        }

        pid_t PID = fork();
        if(PID == 0) {
            char MessageBuffer[BUFFER_SIZE] = { "\0" };
            int ReadBytes = 0;
            while((ReadBytes = recv(ClientSocket, MessageBuffer, BUFFER_SIZE, 0)) > 0) {
                std::cout << "Read Data : " << inet_ntoa(ClientAddress.sin_addr) << "(" << ClientAddress.sin_port << ") -> " << MessageBuffer << std::endl;
                if(send(ClientSocket, MessageBuffer, ReadBytes, 0) < 0) {
                    std::cout << "Failed To Send Data!\n";
                    close(ClientSocket);
                    return -1;
                }
            }
            close(ClientSocket);
            return 0;
        }
        else if(PID > 0) {
            close(ClientSocket);
        }
        else {
            std::cout << "Failed To Create Child Process!\n";
            continue;
        }
    }
    close(ListenSocket);
    return 0;
}