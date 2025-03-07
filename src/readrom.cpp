#include "readrom.hpp"
#include <exception>

Cart::Cart() {};

void Cart::loadRom(std::string filename) {
    file.open(filename);
    if (!file.is_open())
        throw std::runtime_error("Arquivo nao encontrado.");
    return;
};