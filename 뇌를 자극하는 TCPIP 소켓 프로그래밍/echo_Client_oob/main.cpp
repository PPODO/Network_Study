#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Usage : " << argv[0] << " [Port Number]\n";
        return -1;
    }

    int ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(ClientSocket == -1) {
        std::cout << "Failed To Create Client Socket!\n";
        return -1;
    }

    sockaddr_in ConnectAddress;
    ConnectAddress.sin_family = AF_INET;
    ConnectAddress.sin_port = htons(atoi(argv[1]));
    ConnectAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(ClientSocket, reinterpret_cast<sockaddr*>(&ConnectAddress), sizeof(ConnectAddress)) == -1) {
        std::cout << "Failed To Connect To Server!\n";
        return -1;
    }

    int WriteCount = 0;
    while(true) {
        std::string InputBuffer;
        std::getline(std::cin, InputBuffer);
        if(write(ClientSocket, InputBuffer.c_str(), InputBuffer.length()) <= 0) {
            std::cout << "Failed To Write Data!\n";
            break;  
        }

        if((WriteCount % 2) == 0) {
            send(ClientSocket, "9", sizeof(char), MSG_OOB);
        }

        char MessageBuffer[1024] = { "\0" };
        if(read(ClientSocket, MessageBuffer, 1024) <= 0) {
            std::cout << "Failed To Recv Data!\n";
            break;
        }
        std::cout << "Recv : " << MessageBuffer << std::endl;
        WriteCount++;
    }

    close(ClientSocket);
    return 0;
}