#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int FileID = 0;
    if((FileID = open("test.txt", O_RDONLY)) == -1) {
        if(errno == ENOEXEC) {
            std::cout << "File Doesn't Exist!\n";
            return -1;
        }
    }

    char ReadBuffer[1024] = { "\0" };
    if(read(FileID, ReadBuffer, 1024) == -1) {
        close(FileID);
        return -1;
    }

    std::cout << ReadBuffer << std::endl;

    close(FileID);
    return 0;
}