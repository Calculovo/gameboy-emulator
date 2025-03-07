#include "readrom.hpp"

class Emulator {
    private:
        Cart cartridge;
    public:
        Emulator(std::string filename);
        void run();
};