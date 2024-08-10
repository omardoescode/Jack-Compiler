#include "JackCompiler.h"
#include <filesystem>
#include <iostream>
int main (int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Invalid number of arguments\n";
        return -1;
    }

    std::filesystem::path in_file = argv[1];

    if (!std::filesystem::exists (in_file)) {
        std::cout << "Path is invalid";
        return -1;
    }

    JackCompiler compiler (in_file);
    compiler.compile ();
}
