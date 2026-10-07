#include <unity.h>
#include "kume_eeprom.cpp"

unsigned long fake_millis_now = 0;
TimerOneFake Timer1;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

unsigned long millis() { return fake_millis_now; }

void test_eeprom_reads_saved_values(void) {
    EEPROM.memory[SetP_ACS_Address] = 52;
    EEPROM.memory[EnableFlowAlarm_Address] = 1;
    EEPROM.memory[EnableACS_Address] = 0;
    EEPROM.memory[EnableACS_DeltaElectrico_Address] = 1;
    EEPROM.memory[EnableElectricACS_Address] = 1;
    EEPROM.memory[modoFrio_address] = 1;
    EEPROM.memory[heating_off_address] = 1;

    EEPROMLectura();

    TEST_ASSERT_EQUAL_UINT8(52, SetP_ACS);
    TEST_ASSERT_EQUAL_UINT8(52, SetP_ACS_Edit);
    TEST_ASSERT_TRUE(EnableFlowAlarm);
    TEST_ASSERT_FALSE(EnableACS);
    TEST_ASSERT_TRUE(EnableACS_DeltaElectrico);
    TEST_ASSERT_TRUE(EnableElectricACS);
    TEST_ASSERT_TRUE(modoFrio);
    TEST_ASSERT_TRUE(heating_off);
}

void test_eeprom_invalid_boolean_uses_current_default(void) {
    EnableFlowAlarm = true;
    EnableACS = false;
    EEPROM.memory[EnableFlowAlarm_Address] = 99;
    EEPROM.memory[EnableACS_Address] = 99;

    EEPROMLectura();

    TEST_ASSERT_FALSE(EnableFlowAlarm);
    TEST_ASSERT_TRUE(EnableACS);
}

void test_eeprom_write_uses_update_and_correct_address(void) {
    EEPROM.update_count = 0;

    EEPROMwrite(SetP_ACS_Address, static_cast<uint8_t>(48));

    TEST_ASSERT_EQUAL_UINT8(48, EEPROM.memory[SetP_ACS_Address]);
    TEST_ASSERT_EQUAL(1, EEPROM.update_count);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_eeprom_reads_saved_values);
    RUN_TEST(test_eeprom_invalid_boolean_uses_current_default);
    RUN_TEST(test_eeprom_write_uses_update_and_correct_address);
    return UNITY_END();
}
