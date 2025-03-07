#include "emulator.hpp"
#include <iostream>

Emulator::Emulator(std::string filename) {
    cartridge.loadRom(filename);
}

void Emulator::run() {
    std::cout << "Not implemented yet." << std::endl;
}