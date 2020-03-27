#include <iostream>
#include <pthread.h>
#include <sys/sem.h>
#include <unistd.h>

constexpr size_t MAX_THREAD = 2;
constexpr key_t SHARED_KEY_VALUE = 1890;

int SemaphoreKey = 0;
sembuf GET_SEMAPHORE { 0, -1, SEM_UNDO };
sembuf RELEASE_SEMAPHORE { 0, 1, SEM_UNDO };

void* Thread_Function(void* Data) {
    int* Count = reinterpret_cast<int*>(Data);
    pthread_t Thread_ID = pthread_self();
    
    while(*Count < 20) {
        semop(SemaphoreKey, &GET_SEMAPHORE, 1);
        std::cout << Thread_ID << " : " << *Count << std::endl;
        *Count += 1;
        semop(SemaphoreKey, &RELEASE_SEMAPHORE, 1);
    }
    return nullptr;
}

int main() {
    pthread_t Threads[MAX_THREAD];
    int Count = 0;

    SemaphoreKey = semget(SHARED_KEY_VALUE, 1, IPC_CREAT | 0666);
    if(SemaphoreKey == -1 || semctl(SemaphoreKey, 0, SETVAL, 1)) {
        std::cout << "Failed To Create Semaphore\n";
        return -1;
    }
    
    for(int i = 0; i < MAX_THREAD; i++) {
        pthread_create(&Threads[i], NULL, Thread_Function, &Count);
    }

    while(Count < 20) {
        semop(SemaphoreKey, &GET_SEMAPHORE, 1);
        std::cout << "Main Thread : " << Count << std::endl;
        semop(SemaphoreKey, &RELEASE_SEMAPHORE, 1);
    }

    for(int i = 0; i < MAX_THREAD; i++) {
        pthread_join(Threads[i], NULL);
    }

    semctl(SemaphoreKey, 0, IPC_RMID);
    return 0;
}