#include <iostream>
#include "hash.hpp"

int main(int argc, char** argv) {
    if (argc < 2 || !ghc::filesystem::exists(argv[1])) {
        std::cout << "Usage: \"checksum <file>\"\n";
        return 1;
    }
    std::cout << calculateHash(argv[1]) << std::endl;
    return 0;
}
