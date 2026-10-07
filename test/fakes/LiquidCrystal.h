#pragma once
#include <stdint.h>
extern int fake_lcd_begin_calls;
extern int fake_lcd_clear_calls;
extern int fake_lcd_cursor_calls;
extern int fake_lcd_print_calls;
extern int fake_lcd_write_calls;

class LiquidCrystal {
public:
    LiquidCrystal(int, int, int, int, int, int, int, int, int, int) {}
    void begin(int, int) { ++fake_lcd_begin_calls; }
    void clear() { ++fake_lcd_clear_calls; }
    void createChar(uint8_t, uint8_t*) {}
    void setCursor(int, int) { ++fake_lcd_cursor_calls; }
    void write(uint8_t) { ++fake_lcd_write_calls; }
    void print(const char*) { ++fake_lcd_print_calls; }
    void print(int) { ++fake_lcd_print_calls; }
    void print(float, int) { ++fake_lcd_print_calls; }
};
