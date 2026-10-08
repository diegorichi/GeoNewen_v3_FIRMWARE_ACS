#pragma once
#include <stdint.h>
extern int fakeLcdBeginCalls;
extern int fakeLcdClearCalls;
extern int fakeLcdCursorCalls;
extern int fakeLcdPrintCalls;
extern int fakeLcdWriteCalls;

class LiquidCrystal {
public:
    LiquidCrystal(int, int, int, int, int, int, int, int, int, int) {}
    void begin(int, int) { ++fakeLcdBeginCalls; }
    void clear() { ++fakeLcdClearCalls; }
    void createChar(uint8_t, uint8_t*) {}
    void setCursor(int, int) { ++fakeLcdCursorCalls; }
    void write(uint8_t) { ++fakeLcdWriteCalls; }
    void print(const char*) { ++fakeLcdPrintCalls; }
    void print(int) { ++fakeLcdPrintCalls; }
    void print(float, int) { ++fakeLcdPrintCalls; }
};
