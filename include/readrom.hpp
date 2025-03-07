#include <fstream>
#include <string>
typedef unsigned char u8;

class Cart {
    private:
        std::ifstream file;
        int romBanks;
        u8 newLicensee[2];
        u8 oldLicensee;
        u8 type;
        u8 mapperChip;
        u8 ramBanks;
        bool battery;
        bool timer;
        bool rumble;
        bool sensor;
        bool overseasOnly;
        u8 version;
        u8 globalChecksum[2];
    public:
        Cart();
        void loadRom(std::string filename);
        void loadType();
        u8 nextChar();
};