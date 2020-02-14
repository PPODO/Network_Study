#include <iostream>
#include <pthread.h>
#include <unistd.h>

constexpr size_t ARRAY_SIZE = 100;
constexpr size_t MAX_THREAD = 4;

pthread_mutex_t g_MutexLock;
pthread_cond_t g_ConditionalVariable;

int* g_DataArray;
int g_SumArray[MAX_THREAD];

struct Data_Info {
    int* m_Address;
    int m_Index;
};

void* Thread_Function(void* Data) {
    Data_Info* Information = reinterpret_cast<Data_Info*>(Data);

    pthread_mutex_lock(&g_MutexLock);
    std::cout << "Start " << Information->m_Index << " Thread\n";
    pthread_cond_wait(&g_ConditionalVariable, &g_MutexLock);
    pthread_mutex_unlock(&g_MutexLock);

    int Sum = 0, LoopCount = ARRAY_SIZE / MAX_THREAD;
    for(int i = 0; i < LoopCount; i++) {
        Sum += Information->m_Address[(Information->m_Index * 25) + i];
    }

    pthread_mutex_lock(&g_MutexLock);
    std::cout << "(" << Information->m_Index << ") " << Sum << std::endl;
    pthread_mutex_unlock(&g_MutexLock);
    g_SumArray[Information->m_Index] = Sum;

    return nullptr;
}

int main() {
    pthread_t ThreadID[MAX_THREAD];
    
    if(!(g_DataArray = new int[ARRAY_SIZE])) {
       std::cout << "Bad Aolloc!\n";
       return -1; 
    }
    
    if(pthread_mutex_init(&g_MutexLock, NULL) != 0 || pthread_cond_init(&g_ConditionalVariable, NULL)) {
        std::cout << "Failed To Initialize Mutex or Conditional Variable\n";
        return -1;
    }

    Data_Info Information[MAX_THREAD];
    for(int i = 0; i < MAX_THREAD; i++) {
        Information[i].m_Address = g_DataArray;
        Information[i].m_Index = i;
        pthread_create(&ThreadID[i], NULL, Thread_Function, &Information[i]);
        usleep(100);
    }

    for(int i = 0; i < ARRAY_SIZE; i++) {
        *(g_DataArray + i) = i;
    }

    pthread_cond_broadcast(&g_ConditionalVariable);

    int Sum = 0;
    for(int i = 0; i < MAX_THREAD; i++) {
        pthread_join(ThreadID[i], NULL);
        Sum += g_SumArray[i];
    }
    std::cout << "SUM (0 - 99) : " << Sum << std::endl;

    pthread_mutex_destroy(&g_MutexLock);
    pthread_cond_destroy(&g_ConditionalVariable);
    return 0;
}