#include "emulator.hpp"
#include <iostream>
#include <exception>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Modo de usar: ./emu <arquivo rom>" << std::endl;
        return -1;
    }
    try {
        Emulator emu(argv[1]);
        emu.run();
    } catch (std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        return -2; 
    }
    return 0;
}