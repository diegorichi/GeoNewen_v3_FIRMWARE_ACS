#include <unity.h>
#include "alarm.cpp"

unsigned long fakeMillisNow = 0;
TimerOneFake Timer1;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

int eepromAlarmWrites = 0;
int buzzerStopCalls = 0;

unsigned long millis() { return fakeMillisNow; }
void digitalWrite(int, int) {}
void eepromWrite(int address, uint8_t number) {
    if (address == alarmaAddress && number != 0) {
        ++eepromAlarmWrites;
    }
}
void eepromWrite(int, bool) {}
void buzzerStop() { ++buzzerStopCalls; }

void resetFixtures() {
    estadoMaquina = 0;
    nroAlarma = 0;
    alarmaActiva = false;
    flagTempCompressor = false;
    flagTempDescarga = false;
    flagCaudT = false;
    flagCaudH = false;
    flagPresHi = false;
    flagPresLow = false;
    flagTempAdm = false;
    contTempCompressor = 4;
    contPressHi = 5;
    contPressLow = 6;
    contTempDes = 7;
    eepromAlarmWrites = 0;
    buzzerStopCalls = 0;
}

void test_alarm_priority_and_persistence(void) {
    resetFixtures();
    flagCaudT = true;
    flagPresHi = true;

    ConvertFlagToAlarm();

    TEST_ASSERT_EQUAL_UINT8(7, nroAlarma);
    TEST_ASSERT_EQUAL(1, eepromAlarmWrites);
}

void test_every_alarm_type_maps_to_existing_code(void) {
    const bool* flags[] = {
        &flagTempCompressor, &flagCaudT, &flagCaudH, &flagPresHi,
        &flagPresLow, &flagTempAdm, &flagTempDescarga};
    const uint8_t expected[] = {6, 7, 8, 9, 10, 15, 18};

    for (unsigned int i = 0; i < 7; ++i) {
        resetFixtures();
        *const_cast<bool*>(flags[i]) = true;
        ConvertFlagToAlarm();
        TEST_ASSERT_EQUAL_UINT8(expected[i], nroAlarma);
        TEST_ASSERT_EQUAL(1, eepromAlarmWrites);
    }
}

void test_alarm_priority_is_stable_when_multiple_flags_are_active(void) {
    resetFixtures();
    flagTempCompressor = true;
    flagCaudT = true;
    flagCaudH = true;
    flagPresHi = true;
    flagPresLow = true;
    flagTempAdm = true;
    flagTempDescarga = true;

    ConvertFlagToAlarm();

    TEST_ASSERT_EQUAL_UINT8(6, nroAlarma);
}

void test_alarm_detection_enters_alarm_state(void) {
    resetFixtures();
    flagPresLow = true;

    checkFlagsForAlarms();

    TEST_ASSERT_EQUAL(4, estadoMaquina);
}

void test_pressure_flags_are_alarm_conditions(void) {
    resetFixtures();
    flagPresHi = true;
    checkFlagsForAlarms();
    TEST_ASSERT_EQUAL(4, estadoMaquina);

    resetFixtures();
    flagPresLow = true;
    checkFlagsForAlarms();
    TEST_ASSERT_EQUAL(4, estadoMaquina);
}

void test_reset_alarm_clears_flags_counters_and_state(void) {
    resetFixtures();
    estadoMaquina = 4;
    flagTempCompressor = true;
    flagCaudH = true;
    flagPresHi = true;
    alarmaActiva = true;
    nroAlarma = 9;

    resetAlarms();

    TEST_ASSERT_FALSE(flagTempCompressor);
    TEST_ASSERT_FALSE(flagCaudH);
    TEST_ASSERT_FALSE(flagPresHi);
    TEST_ASSERT_EQUAL(0, contTempCompressor);
    TEST_ASSERT_EQUAL(0, contPressHi);
    TEST_ASSERT_EQUAL(0, contPressLow);
    TEST_ASSERT_EQUAL(0, contTempDes);
    TEST_ASSERT_EQUAL_UINT8(0, nroAlarma);
    TEST_ASSERT_FALSE(alarmaActiva);
    TEST_ASSERT_EQUAL(0, estadoMaquina);
    TEST_ASSERT_EQUAL(1, buzzerStopCalls);
}

void test_reset_alarm_does_nothing_outside_alarm_state(void) {
    resetFixtures();
    estadoMaquina = 3;
    nroAlarma = 9;
    alarmaActiva = true;
    flagCaudH = true;
    resetAlarms();
    TEST_ASSERT_EQUAL(3, estadoMaquina);
    TEST_ASSERT_EQUAL_UINT8(9, nroAlarma);
    TEST_ASSERT_TRUE(alarmaActiva);
    TEST_ASSERT_TRUE(flagCaudH);
    TEST_ASSERT_EQUAL(0, buzzerStopCalls);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_alarm_priority_and_persistence);
    RUN_TEST(test_every_alarm_type_maps_to_existing_code);
    RUN_TEST(test_alarm_priority_is_stable_when_multiple_flags_are_active);
    RUN_TEST(test_alarm_detection_enters_alarm_state);
    RUN_TEST(test_pressure_flags_are_alarm_conditions);
    RUN_TEST(test_reset_alarm_clears_flags_counters_and_state);
    RUN_TEST(test_reset_alarm_does_nothing_outside_alarm_state);
    return UNITY_END();
}
