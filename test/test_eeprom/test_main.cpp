#include <unity.h>
#include "kume_eeprom.cpp"

unsigned long fakeMillisNow = 0;
TimerOneFake Timer1;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

unsigned long millis() { return fakeMillisNow; }

void test_eeprom_reads_saved_values(void) {
    EEPROM.memory[acsSetpointAddress] = 52;
    EEPROM.memory[enableFlowAlarmAddress] = 1;
    EEPROM.memory[enableAcsAddress] = 0;
    EEPROM.memory[enableAcsDeltaElectricoAddress] = 1;
    EEPROM.memory[enableElectricAcsAddress] = 1;
    EEPROM.memory[modoFrioAddress] = 1;
    EEPROM.memory[heatingOffAddress] = 1;

    EEPROMLectura();

    TEST_ASSERT_EQUAL_UINT8(52, acsSetpoint);
    TEST_ASSERT_EQUAL_UINT8(52, acsSetpointEdit);
    TEST_ASSERT_TRUE(enableFlowAlarm);
    TEST_ASSERT_FALSE(enableAcs);
    TEST_ASSERT_TRUE(enableAcsDeltaElectrico);
    TEST_ASSERT_TRUE(enableElectricAcs);
    TEST_ASSERT_TRUE(modoFrio);
    TEST_ASSERT_TRUE(heatingOff);
}

void test_eeprom_invalid_boolean_uses_current_default(void) {
    enableFlowAlarm = true;
    enableAcs = false;
    EEPROM.memory[enableFlowAlarmAddress] = 99;
    EEPROM.memory[enableAcsAddress] = 99;

    EEPROMLectura();

    TEST_ASSERT_FALSE(enableFlowAlarm);
    TEST_ASSERT_TRUE(enableAcs);
}

void test_eeprom_write_uses_update_and_correct_address(void) {
    EEPROM.update_count = 0;

    eepromWrite(acsSetpointAddress, static_cast<uint8_t>(48));

    TEST_ASSERT_EQUAL_UINT8(48, EEPROM.memory[acsSetpointAddress]);
    TEST_ASSERT_EQUAL(1, EEPROM.update_count);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_eeprom_reads_saved_values);
    RUN_TEST(test_eeprom_invalid_boolean_uses_current_default);
    RUN_TEST(test_eeprom_write_uses_update_and_correct_address);
    return UNITY_END();
}
