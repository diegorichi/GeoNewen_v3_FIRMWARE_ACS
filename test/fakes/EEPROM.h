#pragma once
#include <stdint.h>
class EEPROMFake {
public:
    uint8_t memory[64] = {};
    int update_count = 0;
    uint8_t read(int address) const { return memory[address]; }
    void update(int address, uint8_t value) {
        memory[address] = value;
        ++update_count;
    }
};
extern EEPROMFake EEPROM;
