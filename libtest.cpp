// Example test file for the library
#include "base64.h"
#include <vector>
#include <iostream>

int main() {
    int count;
    std::cout << "Enter the array length: " << std::endl;
    std::cin >> count;

    //encoding
    std::vector<unsigned char> decArr(count);
    for(int i = 0; i < count; i++) {
        int tmp;
        std::cout << "Enter max 32bit decimal for index " + std::to_string(i) << std::endl;
        std::cin >> tmp;
        decArr[i] = tmp;
    }

    std::cout << std::endl;
    std::cout << base64::encode(decArr) << std::endl;

    //decoding
    std::string input;
    std::cout << "Enter message to decode" << std::endl;
    std::cin >> input;

    std::vector<unsigned char> decoded = base64::decode(input);

    for(int i : decoded) std::cout << std::to_string(i) + ", ";

    std::cout << std::endl;
    return 0;
}
