#include <iostream>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/un.h>

int main() {
    int Temp = socket(AF_UNIX, SOCK_STREAM, 0);
    if(Temp == -1) {
        std::cout << "Failed To Create Socket!\n";
        return -1;
    }

    sockaddr_un Address;
    Address.sun_family = AF_UNIX;
    strncpy(Address.sun_path, "/tmp/test_file", strlen("/tmp/test_file"));
    // 소켓파일이 생성되는 시점은 bind 함수를 호출할 때임을 알게 됨.
    if(bind(Temp, reinterpret_cast<sockaddr*>(&Address), sizeof(sockaddr_un)) == -1) {
        std::cout << "Failed To Bind Socket!\n";
        close(Temp);
        return -1;  
    }

    unlink("/tmp/test_file");
    close(Temp);
    return 0;
}