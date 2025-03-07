#include <iostream>
#include <fstream>

int main() {
    std::ifstream input;
    input.open("testroms/cpu_instrs.gb");
    char read;
    unsigned char low = 0, high = 0;
    while (1) {
        input.get(read);
        std::cout << (short) high << "\t" << (short) low << ":" << (short) read << "\n";
        low++;
        if (low == 0) high++;
        if (input.peek() == EOF) break;
    }
    return 0;
}