#include <iostream>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>

int main() {
    rlimit limit;

    if(getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        return -1;
    }
    std::cout << "Open File : " << limit.rlim_cur << ", " << limit.rlim_max << std::endl;

    limit.rlim_cur += 1024;
    // 일반유저는 max값을 변경할 수 없다.
    //limit.rlim_max += 1024;

    if(setrlimit(RLIMIT_NOFILE, &limit) == -1) {
        return -1;
    }
    std::cout << "Open File : " << limit.rlim_cur << ", " << limit.rlim_max << std::endl;
    return 0;
}