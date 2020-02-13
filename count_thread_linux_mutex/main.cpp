#include <iostream>
#include <pthread.h>
#include <unistd.h>

constexpr size_t MAX_THREAD = 2;
pthread_mutex_t g_MutexLock;

void* Thread_Function(void* Data) {
    int* Count = reinterpret_cast<int*>(Data);
    pthread_t Thread_ID = pthread_self();

    while(*Count < 30) {
        pthread_mutex_lock(&g_MutexLock);
        std::cout << Thread_ID << " : " << *Count << std::endl;
        *Count += 1;
        pthread_mutex_unlock(&g_MutexLock);
    }

    return nullptr;
}

int main() {
    pthread_t Threads[MAX_THREAD];
    int Count = 0;
    
    if(pthread_mutex_init(&g_MutexLock, NULL) != 0) {
        std::cout << "Failed To Initialize Mutex!\n";
        return -1;
    }

    for(int i = 0; i < MAX_THREAD; i++) {
        pthread_create(&Threads[i], NULL, Thread_Function, &Count);
    }

    while(Count < 30) {
        pthread_mutex_lock(&g_MutexLock);
        std::cout << "Main Thread : " << Count << std::endl;
        pthread_mutex_unlock(&g_MutexLock);
    }

    for(int i = 0; i < MAX_THREAD; i++) {
        pthread_join(Threads[i], NULL);
    }
    pthread_mutex_destroy(&g_MutexLock);
    return 0;
}