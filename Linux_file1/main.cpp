#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int FileID = 0;
    if((FileID = open("test.txt", O_WRONLY | O_CREAT | O_EXCL)) == -1) {
        if(errno == EEXIST) {
            std::cout << "File Exist!\n";
            close(FileID);
            return -1;
        }
    }

    return 0;
}