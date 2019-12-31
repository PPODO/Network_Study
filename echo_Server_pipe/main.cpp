#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

const size_t BUFFER_SIZE = 1024;

int main() {
    
    mkfifo("/tmp/mkfifo_r", S_IRUSR | S_IWUSR);
    mkfifo("/tmp/mkfifo_w", S_IRUSR | S_IWUSR);

    int rfd;
    if((rfd = open("/tmp/mkfifo_r", O_RDWR)) == -1) {
        std::cout << "Failed To Open Named pipe!\n";
        return -1; 
    }

    int wfd;
    if((wfd = open("/tmp/mkfifo_w", O_RDWR)) == -1) {
        std::cout << "Failed To Open Named pipe!\n";
        return -1;
    }

    char MessageBuffer[BUFFER_SIZE] = { "\0" };
    while(true) {   
        if(read(rfd, MessageBuffer, BUFFER_SIZE) < 0) {
            std::cout << "Failed To Read Data!\n";
            return -1;
        }
        std::cout << "Read : " << MessageBuffer << std::endl;
        write(wfd, MessageBuffer, BUFFER_SIZE);
        // 파일의 처음 위치로 돌아감.
        lseek(wfd, 0, SEEK_SET);
    }
    return 0;
}