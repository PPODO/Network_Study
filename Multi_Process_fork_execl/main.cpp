#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

const size_t BUFFER_SIZE = 256;

int main() {
    std::cout << "My Shell Ver 1.0\n";

    while(true) {
        char Buffer[BUFFER_SIZE] = { "\0" };
        std::cout << "> ";
        std::cin >> Buffer;
        if(strncmp(Buffer, "quit", 4) == 0) {
            std::cout << "shutdown\n";
            break;
        }

        pid_t PID = fork();
        if(PID == 0) {
            if(execl(Buffer, Buffer, NULL) < 0) {
                std::cout << "Execl Failure!\n";
                exit(0);
            }
        }
        else if(PID > 0) {
            int Proc_status = 0;
            std::cout << "Child Wait\n";
            wait(&Proc_status);
            std::cout << "Child Exit\n";
        }
    }
    return 0;
}