#include <iostream>

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Usage : " << argv[0] << " [Return Value]\n";
        return -1;
    }
    return atoi(argv[1]);
}