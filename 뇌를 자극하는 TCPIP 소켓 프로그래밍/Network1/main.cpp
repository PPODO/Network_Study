#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <vector>
#include <atomic>

pthread_mutex_t g_ClientListMutex;
std::vector<int> g_ClientSockList;
std::atomic<bool> g_ProcessRunState; 

void* AcceptThreadWork(void* arg) {
    int ListenSocket = *reinterpret_cast<int*>(arg);
    sockaddr_in ClientSockAddr;
    socklen_t socksize = sizeof(sockaddr_in);
    memset(&ClientSockAddr, 0x00, socksize);

    while(g_ProcessRunState.load()) {
        int ClientSocket = accept(ListenSocket, reinterpret_cast<sockaddr*>(&ClientSockAddr), &socksize);
        if(ClientSocket == -1) {
            std::cout << "Accept Error!\n";
            continue;
        }
        pthread_mutex_lock(&g_ClientListMutex);
        g_ClientSockList.push_back(ClientSocket);
        pthread_mutex_unlock(&g_ClientListMutex);
    }
    return nullptr;
}

void* CommandThreadWork(void* arg) {
    while(true) {
        char C = 0;
        std::cin >> C; 
        if(std::cin.fail()) {
            std::cout << "Wrong Input!\n";
            std::cin.clear();
            std::cin.ignore(256);
            continue;
        }
        else if(C == 'q' || C == 'Q') {
            g_ProcessRunState.exchange(false);
            break;
        }
    }
    return nullptr;
}

int main() {
    g_ProcessRunState.exchange(true);
    // 두번 째 인자는 뮤텍스의 속성(fast, recurisev, error checking) 기본은 fast임
    pthread_mutex_init(&g_ClientListMutex, nullptr);

    int ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ListenSocket  == -1) {
        std::cout << "Failed To Create Listen Socket!\n";
        return -1;
    }

    sockaddr_in BindAddress;
    memset(&BindAddress, 0x00, sizeof(sockaddr_in));
    BindAddress.sin_family = AF_INET;
    BindAddress.sin_addr.s_addr = INADDR_ANY;
    BindAddress.sin_port = htons(3550);

    if(bind(ListenSocket, reinterpret_cast<sockaddr*>(&BindAddress), sizeof(sockaddr_in)) == -1) {
        std::cout << "Failed To Bind Listen Socket!\n";
        return -1;
    }
    if(listen(ListenSocket, SOMAXCONN) == -1) {
        std::cout << "Failde To Listen Socket!\n";
        return -1;
    }

    pthread_t CommandThread = 0;
    pthread_create(&CommandThread, nullptr, CommandThreadWork, nullptr);

    pthread_t AcceptThread = 0;
    pthread_create(&AcceptThread, nullptr, AcceptThreadWork, &ListenSocket);
    
    while(g_ProcessRunState.load()) {
        pthread_mutex_lock(&g_ClientListMutex);
        for(auto Client = g_ClientSockList.begin(); Client != g_ClientSockList.end(); ++Client) {
            if(*Client == -1 || send(*Client, "Connected!", strlen("Connected!"), 0) == -1) {
                std::cout << "Invalid Socket Value!\n";
                Client = g_ClientSockList.erase(Client);
                continue;
            }
        }
        pthread_mutex_unlock(&g_ClientListMutex);
    }

    shutdown(ListenSocket, SHUT_RDWR);
    // 두번 째 인자는 스레드함수(지금은 AcceptThreadWork)가 리턴하는 값을 저장함.
    pthread_join(AcceptThread, nullptr);
    pthread_join(CommandThread, nullptr);

    close(ListenSocket);
    for(auto It : g_ClientSockList) {
        close(It);
    }

    pthread_mutex_destroy(&g_ClientListMutex);

    std::cout << "Shutdown!\n";
    return 0;
}