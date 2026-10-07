#include <unity.h>
#include "alarm.cpp"

unsigned long fake_millis_now = 0;
TimerOneFake Timer1;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

int eeprom_alarm_writes = 0;
int buzzer_stop_calls = 0;

unsigned long millis() { return fake_millis_now; }
void digitalWrite(int, int) {}
void EEPROMwrite(int address, uint8_t number) {
    if (address == Alarma_Address && number != 0) {
        ++eeprom_alarm_writes;
    }
}
void EEPROMwrite(int, bool) {}
void buzzerStop() { ++buzzer_stop_calls; }

void resetFixtures() {
    Estado_Maquina = 0;
    Nro_Alarma = 0;
    Alarma_Activa = false;
    Flag_TempCompressor = false;
    Flag_Temp_Descarga = false;
    Flag_CaudT = false;
    Flag_CaudH = false;
    Flag_PresHI = false;
    Flag_PresLOW = false;
    Flag_Temp_Adm = false;
    Cont_Temp_Compressor = 4;
    Cont_Press_HI = 5;
    Cont_Press_LOW = 6;
    Cont_Temp_Des = 7;
    eeprom_alarm_writes = 0;
    buzzer_stop_calls = 0;
}

void test_alarm_priority_and_persistence(void) {
    resetFixtures();
    Flag_CaudT = true;
    Flag_PresHI = true;

    ConvertFlagToAlarm();

    TEST_ASSERT_EQUAL_UINT8(7, Nro_Alarma);
    TEST_ASSERT_EQUAL(1, eeprom_alarm_writes);
}

void test_every_alarm_type_maps_to_existing_code(void) {
    const bool* flags[] = {
        &Flag_TempCompressor, &Flag_CaudT, &Flag_CaudH, &Flag_PresHI,
        &Flag_PresLOW, &Flag_Temp_Adm, &Flag_Temp_Descarga};
    const uint8_t expected[] = {6, 7, 8, 9, 10, 15, 18};

    for (unsigned int i = 0; i < 7; ++i) {
        resetFixtures();
        *const_cast<bool*>(flags[i]) = true;
        ConvertFlagToAlarm();
        TEST_ASSERT_EQUAL_UINT8(expected[i], Nro_Alarma);
        TEST_ASSERT_EQUAL(1, eeprom_alarm_writes);
    }
}

void test_alarm_priority_is_stable_when_multiple_flags_are_active(void) {
    resetFixtures();
    Flag_TempCompressor = true;
    Flag_CaudT = true;
    Flag_CaudH = true;
    Flag_PresHI = true;
    Flag_PresLOW = true;
    Flag_Temp_Adm = true;
    Flag_Temp_Descarga = true;

    ConvertFlagToAlarm();

    TEST_ASSERT_EQUAL_UINT8(6, Nro_Alarma);
}

void test_alarm_detection_enters_alarm_state(void) {
    resetFixtures();
    Flag_PresLOW = true;

    checkFlagsForAlarms();

    TEST_ASSERT_EQUAL(4, Estado_Maquina);
}

void test_pressure_flags_are_alarm_conditions(void) {
    resetFixtures();
    Flag_PresHI = true;
    checkFlagsForAlarms();
    TEST_ASSERT_EQUAL(4, Estado_Maquina);

    resetFixtures();
    Flag_PresLOW = true;
    checkFlagsForAlarms();
    TEST_ASSERT_EQUAL(4, Estado_Maquina);
}

void test_reset_alarm_clears_flags_counters_and_state(void) {
    resetFixtures();
    Estado_Maquina = 4;
    Flag_TempCompressor = true;
    Flag_CaudH = true;
    Flag_PresHI = true;
    Alarma_Activa = true;
    Nro_Alarma = 9;

    resetAlarms();

    TEST_ASSERT_FALSE(Flag_TempCompressor);
    TEST_ASSERT_FALSE(Flag_CaudH);
    TEST_ASSERT_FALSE(Flag_PresHI);
    TEST_ASSERT_EQUAL(0, Cont_Temp_Compressor);
    TEST_ASSERT_EQUAL(0, Cont_Press_HI);
    TEST_ASSERT_EQUAL(0, Cont_Press_LOW);
    TEST_ASSERT_EQUAL(0, Cont_Temp_Des);
    TEST_ASSERT_EQUAL_UINT8(0, Nro_Alarma);
    TEST_ASSERT_FALSE(Alarma_Activa);
    TEST_ASSERT_EQUAL(0, Estado_Maquina);
    TEST_ASSERT_EQUAL(1, buzzer_stop_calls);
}

void test_reset_alarm_does_nothing_outside_alarm_state(void) {
    resetFixtures();
    Estado_Maquina = 3;
    Nro_Alarma = 9;
    Alarma_Activa = true;
    Flag_CaudH = true;
    resetAlarms();
    TEST_ASSERT_EQUAL(3, Estado_Maquina);
    TEST_ASSERT_EQUAL_UINT8(9, Nro_Alarma);
    TEST_ASSERT_TRUE(Alarma_Activa);
    TEST_ASSERT_TRUE(Flag_CaudH);
    TEST_ASSERT_EQUAL(0, buzzer_stop_calls);
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
