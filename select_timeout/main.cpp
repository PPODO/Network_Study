#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define STDIN 0

const size_t BUFFER_SIZE = 1024;

int main() {
    fd_set ReadFDS;

    FD_ZERO(&ReadFDS);
    FD_SET(STDIN, &ReadFDS);


    while(true) {
    timeval Timeout;
        Timeout.tv_sec = 5;
        Timeout.tv_usec = 0;
        std::cout << "> ";
        int fdn = select(STDIN + 1, &ReadFDS, NULL, NULL, &Timeout);
        if(fdn == 0) {
            std::cout << "Error : Time Out!\n";
        }
        else {
            char MessageBuffer[BUFFER_SIZE] = { "\0" };
            if(read(STDIN, MessageBuffer, BUFFER_SIZE) > 0 && strncmp(MessageBuffer, "quit", 4) == 0) {
                break;
            }
            std::cout << "Your Message : " << MessageBuffer << std::endl;
        }
        FD_SET(STDIN, &ReadFDS);
    }
    return 0;
}