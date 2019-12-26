#include <iostream>
#include <unistd.h>

int main() {
    std::cout << "-- START " << getpid() << " --\n";
    execl("./exec_main", "exec_main", NULL);

    return 0;
}