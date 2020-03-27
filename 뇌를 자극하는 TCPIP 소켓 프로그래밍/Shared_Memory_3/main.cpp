#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <error.h>

const key_t SHARED_MEMORY_KEY_VALUE = 1355;

int main() {
    int shmID = shmget(SHARED_MEMORY_KEY_VALUE, sizeof(int), 0666 | IPC_CREAT);
    if(shmID == -1) {
        std::cout << "Failed To Create Shared Memory!\n";
        return -1;
    }   

    auto Address = shmat(shmID, NULL, 0);
    if(Address == (void*)-1) {
        std::cout << "Attach To Shared Memory Failure! - " << errno << std::endl;
        return -1;
    }

    int* i = (int*)Address;
    pid_t ProcessID = fork();

    if(ProcessID == 0) {
        while(*i < 100) {
            std::cout << *i << std::endl;
            sleep(1);
        }
    }
    else if(ProcessID > 0) {
        while(*i < 100) {
            *i += 2;
            sleep(1);
        }

        wait(&ProcessID);

        if(shmctl(shmID, IPC_RMID, NULL) == -1) {
            std::cout << "Failed To Remove Shared Memory!\n";
            return -1;
        }
    }
    
    shmdt(Address);
    return 0;
}