#include <unity.h>
#include "functionsLCDMenu.cpp"

unsigned long fakeMillisNow = 0;
int fakeLcdBeginCalls = 0;
int fakeLcdClearCalls = 0;
int fakeLcdCursorCalls = 0;
int fakeLcdPrintCalls = 0;
int fakeLcdWriteCalls = 0;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

unsigned long millis() { return fakeMillisNow; }
uint8_t eepromReadUint8(int address) { return EEPROM.read(address); }
void refreshCurrentMenu() {}

void resetFixtures() {
    fakeLcdBeginCalls = 0;
    fakeLcdClearCalls = 0;
    fakeLcdCursorCalls = 0;
    fakeLcdPrintCalls = 0;
    fakeLcdWriteCalls = 0;
    alarmaEeprom = 0;
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

    TEST_ASSERT_EQUAL(18, fakeLcdBeginCalls);
    TEST_ASSERT_EQUAL(18, fakeLcdClearCalls);
    TEST_ASSERT_TRUE(fakeLcdPrintCalls > 18);
}

void test_refresh_functions_cover_alarm_codes_and_history(void) {
    resetFixtures();
    const uint8_t alarmCodes[] = {0, 6, 7, 8, 9, 10, 15, 18};
    for (uint8_t code : alarmCodes) {
        refreshAlarmMessage(code);
    }
    TEST_ASSERT_TRUE(fakeLcdPrintCalls >= 8);

    EEPROM.memory[alarmaAddress] = 18;
    refreshAlarmHistoryScreen();
    TEST_ASSERT_EQUAL_UINT8(18, alarmaEeprom);
}

void test_special_chars_and_navigation_markers_are_written(void) {
    resetFixtures();
    lcdCreateSpecialChars();
    showNavigation();
    TEST_ASSERT_EQUAL(2, fakeLcdWriteCalls);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_all_draw_functions_initialize_the_display);
    RUN_TEST(test_refresh_functions_cover_alarm_codes_and_history);
    RUN_TEST(test_special_chars_and_navigation_markers_are_written);
    return UNITY_END();
}
