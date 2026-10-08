#include <unity.h>
#include "menu_navigation.cpp"
#include "menu_actions.cpp"

unsigned long fakeMillisNow = 0;
int fakeDigitalInputs[64] = {};
TimerOneFake Timer1;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

int drawCalls = 0;
int refreshCalls = 0;
int actionCalls = 0;
int resetAlarmCalls = 0;
int modoChanges = 0;

unsigned long millis() { return fakeMillisNow; }
void digitalWrite(int, int) {}
void eepromWrite(int address, bool value) { EEPROM.update(address, value); }
void eepromWrite(int address, uint8_t value) { EEPROM.update(address, value); }
void resetAlarms() { ++resetAlarmCalls; }
void changeModo(bool value) { modoFrio = value; ++modoChanges; }
uint8_t normalizeAcsTemp(volatile uint8_t* value) {
    if (*value < 30) *value = 30;
    if (*value > 48) *value = 48;
    return *value;
}

#define DRAW_STUB(name) void name() { ++drawCalls; }
#define REFRESH_STUB(name) void name() { ++refreshCalls; }
DRAW_STUB(drawHomeScreen)
DRAW_STUB(drawMonitorMenu)
DRAW_STUB(drawMonitorScreen1)
DRAW_STUB(drawMonitorScreen2)
DRAW_STUB(drawConfigurationMenu)
DRAW_STUB(drawModeScreen)
DRAW_STUB(drawModeChangingScreen)
DRAW_STUB(drawAcsConfigurationScreen)
DRAW_STUB(drawAcsEditScreen)
DRAW_STUB(drawFlowAlarmScreen)
DRAW_STUB(drawHeatingScreen)
DRAW_STUB(drawAlarmMenu)
DRAW_STUB(drawActiveAlarmScreen)
DRAW_STUB(drawAlarmHistoryMenu)
DRAW_STUB(drawAlarmHistoryScreen)
DRAW_STUB(drawAcsEnableScreen)
DRAW_STUB(drawAcsDeltaScreen)
DRAW_STUB(drawAcsElectricScreen)
REFRESH_STUB(refreshHomeScreen)
REFRESH_STUB(refreshMonitorScreen1)
REFRESH_STUB(refreshMonitorScreen2)
REFRESH_STUB(refreshModeScreen)
REFRESH_STUB(refreshAcsEditScreen)
REFRESH_STUB(refreshFlowAlarmScreen)
REFRESH_STUB(refreshHeatingScreen)
REFRESH_STUB(refreshAcsEnableScreen)
REFRESH_STUB(refreshAcsDeltaScreen)
REFRESH_STUB(refreshAcsElectricScreen)
REFRESH_STUB(refreshActiveAlarmScreen)
REFRESH_STUB(refreshAlarmHistoryScreen)

void resetFixtures() {
    menuActual = MENU_HOME;
    acsSetpoint = 45;
    acsSetpointEdit = 45;
    modoFrio = false;
    enableFlowAlarm = false;
    heatingOff = false;
    enableAcs = true;
    enableAcsDeltaElectrico = true;
    enableElectricAcs = false;
    valorDoBuzzer = HIGH;
    drawCalls = refreshCalls = actionCalls = resetAlarmCalls = modoChanges = 0;
    EEPROM.update_count = 0;
}

void test_navigation_uses_declared_routes(void) {
    resetFixtures();
    navigateTo(MENU_MONITOR);
    TEST_ASSERT_EQUAL(MENU_MONITOR, menuActual);
    processMenuButton(BUTTON_DOWN);
    TEST_ASSERT_EQUAL(MENU_CONFIGURATION, menuActual);
    processMenuButton(BUTTON_BACK);
    TEST_ASSERT_EQUAL(MENU_HOME, menuActual);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_EQUAL(MENU_MONITOR, menuActual);
    TEST_ASSERT_TRUE(drawCalls >= 4);
}

void test_monitor_routes_all_navigation_buttons(void) {
    resetFixtures();
    navigateTo(MENU_MONITOR);
    processMenuButton(BUTTON_UP);
    TEST_ASSERT_EQUAL(MENU_ALARM_LOG, menuActual);
    processMenuButton(BUTTON_DOWN);
    TEST_ASSERT_EQUAL(MENU_MONITOR, menuActual);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_EQUAL(MENU_MONITOR_VALUES_1, menuActual);
    processMenuButton(BUTTON_BACK);
    TEST_ASSERT_EQUAL(MENU_MONITOR, menuActual);
}

void test_actions_run_only_when_button_has_no_navigation_target(void) {
    resetFixtures();
    acsSetpoint = 45;
    acsSetpointEdit = acsSetpoint;
    navigateTo(MENU_ACS_EDIT);
    processMenuButton(BUTTON_UP);
    TEST_ASSERT_EQUAL_UINT8(46, acsSetpointEdit);
    TEST_ASSERT_EQUAL_UINT8(45, acsSetpoint);
    processMenuButton(BUTTON_DOWN);
    TEST_ASSERT_EQUAL_UINT8(45, acsSetpointEdit);
    processMenuButton(BUTTON_BACK);
    TEST_ASSERT_EQUAL(MENU_ACS_CONFIG, menuActual);
    TEST_ASSERT_EQUAL_UINT8(45, acsSetpoint);

    resetFixtures();
    acsSetpoint = 45;
    acsSetpointEdit = acsSetpoint;
    navigateTo(MENU_ACS_EDIT);
    processMenuButton(BUTTON_UP);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_EQUAL_UINT8(46, acsSetpoint);
    TEST_ASSERT_EQUAL_UINT8(46, acsSetpointEdit);
    TEST_ASSERT_EQUAL_UINT8(46, EEPROM.memory[acsSetpointAddress]);

    resetFixtures();
    navigateTo(MENU_FLOW_ALARMS);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_TRUE(enableFlowAlarm);
    TEST_ASSERT_EQUAL(1, EEPROM.update_count);
    TEST_ASSERT_EQUAL(MENU_FLOW_ALARMS, menuActual);
}

void test_back_cancels_setpoint_edit(void) {
    resetFixtures();
    acsSetpoint = 45;
    acsSetpointEdit = acsSetpoint;
    navigateTo(MENU_ACS_EDIT);
    processMenuButton(BUTTON_UP);
    processMenuButton(BUTTON_BACK);

    TEST_ASSERT_EQUAL_UINT8(45, acsSetpoint);
    TEST_ASSERT_EQUAL_UINT8(45, acsSetpointEdit);
}

void test_configuration_actions_persist_each_existing_flag(void) {
    resetFixtures();
    navigateTo(MENU_MODE);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_TRUE(modoFrio);
    TEST_ASSERT_EQUAL(1, modoChanges);

    navigateTo(MENU_HEATING);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_TRUE(heatingOff);
    navigateTo(MENU_ACS_ENABLE);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_FALSE(enableAcs);
    navigateTo(MENU_ACS_DELTA);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_FALSE(enableAcsDeltaElectrico);
    navigateTo(MENU_ACS_ELECTRIC);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_TRUE(enableElectricAcs);
    TEST_ASSERT_EQUAL(4, EEPROM.update_count);
}

void test_alarm_actions_and_history_action_are_reachable(void) {
    resetFixtures();
    navigateTo(MENU_ALARM_ACTIVE);
    processMenuButton(BUTTON_UP);
    TEST_ASSERT_EQUAL(LOW, valorDoBuzzer);
    valorDoBuzzer = HIGH;
    processMenuButton(BUTTON_DOWN);
    TEST_ASSERT_EQUAL(LOW, valorDoBuzzer);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_EQUAL(1, resetAlarmCalls);

    navigateTo(MENU_ALARM_LOG_VIEW);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_EQUAL_UINT8(0, EEPROM.memory[alarmaAddress]);
}

void test_refresh_calls_current_menu_refresh_only_when_defined(void) {
    resetFixtures();
    navigateTo(MENU_MONITOR);
    refreshCurrentMenu();
    TEST_ASSERT_EQUAL(0, refreshCalls);
    navigateTo(MENU_MONITOR_VALUES_1);
    refreshCurrentMenu();
    TEST_ASSERT_EQUAL(1, refreshCalls);
}

void test_setpoint_actions_clamp_at_both_limits(void) {
    resetFixtures();
    navigateTo(MENU_ACS_EDIT);
    acsSetpointEdit = 30;
    processMenuButton(BUTTON_DOWN);
    TEST_ASSERT_EQUAL_UINT8(30, acsSetpointEdit);
    acsSetpointEdit = 48;
    processMenuButton(BUTTON_UP);
    TEST_ASSERT_EQUAL_UINT8(48, acsSetpointEdit);
}

void test_invalid_menu_is_ignored(void) {
    resetFixtures();
    menuActual = MENU_HOME;
    navigateTo(static_cast<MenuId>(99));
    TEST_ASSERT_EQUAL(MENU_HOME, menuActual);
    TEST_ASSERT_EQUAL(0, drawCalls);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_navigation_uses_declared_routes);
    RUN_TEST(test_monitor_routes_all_navigation_buttons);
    RUN_TEST(test_actions_run_only_when_button_has_no_navigation_target);
    RUN_TEST(test_back_cancels_setpoint_edit);
    RUN_TEST(test_configuration_actions_persist_each_existing_flag);
    RUN_TEST(test_alarm_actions_and_history_action_are_reachable);
    RUN_TEST(test_refresh_calls_current_menu_refresh_only_when_defined);
    RUN_TEST(test_setpoint_actions_clamp_at_both_limits);
    RUN_TEST(test_invalid_menu_is_ignored);
    return UNITY_END();
}
