#include <unity.h>
#include "functionsLCDMenu.cpp"

unsigned long fake_millis_now = 0;
int fake_lcd_begin_calls = 0;
int fake_lcd_clear_calls = 0;
int fake_lcd_cursor_calls = 0;
int fake_lcd_print_calls = 0;
int fake_lcd_write_calls = 0;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

unsigned long millis() { return fake_millis_now; }
uint8_t EEPROMreaduint8_t(int address) { return EEPROM.read(address); }
void refreshCurrentMenu() {}

void resetFixtures() {
    fake_lcd_begin_calls = 0;
    fake_lcd_clear_calls = 0;
    fake_lcd_cursor_calls = 0;
    fake_lcd_print_calls = 0;
    fake_lcd_write_calls = 0;
    Alarma_Eeprom = 0;
}

void test_all_draw_functions_initialize_the_display(void) {
    resetFixtures();
    drawHomeScreen();
    drawMonitorMenu();
    drawMonitorScreen1();
    drawMonitorScreen2();
    drawConfigurationMenu();
    drawModeScreen();
    drawModeChangingScreen();
    drawAcsConfigurationScreen();
    drawAcsEditScreen();
    drawFlowAlarmScreen();
    drawHeatingScreen();
    drawAcsEnableScreen();
    drawAcsDeltaScreen();
    drawAcsElectricScreen();
    drawAlarmMenu();
    drawActiveAlarmScreen();
    drawAlarmHistoryMenu();
    drawAlarmHistoryScreen();

    TEST_ASSERT_EQUAL(18, fake_lcd_begin_calls);
    TEST_ASSERT_EQUAL(18, fake_lcd_clear_calls);
    TEST_ASSERT_TRUE(fake_lcd_print_calls > 18);
}

void test_refresh_functions_cover_alarm_codes_and_history(void) {
    resetFixtures();
    const uint8_t alarmCodes[] = {0, 6, 7, 8, 9, 10, 15, 18};
    for (uint8_t code : alarmCodes) {
        refreshAlarmMessage(code);
    }
    TEST_ASSERT_TRUE(fake_lcd_print_calls >= 8);

    EEPROM.memory[Alarma_Address] = 18;
    refreshAlarmHistoryScreen();
    TEST_ASSERT_EQUAL_UINT8(18, Alarma_Eeprom);
}

void test_special_chars_and_navigation_markers_are_written(void) {
    resetFixtures();
    lcdCreateSpecialChars();
    showNavigation();
    TEST_ASSERT_EQUAL(2, fake_lcd_write_calls);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_all_draw_functions_initialize_the_display);
    RUN_TEST(test_refresh_functions_cover_alarm_codes_and_history);
    RUN_TEST(test_special_chars_and_navigation_markers_are_written);
    return UNITY_END();
}
