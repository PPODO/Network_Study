#include <iostream>
#include <unistd.h>

int main() {
    int fd[2];
    if(pipe(fd) == -1) {
        std::cout << "pipe error!\n";
        return -1;
    }
    
    int pid;
    if((pid = fork()) == -1) {
        std::cout << "Failed to Create Child Process\n";
        return -1;
    }
    else if(pid == 0) {
        close(fd[0]);
        for(int i = 0; i < 100; i++) {
            write(fd[1], reinterpret_cast<void*>(&i), sizeof(int));
            std::cout << "Write Data : " << i << std::endl;
            sleep(1);
        }
        close(fd[1]);
    }
    else {
        close(fd[1]);
        for(int i = 0; i < 100; i++) {
            int Data = 0;
            read(fd[0], reinterpret_cast<void*>(&Data), sizeof(int));
            std::cout << "Read data : " << Data << std::endl;
        }
        close(fd[0]);
    }

    return 0;
}