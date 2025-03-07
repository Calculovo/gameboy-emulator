#include <fstream>
#include <string>

class Cart {
    private:
        std::ifstream file;
    public:
        Cart();
        void loadRom(std::string filename);
};