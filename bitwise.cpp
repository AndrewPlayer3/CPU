#include <iostream>

int main() {

    int arr[4];
    
    arr[0] = 0x41;
    arr[1] = 0x42;
    arr[2] = 0x43;
    arr[3] = 0x44;

    int hex;
    std::string str;
    for(int i : arr) {
        str += (char)i;
    }

    std::cout << str << std::endl;

    return 0;
}