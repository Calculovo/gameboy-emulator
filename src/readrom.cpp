#include "readrom.hpp"
#include <exception>
#include <iostream>
#include <map>
#define NINTENDO_LOGO_LEN 48

const u8 NINTENDO_LOGO[48] = {
    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
    0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
    0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
    0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
    0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
};

Cart::Cart() {};

int add = 0x104;
u8 Cart::nextChar() {
    char p;
    file.get(p);
    return (u8) p;
}

//no mapper is 0
//MBCn are mapped to n
//MMMn are 100+n
//pocket camera is 99
//Bandai tama5 is 55
//HuCn is 200+n
const u8 mappers[][7] = {
    //INP, MAP, RAM  , BATRY, TIMER, RUMBL, SENSOR
    {0x00,   0, false, false, false, false, false},
    {0x01,   1, false, false, false, false, false},
    {0x02,   1, true , false, false, false, false},
    {0x03,   1, true , true , false, false, false},
    {0x05,   2, false, false, false, false, false},
    {0x06,   2, false, false, false, false, false},
    {0x08,   0, true , false, false, false, false},
    {0x09,   0, true , true , false, false, false},
    {0x0B, 100, false, false, false, false, false},
    {0x0C, 100, true , false, false, false, false},
    {0x0D, 100, true , true , false, false, false},
    {0x0F,   3, false, true , true , false, false},
    {0x10,   3, true , true , true , false, false},
    {0x11,   3, false, false, false, false, false},
    {0x12,   3, true , false, false, false, false},
    {0x13,   3, true , true , false, false, false},
    {0x19,   5, false, false, false, false, false},
    {0x1A,   5, true , false, false, false, false},
    {0x1B,   5, true , true , false, false, false},
    {0x1C,   5, false, false, false, true , false},
    {0x1D,   5, true , false, false, true , false},
    {0x1E,   5, true , true , false, true , false},
    {0x20,   6, false, false, false, false, false},
    {0x22,   7, true , true , false, true , true },
    {0xFC,  99, false, false, false, false, false},
    {0xFD,  55, false, false, false, false, false},
    {0xFE, 203, false, false, false, false, false},
    {0xFF, 201, true , true , false, false, false},
};

void Cart::loadType() {
    for (const u8* mapperSet : mappers) {
        if (this->type != mapperSet[0]) continue;
        this->mapperChip = mapperSet[1];
        this->ramBanks = mapperSet[2] ? 1 : 0;
        this->battery = mapperSet[3];
        this->timer = mapperSet[4];
        this->rumble = mapperSet[5];
        this->sensor = mapperSet[6];
        return;
    }
    throw std::runtime_error("Mapper desconhecido.");
}

void Cart::loadRom(std::string filename) {
    file.open(filename);
    if (!file.is_open())
        throw std::runtime_error("Arquivo nao encontrado.");
    //skip boot ROM $0000-00FF
    file.ignore(0x100, EOF);
    //skip entry point $0100-0103
    file.ignore(4, EOF);
    //verify Nintendo logo $0104-0133
    u8 p;
    for (int i = 0; i < NINTENDO_LOGO_LEN; i++) {
        p = nextChar();
        if (NINTENDO_LOGO[i] == p) continue;
        throw std::runtime_error("ROM nao oficial detectada.");
    }
    u8 checksum = 0;
    //title $0134-0143
    std::cout << "Game Title:";
    for (int i = 0; i < 0x10; i++) {
        p = nextChar();
        checksum -= p+1;
        if (p == 0) continue;
        std::cout << p;
    }
    std::cout << "\n";
    //GCB flag $0143
    if (p == 0xC0) throw std::runtime_error("Game Boy Color only game.");
    //New licensee code $0144-0145
    for (int i = 0; i < 2; i++) {
        newLicensee[i] = nextChar();
        checksum -= newLicensee[i]+1;
    }
    //SGB flag $0146 (ignored for now)
    p = nextChar();
    checksum -= p+1;
    //Cart type $0147
    type = nextChar();
    checksum -= type+1;
    this->loadType();
    std::cout << "Type      :" << std::hex << type << " " << mapperChip << "\n";
    std::cout << "RAM?      :" << (int) ramBanks << "\n";
    std::cout << "BATTERY?  :" << battery << "\n";
    std::cout << "TIMER?    :" << timer << "\n";
    std::cout << "RUMBLE?   :" << rumble << "\n";
    std::cout << "SENSOR?   :" << sensor << "\n";
    //ROM Size $0148
    p = nextChar();
    checksum -= p+1;
    if (p > 8) throw std::runtime_error("ROM excessiva demandada.");
    romBanks = 2 << p;
    //RAM Size $0149
    p = nextChar();
    checksum -= p+1;
    if (ramBanks != 0) {
        switch (p) {
            case 0:
            ramBanks = 0;
            break;
            case 2:
            ramBanks = 1;
            break;
            case 3:
            ramBanks = 4;
            break;
            case 4:
            ramBanks = 16;
            break;
            case 5:
            ramBanks = 8;
            break;
            default:
            throw std::runtime_error("RAM desconhecida.");
        }
    }
    //Destination $014A
    p = nextChar();
    checksum -= p+1;
    overseasOnly = (p == 1);
    //Old licensee code $014B
    oldLicensee = nextChar();
    checksum -= oldLicensee+1;
    //Rom version $014C
    version = nextChar();
    checksum -= version+1;
    //Header checksum $014D
    p = nextChar();
    if (checksum != p) throw std::runtime_error("Checksum não bate.");
    //Global checksum $014E-$014F
    for (int i = 0; i < 2; i++) {
        globalChecksum[i] = nextChar();
    } 
    return;
};