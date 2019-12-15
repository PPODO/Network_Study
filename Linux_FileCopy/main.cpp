#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
    if(argc != 3) {
        std::cout << "Usage : " << argv[0] << " [source file] [dest file]\n";
        return -1;
    }
    
    int SourceFileID;
    if((SourceFileID = open(argv[1], O_RDONLY)) == -1) {
        std::cout << "failed to open source file!\n";
        return -1;
    }

    int DestFileID;
    if((DestFileID = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
        if(errno == EEXIST) {
            std::cout << "file already exists!\n";
        }
        else {
            std::cout << "failed to open dest file!\n";
        }
        return -1;
    }

    char ReadBuffer[1024] = { "\0" };
    int ReadBytes = 0;
    int TotalBytes = 0;
    while((ReadBytes = read(SourceFileID, ReadBuffer, 1024)) > 0) {
        std::cout << "Readed Bytes : " << ReadBytes << std::endl;
        TotalBytes += write(DestFileID, ReadBuffer, ReadBytes);
        memset(ReadBuffer, 0x00, ReadBytes);
    }
    std::cout << "Total Copy Bytes : " << TotalBytes << std::endl;
    return 0;
}