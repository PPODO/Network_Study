#include <iostream>
#include <unistd.h>

int main() {
    std::cout << "Program Start!!\n";

    pid_t PID = fork();
    if(PID < 0) {
        std::cout << "Fork Failure!\n";
        return -1;
    }
    std::cout << "Fork!!!\n";

    if(PID == 0) {
        std::cout << "I'm Child Process : " << getpid() << std::endl;

        int i = 0;
        while(i < 100) {
            std::cout << "Child : " << i << std::endl;
            i += 1;
            sleep(1);
        }
    }
    else if(PID > 0) {
        std::cout << "I'm Parent Process : " << getpid() << std::endl;

        int i = 0;
        while(i < 200) {
            std::cout << "Parent : " << i << std::endl;
            i += 2;
            sleep(1);
        }
    }
    return 0;
}