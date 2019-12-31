#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

const size_t BUFFER_SIZE = 1024;

int main() {
    int rfd, wfd;
    if((rfd = open("/tmp/mkfifo_w", O_RDWR)) == -1) {
        std::cout << "Failed To Open Named pipe!\n";
        return -1;
    }
    if((wfd = open("/tmp/mkfifo_r", O_RDWR)) == -1) {
        std::cout << "Failed To Open Named pipe!\n";
        return -1;
    }

    while(true) {
        char MessageBuffer[BUFFER_SIZE] = { "\0" };
        std::cout << "> ";
        std::cin >> MessageBuffer;
        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(BUFFER_SIZE);
        }
        else if(strncmp(MessageBuffer, "quit", 5) == 0) {
            break;
        }
        else {
            write(wfd, MessageBuffer, BUFFER_SIZE);
            memset(MessageBuffer, 0x00, BUFFER_SIZE);
            read(rfd, MessageBuffer, BUFFER_SIZE);
            std::cout << "Server -> " << MessageBuffer << std::endl;
        }
    }
    return 0;
}