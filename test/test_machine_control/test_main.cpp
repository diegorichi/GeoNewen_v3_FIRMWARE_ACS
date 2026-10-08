#include <unity.h>
#include "machine_control.cpp"

unsigned long fakeMillisNow = 0;
int fakeDigitalInputs[64] = {};
int fakeDigitalOutputs[64] = {};
TimerOneFake Timer1;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

unsigned long millis() { return fakeMillisNow; }
int digitalRead(int pin) { return fakeDigitalInputs[pin]; }
void digitalWrite(int pin, int value) { fakeDigitalOutputs[pin] = value; }
void pinMode(int, int) {}
void tone(int, unsigned int, unsigned long) {}
void noTone(int) {}
void noInterrupts() {}
void interrupts() {}
void attachInterrupt(int, void (*)(), int) {}
void detachInterrupt(int) {}
void eepromWrite(int address, bool value) { EEPROM.update(address, value); }
void eepromWrite(int address, uint8_t value) { EEPROM.update(address, value); }
void temperatureCalculation() {}
void lcdRefreshValues() {}

void resetFixtures() {
    fakeMillisNow = 0;
    for (int& value : fakeDigitalInputs) value = LOW;
    for (int& value : fakeDigitalOutputs) value = LOW;
    estadoMaquina = 1;
    modoFrio = false;
    heatingOff = false;
    tempOutH = tempOutT = tempAdmision = 0;
    ingresoE3 = 0;
    periodoRefresco = 0;
    flagBuzzer = false;
    valorDoBuzzer = LOW;
    EEPROM.update_count = 0;
    lastModeChangeAt = 0;
}

void test_start_stop_signal_follows_mode(void) {
    resetFixtures();
    fakeDigitalInputs[diMarchaOn] = HIGH;
    calculateStartStopSignal();
    TEST_ASSERT_TRUE(senalStart);
    TEST_ASSERT_FALSE(senalStop);

    modoFrio = true;
    calculateStartStopSignal();
    TEST_ASSERT_FALSE(senalStart);
    TEST_ASSERT_TRUE(senalStop);

    fakeDigitalInputs[diMarchaOn] = LOW;
    calculateStartStopSignal();
    TEST_ASSERT_TRUE(senalStart);
    TEST_ASSERT_FALSE(senalStop);
}

void test_normalize_setpoint_clamps_both_limits(void) {
    resetFixtures();
    volatile uint8_t value = 29;
    TEST_ASSERT_EQUAL_UINT8(30, normalizeAcsTemp(&value));
    value = 61;
    TEST_ASSERT_EQUAL_UINT8(48, normalizeAcsTemp(&value));
    value = 40;
    TEST_ASSERT_EQUAL_UINT8(40, normalizeAcsTemp(&value));
}

void test_mode_change_requires_idle_and_respects_lockout(void) {
    resetFixtures();
    modoFrio = false;
    fakeMillisNow = 1;
    changeModo(true);
    TEST_ASSERT_TRUE(modoFrio);
    TEST_ASSERT_EQUAL(1, EEPROM.update_count);
    TEST_ASSERT_TRUE(isModeChangeLocked());

    changeModo(false);
    TEST_ASSERT_TRUE(modoFrio);
    fakeMillisNow = 3001;
    changeModo(false);
    TEST_ASSERT_FALSE(modoFrio);

    estadoMaquina = 3;
    changeModo(true);
    TEST_ASSERT_FALSE(modoFrio);
}

void test_heating_cooling_and_long_period_checks(void) {
    resetFixtures();
    fakeMillisNow = 120001;
    tempOutH = 43;
    TEST_ASSERT_TRUE(heatingCheck());

    modoFrio = true;
    tempOutH = 9;
    TEST_ASSERT_TRUE(coolingCheck());

    fakeMillisNow = 43200001;
    TEST_ASSERT_TRUE(longPeriodRunningCheck());
}

void test_take_rest_control_enters_rest_for_each_condition(void) {
    resetFixtures();
    fakeMillisNow = 120001;
    tempOutH = 43;
    takeRestControl();
    TEST_ASSERT_EQUAL(6, estadoMaquina);
    TEST_ASSERT_EQUAL(120001, ingresoDescanso);

    resetFixtures();
    modoFrio = true;
    tempOutT = 41;
    takeRestControl();
    TEST_ASSERT_EQUAL(6, estadoMaquina);
}

void test_outputs_and_buzzer_follow_existing_control(void) {
    resetFixtures();
    valorDoBombas = HIGH;
    valorDoCalentador = HIGH;
    valorDoV4v = HIGH;
    valorDoCompressor = HIGH;
    valorDoVacs = HIGH;
    writeOutput();
    TEST_ASSERT_EQUAL(HIGH, fakeDigitalOutputs[doBombas]);
    TEST_ASSERT_EQUAL(HIGH, fakeDigitalOutputs[doCompressor]);
    TEST_ASSERT_EQUAL(HIGH, fakeDigitalOutputs[doValvulaAcs]);

    flagBuzzer = true;
    estadoMaquina = 1;
    buzzerControl();
    TEST_ASSERT_FALSE(flagBuzzer);
    estadoMaquina = 4;
    flagBuzzer = true;
    buzzerControl();
    TEST_ASSERT_TRUE(flagBuzzer);
    buzzerStop();
    TEST_ASSERT_FALSE(flagBuzzer);
    TEST_ASSERT_EQUAL(LOW, valorDoBuzzer);
}

void test_output_initialization_turns_every_output_off(void) {
    resetFixtures();
    for (int& value : fakeDigitalOutputs) value = HIGH;
    initializeDigitalOuputs();
    TEST_ASSERT_EQUAL(LOW, fakeDigitalOutputs[doCompressor]);
    TEST_ASSERT_EQUAL(LOW, fakeDigitalOutputs[doValvulaAcs]);
    TEST_ASSERT_EQUAL(LOW, fakeDigitalOutputs[doBombas]);
    TEST_ASSERT_EQUAL(LOW, fakeDigitalOutputs[doCalentador]);
    TEST_ASSERT_EQUAL(LOW, fakeDigitalOutputs[doValvula4Vias]);
    TEST_ASSERT_EQUAL(LOW, fakeDigitalOutputs[doTriac01]);
    TEST_ASSERT_EQUAL(LOW, fakeDigitalOutputs[doBuzzer]);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_start_stop_signal_follows_mode);
    RUN_TEST(test_normalize_setpoint_clamps_both_limits);
    RUN_TEST(test_mode_change_requires_idle_and_respects_lockout);
    RUN_TEST(test_heating_cooling_and_long_period_checks);
    RUN_TEST(test_take_rest_control_enters_rest_for_each_condition);
    RUN_TEST(test_outputs_and_buzzer_follow_existing_control);
    RUN_TEST(test_output_initialization_turns_every_output_off);
    return UNITY_END();
}
