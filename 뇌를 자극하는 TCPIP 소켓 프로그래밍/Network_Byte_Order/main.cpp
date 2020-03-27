#include <iostream>

#define BIG 0
#define LITTLE 1

bool Check_Endian() {
    int i = 0x00000001;
    return (reinterpret_cast<char*>(&i)[0] ? LITTLE : BIG);
}

int main() {
    std::cout << Check_Endian() << std::endl;

    return 0;
}