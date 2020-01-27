#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

const size_t BUFFER_SIZE = 1024;

bool ChangeNonblocking(int Socket) {
    int opt = fcntl(Socket, F_GETFL);
    if(opt < 0) {
        std::cout << "fcntl(F_GETFL) Failure!\n";
        return false;
    }

    opt = opt | O_NONBLOCK;
    if(fcntl(Socket, F_SETFL, opt) < 0) {
        std::cout << "fcntl(F_SETFL) Failure!\n";
        return false;
    }
    return true;
}

int main() {
    int ListenSocket;
    if((ListenSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cout << "Failed To Create Socket!\n";
        return -1;
    }

    sockaddr_in BindAddress;
    BindAddress.sin_family = AF_INET;
    BindAddress.sin_port = htons(3550);
    BindAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(bind(ListenSocket, reinterpret_cast<sockaddr*>(&BindAddress), sizeof(sockaddr_in)) == -1) {
        std::cout << "Failed To Bind Socket!\n";
        return -1;
    }
    if(listen(ListenSocket, SOMAXCONN) == -1) {
        std::cout << "Failed To Listen Socket!\n";
        return -1;
    }

    int MaxFDSNum = ListenSocket;
    fd_set ReadFDS;
    FD_ZERO(&ReadFDS);
    FD_SET(ListenSocket, &ReadFDS);

    while(true) {
        fd_set TempFDS = ReadFDS;
        std::cout << "Select Wait " << MaxFDSNum << std::endl;
        if(select(MaxFDSNum + 1, &TempFDS, NULL, NULL, NULL) > 0) {
            if(FD_ISSET(ListenSocket, &TempFDS)) {
                sockaddr ClientAddress;
                socklen_t AddrLength = sizeof(sockaddr);
                int ClientSocket = accept(ListenSocket, &ClientAddress, &AddrLength);
                if(ClientSocket == -1 || !ChangeNonblocking(ClientSocket)) {
                    std::cout << "Failed To Accept Client!\n";
                    continue;
                }
                FD_SET(ClientSocket, &ReadFDS);
                if(MaxFDSNum < ClientSocket) {
                    MaxFDSNum = ClientSocket;
                }
                std::cout << "Accept New Client!\n";
                continue;
            }

            for(int i = 0; i <= MaxFDSNum; i++) {
                if(FD_ISSET(i, &TempFDS)) {
                    char MessageBuffer[BUFFER_SIZE] = { "\0" };
                    int ReadBytes;
                    while((ReadBytes = read(i, MessageBuffer, BUFFER_SIZE)) > 0 && strncmp(MessageBuffer, "quit", 4) != 0) {
                        std::cout << "Read : " << MessageBuffer << std::endl;
                        write(i, MessageBuffer, strlen(MessageBuffer));
                    }

                    if(ReadBytes < 0 && errno != EAGAIN) {
                        close(i);
                        FD_CLR(i, &ReadFDS);
                        std::cout << "Remove Client!\n";
                    }
                }
            }
        }
    }
    return 0;
}