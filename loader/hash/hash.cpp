#include <iostream>
#include "hash.hpp"

int main(int ac, char* av[]) {
    if (ac < 2) {
        std::cout << "Usage: \"checksum <file>\"\n";
        return 1;
    }
    std::cout << calculateHash(av[1]) << std::hex;
    return 0;
}
