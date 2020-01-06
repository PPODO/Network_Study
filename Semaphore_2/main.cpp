#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <wait.h>

const key_t SEMAPHORE_KEY_VALUE = 1012;
const key_t SHARED_MEMORY_KEY_VALUE = 1641;
sembuf GET_SEMAPHORE{ 0, -1, SEM_UNDO };
sembuf RELEASE_SEMAPHORE{ 0, 1, SEM_UNDO };

int main() {
    int SharedMemoryID = shmget(SHARED_MEMORY_KEY_VALUE, sizeof(int), IPC_CREAT | 0666);
    int SemaphoreID = semget(SEMAPHORE_KEY_VALUE, 1, IPC_CREAT | 0666);
    if(SharedMemoryID == -1 || SemaphoreID == -1) {
        std::cout << "Failed To Create IPC.\n";
        return -1;
    }

    if(semctl(SemaphoreID, 0, SETVAL, 1) == -1) {
        std::cout << "Failed To Initialize Semaphore.\n";
        return -1;
    }

    auto SharedMemory = shmat(SharedMemoryID, NULL, 0);
    if(SharedMemory == (int*)-1) {
        std::cout << "Failed To Attach Memory.\n";
        return -1;
    }

    pid_t PID = fork();
    if(PID == 0) {
        int* Cal_Value = (int*)SharedMemory;

        while(*Cal_Value < 100) {
            int local_val = 0;
            if(semop(SemaphoreID, &GET_SEMAPHORE, 1) == -1) {
                std::cout << "Get Semaphore Failure.\n";
                wait(&PID);
                break;
            }
            local_val = *Cal_Value + 1;
            sleep(2);
            *Cal_Value = local_val;
            std::cout << *Cal_Value << std::endl;
            semop(SemaphoreID, &RELEASE_SEMAPHORE, 1);
        }

        shmdt(SharedMemory);
        return 0;
    }
    else if(PID > 0) {
        int* Cal_Value = (int*)SharedMemory;

        while(*Cal_Value < 100) {
            int local_val = 0;
            if(semop(SemaphoreID, &GET_SEMAPHORE, 1) == -1) {
                std::cout << "Get Semaphore Failure.\n";
                wait(&PID);
                break;
            }
            local_val = *Cal_Value + 1;
            sleep(1);
            *Cal_Value = local_val;
            std::cout << *Cal_Value << std::endl;
            semop(SemaphoreID, &RELEASE_SEMAPHORE, 1);
        }
        wait(&PID);
    }
    else {
        std::cout << "Fork Failure!\n";
    }
    shmdt(SharedMemory);
    shmctl(SharedMemoryID, IPC_RMID, NULL);
    semctl(SemaphoreID, 0, IPC_RMID);

    return 0;
}