#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main() {
    if(mkfifo("/tmp/myfifo_r", S_IRUSR | S_IWUSR) == -1) {
        std::cout << "Failed To Create Named PIPE! : " << errno;
        return -1;
    
    std::cout << "Succeed!\n";
    return 0;
}