#include <unity.h>
#include "menu_navigation.cpp"
#include "menu_actions.cpp"

unsigned long fake_millis_now = 0;
int fake_digital_inputs[64] = {};
TimerOneFake Timer1;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

int draw_calls = 0;
int refresh_calls = 0;
int action_calls = 0;
int reset_alarm_calls = 0;
int modo_changes = 0;

unsigned long millis() { return fake_millis_now; }
void digitalWrite(int, int) {}
void EEPROMwrite(int address, bool value) { EEPROM.update(address, value); }
void EEPROMwrite(int address, uint8_t value) { EEPROM.update(address, value); }
void resetAlarms() { ++reset_alarm_calls; }
void changeModo(bool value) { modoFrio = value; ++modo_changes; }
uint8_t normalizeAcsTemp(volatile uint8_t* value) {
    if (*value < 30) *value = 30;
    if (*value > 48) *value = 48;
    return *value;
}

#define DRAW_STUB(name) void name() { ++draw_calls; }
#define REFRESH_STUB(name) void name() { ++refresh_calls; }
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
    MenuActual = MENU_HOME;
    SetP_ACS = 45;
    SetP_ACS_Edit = 45;
    modoFrio = false;
    EnableFlowAlarm = false;
    heating_off = false;
    EnableACS = true;
    EnableACS_DeltaElectrico = true;
    EnableElectricACS = false;
    Valor_DO_Buzzer = HIGH;
    draw_calls = refresh_calls = action_calls = reset_alarm_calls = modo_changes = 0;
    EEPROM.update_count = 0;
}

void test_navigation_uses_declared_routes(void) {
    resetFixtures();
    navigateTo(MENU_MONITOR);
    TEST_ASSERT_EQUAL(MENU_MONITOR, MenuActual);
    processMenuButton(BUTTON_DOWN);
    TEST_ASSERT_EQUAL(MENU_CONFIGURATION, MenuActual);
    processMenuButton(BUTTON_BACK);
    TEST_ASSERT_EQUAL(MENU_HOME, MenuActual);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_EQUAL(MENU_MONITOR, MenuActual);
    TEST_ASSERT_TRUE(draw_calls >= 4);
}

void test_monitor_routes_all_navigation_buttons(void) {
    resetFixtures();
    navigateTo(MENU_MONITOR);
    processMenuButton(BUTTON_UP);
    TEST_ASSERT_EQUAL(MENU_ALARM_LOG, MenuActual);
    processMenuButton(BUTTON_DOWN);
    TEST_ASSERT_EQUAL(MENU_MONITOR, MenuActual);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_EQUAL(MENU_MONITOR_VALUES_1, MenuActual);
    processMenuButton(BUTTON_BACK);
    TEST_ASSERT_EQUAL(MENU_MONITOR, MenuActual);
}

void test_actions_run_only_when_button_has_no_navigation_target(void) {
    resetFixtures();
    SetP_ACS = 45;
    SetP_ACS_Edit = SetP_ACS;
    navigateTo(MENU_ACS_EDIT);
    processMenuButton(BUTTON_UP);
    TEST_ASSERT_EQUAL_UINT8(46, SetP_ACS_Edit);
    TEST_ASSERT_EQUAL_UINT8(45, SetP_ACS);
    processMenuButton(BUTTON_DOWN);
    TEST_ASSERT_EQUAL_UINT8(45, SetP_ACS_Edit);
    processMenuButton(BUTTON_BACK);
    TEST_ASSERT_EQUAL(MENU_ACS_CONFIG, MenuActual);
    TEST_ASSERT_EQUAL_UINT8(45, SetP_ACS);

    resetFixtures();
    SetP_ACS = 45;
    SetP_ACS_Edit = SetP_ACS;
    navigateTo(MENU_ACS_EDIT);
    processMenuButton(BUTTON_UP);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_EQUAL_UINT8(46, SetP_ACS);
    TEST_ASSERT_EQUAL_UINT8(46, SetP_ACS_Edit);
    TEST_ASSERT_EQUAL_UINT8(46, EEPROM.memory[SetP_ACS_Address]);

    resetFixtures();
    navigateTo(MENU_FLOW_ALARMS);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_TRUE(EnableFlowAlarm);
    TEST_ASSERT_EQUAL(1, EEPROM.update_count);
    TEST_ASSERT_EQUAL(MENU_FLOW_ALARMS, MenuActual);
}

void test_back_cancels_setpoint_edit(void) {
    resetFixtures();
    SetP_ACS = 45;
    SetP_ACS_Edit = SetP_ACS;
    navigateTo(MENU_ACS_EDIT);
    processMenuButton(BUTTON_UP);
    processMenuButton(BUTTON_BACK);

    TEST_ASSERT_EQUAL_UINT8(45, SetP_ACS);
    TEST_ASSERT_EQUAL_UINT8(45, SetP_ACS_Edit);
}

void test_configuration_actions_persist_each_existing_flag(void) {
    resetFixtures();
    navigateTo(MENU_MODE);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_TRUE(modoFrio);
    TEST_ASSERT_EQUAL(1, modo_changes);

    navigateTo(MENU_HEATING);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_TRUE(heating_off);
    navigateTo(MENU_ACS_ENABLE);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_FALSE(EnableACS);
    navigateTo(MENU_ACS_DELTA);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_FALSE(EnableACS_DeltaElectrico);
    navigateTo(MENU_ACS_ELECTRIC);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_TRUE(EnableElectricACS);
    TEST_ASSERT_EQUAL(4, EEPROM.update_count);
}

void test_alarm_actions_and_history_action_are_reachable(void) {
    resetFixtures();
    navigateTo(MENU_ALARM_ACTIVE);
    processMenuButton(BUTTON_UP);
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Buzzer);
    Valor_DO_Buzzer = HIGH;
    processMenuButton(BUTTON_DOWN);
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Buzzer);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_EQUAL(1, reset_alarm_calls);

    navigateTo(MENU_ALARM_LOG_VIEW);
    processMenuButton(BUTTON_ENTER);
    TEST_ASSERT_EQUAL_UINT8(0, EEPROM.memory[Alarma_Address]);
}

void test_refresh_calls_current_menu_refresh_only_when_defined(void) {
    resetFixtures();
    navigateTo(MENU_MONITOR);
    refreshCurrentMenu();
    TEST_ASSERT_EQUAL(0, refresh_calls);
    navigateTo(MENU_MONITOR_VALUES_1);
    refreshCurrentMenu();
    TEST_ASSERT_EQUAL(1, refresh_calls);
}

void test_setpoint_actions_clamp_at_both_limits(void) {
    resetFixtures();
    navigateTo(MENU_ACS_EDIT);
    SetP_ACS_Edit = 30;
    processMenuButton(BUTTON_DOWN);
    TEST_ASSERT_EQUAL_UINT8(30, SetP_ACS_Edit);
    SetP_ACS_Edit = 48;
    processMenuButton(BUTTON_UP);
    TEST_ASSERT_EQUAL_UINT8(48, SetP_ACS_Edit);
}

void test_invalid_menu_is_ignored(void) {
    resetFixtures();
    MenuActual = MENU_HOME;
    navigateTo(static_cast<MenuId>(99));
    TEST_ASSERT_EQUAL(MENU_HOME, MenuActual);
    TEST_ASSERT_EQUAL(0, draw_calls);
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
