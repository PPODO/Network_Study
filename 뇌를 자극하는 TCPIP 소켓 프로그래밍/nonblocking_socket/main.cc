#include <iostream>
#include <fcntl.h>

int main() {
    int Socket;    
    int Flags;
    // 현재 소켓의 정보를 유지한 채, nonblocking 특성을 추가해야함.
    if((Flags = fcntl(Socket, F_GETFL, 0)) == -1) {
        Flags = 0;
    }
    fcntl(Socket, F_SETFL, Flags | O_NONBLOCK);
    
    return 0;
}