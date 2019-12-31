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

    if(access(argv[1], F_OK) == -1) {
        std::cout << "Failed To Access!\n";
        return -1;
    }

    int Socket;
    if((Socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        std::cout << "Failed To Create Socket!\n";
        return -1;
    }

    sockaddr_un ConnectAddress;
    memset(&ConnectAddress, 0x00, sizeof(sockaddr_un));
    ConnectAddress.sun_family = AF_UNIX;
    strncpy(ConnectAddress.sun_path, argv[1], strlen(argv[1]));
    if(connect(Socket, reinterpret_cast<sockaddr*>(&ConnectAddress), sizeof(sockaddr_un))) {
        std::cout << "Failed To Connect Server!\n";
        return -1; 
    }

    while(true) {
        char MessageBuffer[BUFFER_SIZE] = { "\0" };
        std::cin >> MessageBuffer;
        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(BUFFER_SIZE);
        }
        else if(strncmp(MessageBuffer, "quit", 4) == 0) {
            break;
        }
        else {
            send(Socket, MessageBuffer, strlen(MessageBuffer), 0);
            memset(MessageBuffer, 0x00, BUFFER_SIZE);
            recv(Socket, MessageBuffer, BUFFER_SIZE, 0);
            std::cout << "Server -> " << MessageBuffer << std::endl;
        }
    }
    close(Socket);
    return 0;
}