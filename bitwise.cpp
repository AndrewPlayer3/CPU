#include <iostream>

int main() {

    int hex = 0xA;

    hex <<= 0x4;

    std::cout << std::hex << hex << std::endl;

    return 0;
}