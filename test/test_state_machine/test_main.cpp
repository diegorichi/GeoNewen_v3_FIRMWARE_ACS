#include <unity.h>
#include "stateMachine.cpp"

unsigned long fakeMillisNow = 0;
TimerOneFake Timer1;
EEPROMFake EEPROM;
int digitalOutputValues[64] = {};

unsigned long millis() { return fakeMillisNow; }
void digitalWrite(int pin, int value) {
    if (pin >= 0 && pin < 64) digitalOutputValues[pin] = value;
}

#include "../fakes/mega_globals.h"

bool fakeAlarmActive = false;
int fakeBuzzerBips = 0;
int fakeRestChecks = 0;

void checkFlagsForAlarms() { if (fakeAlarmActive) estadoMaquina = 4; }
void ConvertFlagToAlarm() { if (nroAlarma == 0) nroAlarma = 1; }
void eepromWrite(int, bool) {}
void eepromWrite(int, uint8_t) {}
void buzzerBip() { fakeBuzzerBips++; }
void buzzerStop() { flagBuzzer = false; }
void takeRestControl() { fakeRestChecks++; }

void resetFixtures() {
    fakeMillisNow = 0;
    fakeAlarmActive = false;
    fakeBuzzerBips = 0;
    fakeRestChecks = 0;
    Timer1 = TimerOneFake{};
    for (int& value : digitalOutputValues) value = 0;
    estadoMaquina = 0;
    heatingOff = false;
    modoFrio = false;
    senalStart = false;
    senalStop = false;
    enableAcs = true;
    tempAcs = 0;
    tempOutH = 0;
    tempDescarga = 0;
    acsSetpoint = 45;
    nroAlarma = 0;
    alarmaActiva = false;
    flagBuzzer = false;
    valorDoCompressor = LOW;
    valorDoBombas = LOW;
}

void test_initial_state_is_safe(void) {
    resetFixtures();
    initializeStateMachine();
    stateMachine0();
    TEST_ASSERT_EQUAL(1, estadoMaquina);
    TEST_ASSERT_EQUAL(LOW, valorDoCompressor);
    TEST_ASSERT_EQUAL(LOW, valorDoBombas);
    TEST_ASSERT_EQUAL(LOW, valorDoVacs);
    TEST_ASSERT_EQUAL(HIGH, valorDoV4v);
}

void test_start_reaches_pump_state_after_existing_delay(void) {
    resetFixtures();
    estadoMaquina = 1;
    senalStart = true;
    enableAcs = false;
    stateMachine1();
    TEST_ASSERT_EQUAL(1, estadoMaquina);
    fakeMillisNow = 60001;
    stateMachine1();
    TEST_ASSERT_EQUAL(2, estadoMaquina);
}

void test_pumps_then_compressor_then_running_state(void) {
    resetFixtures();
    estadoMaquina = 2;
    stateMachine2();
    TEST_ASSERT_EQUAL(HIGH, valorDoBombas);
    TEST_ASSERT_EQUAL(LOW, valorDoCompressor);
    fakeMillisNow = 25001;
    stateMachine2();
    TEST_ASSERT_EQUAL(HIGH, valorDoCompressor);
    fakeMillisNow = 40002;
    stateMachine2();
    TEST_ASSERT_EQUAL(3, estadoMaquina);
}

void test_stop_returns_to_safe_initial_state(void) {
    resetFixtures();
    estadoMaquina = 2;
    valorDoCompressor = HIGH;
    valorDoBombas = HIGH;
    senalStop = true;
    stateMachine2();
    TEST_ASSERT_EQUAL(0, estadoMaquina);
}

void test_alarm_has_priority_in_startup(void) {
    resetFixtures();
    estadoMaquina = 2;
    fakeAlarmActive = true;
    stateMachine2();
    TEST_ASSERT_EQUAL(4, estadoMaquina);
    stateMachine4();
    TEST_ASSERT_EQUAL(LOW, valorDoCompressor);
    TEST_ASSERT_EQUAL(LOW, valorDoBombas);
    TEST_ASSERT_TRUE(alarmaActiva);
}

void test_acs_generation_stops_at_setpoint(void) {
    resetFixtures();
    estadoMaquina = 7;
    tempAcs = 45;
    stateMachine7();
    TEST_ASSERT_EQUAL(0, estadoMaquina);
}

void test_initial_state_respects_heating_off_and_cold_mode(void) {
    resetFixtures();
    initializeStateMachine();
    heatingOff = true;
    modoFrio = true;
    stateMachine0();

    TEST_ASSERT_EQUAL(0, estadoMaquina);
    TEST_ASSERT_EQUAL(LOW, valorDoV4v);
}

void test_state_one_heating_off_returns_to_initial_state(void) {
    resetFixtures();
    estadoMaquina = 1;
    heatingOff = true;

    stateMachine1();

    TEST_ASSERT_EQUAL(0, estadoMaquina);
}

void test_state_one_starts_acs_generation_after_valve_delay(void) {
    resetFixtures();
    estadoMaquina = 1;
    enableAcs = true;
    tempAcs = 30;
    acsSetpoint = 45;
    valvulaAcsStart = 0;
    fakeMillisNow = 15001;

    stateMachine1();

    TEST_ASSERT_EQUAL(7, estadoMaquina);
    TEST_ASSERT_EQUAL(HIGH, valorDoVacs);
    TEST_ASSERT_EQUAL(HIGH, valorDoV4v);
    TEST_ASSERT_EQUAL(15001, ingresoE7);
}

void test_state_one_daily_pump_routine_runs_and_stops(void) {
    resetFixtures();
    estadoMaquina = 1;
    dontStuckPumpsStart = 0;
    enableAcs = false;
    valvulaAcsStart = fakeMillisNow;
    fakeMillisNow = 86400001UL;

    stateMachine1();

    TEST_ASSERT_EQUAL(HIGH, valorDoBombas);
    TEST_ASSERT_EQUAL(1, fakeBuzzerBips);
    fakeMillisNow = 86410002UL;
    stateMachine1();
    TEST_ASSERT_EQUAL(LOW, valorDoBombas);
}

void test_state_two_stop_and_heating_off_are_safe(void) {
    resetFixtures();
    estadoMaquina = 2;
    senalStop = true;
    stateMachine2();
    TEST_ASSERT_EQUAL(0, estadoMaquina);

    resetFixtures();
    estadoMaquina = 2;
    heatingOff = true;
    stateMachine2();
    TEST_ASSERT_EQUAL(0, estadoMaquina);
}

void test_state_three_handles_alarm_before_stop_and_rest(void) {
    resetFixtures();
    estadoMaquina = 3;
    fakeAlarmActive = true;
    senalStop = true;
    stateMachine3();
    TEST_ASSERT_EQUAL(4, estadoMaquina);

    resetFixtures();
    estadoMaquina = 3;
    senalStop = true;
    stateMachine3();
    TEST_ASSERT_EQUAL(0, estadoMaquina);

    resetFixtures();
    estadoMaquina = 3;
    enableAcs = false;
    stateMachine3();
    TEST_ASSERT_EQUAL(1, fakeRestChecks);
}

void test_alarm_state_stops_outputs_and_starts_buzzer_once(void) {
    resetFixtures();
    estadoMaquina = 4;
    valorDoBombas = HIGH;
    valorDoCompressor = HIGH;
    nroAlarma = 6;

    stateMachine4();
    stateMachine4();

    TEST_ASSERT_EQUAL(LOW, valorDoBombas);
    TEST_ASSERT_EQUAL(LOW, valorDoCompressor);
    TEST_ASSERT_TRUE(flagBuzzer);
    TEST_ASSERT_EQUAL(100, Timer1.lastValue);
}

void test_rest_state_exits_after_time_or_heating_off(void) {
    resetFixtures();
    estadoMaquina = 6;
    ingresoDescanso = 0;
    fakeMillisNow = 400001;
    stateMachine6();
    TEST_ASSERT_EQUAL(0, estadoMaquina);

    resetFixtures();
    estadoMaquina = 6;
    heatingOff = true;
    stateMachine6();
    TEST_ASSERT_EQUAL(0, estadoMaquina);
}

void test_state_seven_starts_pumps_and_compressor_after_delays(void) {
    resetFixtures();
    estadoMaquina = 7;
    enableAcs = true;
    tempAcs = 30;
    acsSetpoint = 45;
    valvulaAcsStart = 0;
    ingresoE7 = 0;
    fakeMillisNow = 15001;
    stateMachine7();
    TEST_ASSERT_EQUAL(HIGH, valorDoBombas);
    TEST_ASSERT_EQUAL(LOW, valorDoCompressor);

    fakeMillisNow = 20001;
    stateMachine7();
    TEST_ASSERT_EQUAL(HIGH, valorDoCompressor);
}

void test_state_seven_stops_for_disable_target_or_heating_off(void) {
    resetFixtures();
    estadoMaquina = 7;
    enableAcs = false;
    stateMachine7();
    TEST_ASSERT_EQUAL(0, estadoMaquina);

    resetFixtures();
    estadoMaquina = 7;
    tempAcs = 45;
    acsSetpoint = 45;
    stateMachine7();
    TEST_ASSERT_EQUAL(0, estadoMaquina);

    resetFixtures();
    estadoMaquina = 7;
    heatingOff = true;
    stateMachine7();
    TEST_ASSERT_EQUAL(0, estadoMaquina);
}

void test_state_seven_enters_71_for_temperature_safety_or_alarm(void) {
    resetFixtures();
    estadoMaquina = 7;
    tempOutH = 50.1;
    stateMachine7();
    TEST_ASSERT_EQUAL(71, estadoMaquina);
    TEST_ASSERT_EQUAL(0, ingresoE71);

    resetFixtures();
    estadoMaquina = 7;
    tempOutH = 0;
    tempDescarga = 80.1;
    stateMachine7();
    TEST_ASSERT_EQUAL(71, estadoMaquina);

    resetFixtures();
    estadoMaquina = 7;
    fakeAlarmActive = true;
    fakeMillisNow = 20001;
    stateMachine7();
    TEST_ASSERT_EQUAL(4, estadoMaquina);
}

void test_state_71_returns_to_generation_for_each_release_condition(void) {
    resetFixtures();
    estadoMaquina = 71;
    tempAcs = 46;
    acsSetpoint = 45;
    stateMachine71();
    TEST_ASSERT_EQUAL(7, estadoMaquina);

    resetFixtures();
    estadoMaquina = 71;
    tempAcs = 40;
    tempOutH = 50;
    acsSetpoint = 45;
    TEST_ASSERT_EQUAL_FLOAT(40.0, tempAcs);
    TEST_ASSERT_EQUAL_FLOAT(50.0, tempOutH);
    TEST_ASSERT_EQUAL_UINT8(45, acsSetpoint);
    stateMachine71();
    TEST_ASSERT_EQUAL(71, estadoMaquina);

    resetFixtures();
    estadoMaquina = 71;
    ingresoE71 = 0;
    tempAcs = 40;
    tempOutH = 50;
    fakeMillisNow = 90001;
    stateMachine71();
    TEST_ASSERT_EQUAL(7, estadoMaquina);

    resetFixtures();
    estadoMaquina = 71;
    enableAcs = false;
    stateMachine71();
    TEST_ASSERT_EQUAL(7, estadoMaquina);
}

void test_state_71_heating_off_returns_to_initial_state(void) {
    resetFixtures();
    estadoMaquina = 71;
    heatingOff = true;

    stateMachine71();

    TEST_ASSERT_EQUAL(0, estadoMaquina);
    TEST_ASSERT_EQUAL(LOW, valorDoCompressor);
}

void test_state_transitions_require_strict_time_boundaries(void) {
    resetFixtures();
    estadoMaquina = 1;
    senalStart = true;
    enableAcs = false;
    stateMachine1();
    fakeMillisNow = 60000;
    stateMachine1();
    TEST_ASSERT_EQUAL(1, estadoMaquina);
    fakeMillisNow = 60001;
    stateMachine1();
    TEST_ASSERT_EQUAL(2, estadoMaquina);

    resetFixtures();
    estadoMaquina = 7;
    valvulaAcsStart = 0;
    tempAcs = 30;
    acsSetpoint = 45;
    fakeMillisNow = 15000;
    stateMachine7();
    TEST_ASSERT_EQUAL(LOW, valorDoBombas);
    fakeMillisNow = 15001;
    stateMachine7();
    TEST_ASSERT_EQUAL(HIGH, valorDoBombas);
}

void test_state_71_does_not_return_before_timeout(void) {
    resetFixtures();
    estadoMaquina = 71;
    ingresoE71 = 0;
    tempAcs = 40;
    tempOutH = 50;
    fakeMillisNow = 90000;
    stateMachine71();
    TEST_ASSERT_EQUAL(71, estadoMaquina);
    fakeMillisNow = 90001;
    stateMachine71();
    TEST_ASSERT_EQUAL(7, estadoMaquina);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_initial_state_is_safe);
    RUN_TEST(test_start_reaches_pump_state_after_existing_delay);
    RUN_TEST(test_pumps_then_compressor_then_running_state);
    RUN_TEST(test_stop_returns_to_safe_initial_state);
    RUN_TEST(test_alarm_has_priority_in_startup);
    RUN_TEST(test_acs_generation_stops_at_setpoint);
    RUN_TEST(test_initial_state_respects_heating_off_and_cold_mode);
    RUN_TEST(test_state_one_heating_off_returns_to_initial_state);
    RUN_TEST(test_state_one_starts_acs_generation_after_valve_delay);
    RUN_TEST(test_state_one_daily_pump_routine_runs_and_stops);
    RUN_TEST(test_state_two_stop_and_heating_off_are_safe);
    RUN_TEST(test_state_three_handles_alarm_before_stop_and_rest);
    RUN_TEST(test_alarm_state_stops_outputs_and_starts_buzzer_once);
    RUN_TEST(test_rest_state_exits_after_time_or_heating_off);
    RUN_TEST(test_state_seven_starts_pumps_and_compressor_after_delays);
    RUN_TEST(test_state_seven_stops_for_disable_target_or_heating_off);
    RUN_TEST(test_state_seven_enters_71_for_temperature_safety_or_alarm);
    RUN_TEST(test_state_71_returns_to_generation_for_each_release_condition);
    RUN_TEST(test_state_71_heating_off_returns_to_initial_state);
    RUN_TEST(test_state_transitions_require_strict_time_boundaries);
    RUN_TEST(test_state_71_does_not_return_before_timeout);
    UNITY_END();
    return 0;
}
