#include <iostream>
#include <pthread.h>
#include <unistd.h>

constexpr size_t MAX_THREAD = 2;

void* Thread_Function(void* Data) {
    int* Count = reinterpret_cast<int*>(Data);
    pthread_t Thread_ID = pthread_self();
    
    while(*Count < 20) {
        std::cout << Thread_ID << " : " << *Count << std::endl;
        *Count += 1;
        sleep(1);
    }
    return nullptr;
}

int main() {
    pthread_t Threads[MAX_THREAD];
    int Count = 0;
    
    for(int i = 0; i < MAX_THREAD; i++) {
        pthread_create(&Threads[i], NULL, Thread_Function, &Count);
        usleep(5000);
    }

    while(Count < 20) {
        std::cout << "Main Thread : " << Count << std::endl;
    }

    for(int i = 0; i < MAX_THREAD; i++) {
        pthread_join(Threads[i], NULL);
    }
    return 0;
}