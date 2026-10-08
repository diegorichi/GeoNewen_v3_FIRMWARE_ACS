#pragma once
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define FALLING 2
#define F(value) value
#define B00000 0
#define B00100 4
#define B01110 14
#define B11111 31
extern unsigned long fakeMillisNow;
extern int fakeDigitalInputs[64];
class String {
public:
    char value[128] = {};
    String() {}
    String(const char* text) { strncpy(value, text ? text : "", sizeof(value) - 1); }
    bool startsWith(const char* prefix) const { return strncmp(value, prefix, strlen(prefix)) == 0; }
    int indexOf(char character, int from = 0) const {
        const char* position = strchr(value + from, character);
        return position == nullptr ? -1 : static_cast<int>(position - value);
    }
    int indexOf(const char* text) const {
        const char* position = strstr(value, text ? text : "");
        return position == nullptr ? -1 : static_cast<int>(position - value);
    }
    String substring(int start) const { return String(value + start); }
    String substring(int start, int end) const {
        String result;
        int length = end - start;
        if (length > static_cast<int>(sizeof(result.value) - 1)) length = sizeof(result.value) - 1;
        memcpy(result.value, value + start, length);
        result.value[length] = '\0';
        return result;
    }
    int toInt() const { return atoi(value); }
    void toCharArray(char* target, unsigned int length) const {
        if (length == 0) return;
        strncpy(target, value, length - 1);
        target[length - 1] = '\0';
    }
    operator const char*() const { return value; }
    const char* c_str() const { return value; }
};
inline bool isAlphaNumeric(char value) { return isalnum(static_cast<unsigned char>(value)) != 0; }
inline bool isDigit(char value) { return isdigit(static_cast<unsigned char>(value)) != 0; }
inline char* dtostrf(double value, signed char, unsigned char precision, char* buffer) {
    snprintf(buffer, 32, "%.*f", precision, value);
    return buffer;
}
class HardwareSerial {
public:
    char input[2048] = {};
    int inputLength = 0;
    char output[8192] = {};
    int outputLength = 0;
    void begin(unsigned long) {}
    void setTimeout(unsigned long) {}
    int available() const { return inputLength; }
    char read() { char result = input[0]; memmove(input, input + 1, inputLength); --inputLength; return result; }
    void print(const char* text) { if (text) { int n = strlen(text); memcpy(output + outputLength, text, n); outputLength += n; output[outputLength] = '\0'; } }
    void print(const String& text) { print(text.value); }
    void print(char value) { output[outputLength++] = value; output[outputLength] = '\0'; }
    void print(int value) { char buffer[24]; snprintf(buffer, sizeof(buffer), "%d", value); print(buffer); }
    void println(const char* text) { print(text); print("\n"); }
    void println(const String& text) { print(text); print("\n"); }
    void println(int value) { print(value); print("\n"); }
};
extern HardwareSerial Serial;
unsigned long millis();
void digitalWrite(int pin, int value);
int digitalRead(int pin);
void pinMode(int pin, int mode);
void tone(int pin, unsigned int frequency, unsigned long duration);
void noTone(int pin);
void noInterrupts();
void interrupts();
void attachInterrupt(int interruptNumber, void (*handler)(), int mode);
void detachInterrupt(int interruptNumber);
