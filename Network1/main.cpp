#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ListenSocket  == -1) {
        std::cout << "Failed To Create Listen Socket!\n";
        return -1;
    }

    
    

    return 0;
}