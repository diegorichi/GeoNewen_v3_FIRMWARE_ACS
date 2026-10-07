#include <unity.h>
#include "SerialEsp8266.h"
#include "../protocol_cases.h"

unsigned long fake_millis_now = 0;
int fake_digital_inputs[64] = {};
HardwareSerial Serial;
TimerOneFake Timer1;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

unsigned long millis() { return fake_millis_now; }
void digitalWrite(int, int) {}
void stateMachine0() { Estado_Maquina = 1; }
void resetAlarms() { Estado_Maquina = 0; }
void changeModo(bool value) { modoFrio = value; }
uint8_t normalizeAcsTemp(volatile uint8_t* value) {
    if (*value < 30) *value = 30;
    if (*value > 48) *value = 48;
    return *value;
}
void EEPROMwrite(int address, bool value) { EEPROM.update(address, value); }
void EEPROMwrite(int address, uint8_t value) { EEPROM.update(address, value); }

SerialEsp8266* activeEsp = nullptr;

void resetFixtures() {
    fake_millis_now = 0;
    Serial.input[0] = '\0';
    Serial.inputLength = 0;
    Serial.output[0] = '\0';
    Serial.outputLength = 0;
    EEPROM.update_count = 0;
    EnableACS = true;
    EnableACS_DeltaElectrico = true;
    EnableElectricACS = false;
    SetP_ACS = 45;
    SetP_ACS_Edit = 45;
    Estado_Maquina = 1;
    pendingStatusActive = false;
    pendingStatusAttempts = 0;
    pendingStatusSequence = 0;
    pendingStatusSentAt = 0;
    espQueue.clear();
}

void test_valid_command_updates_state_and_sends_ack(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    strcpy(Serial.input, "cmd:1:ACS_G:on#");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();

    TEST_ASSERT_TRUE(EnableACS);
    TEST_ASSERT_EQUAL(1, EEPROM.update_count);
    TEST_ASSERT_NOT_NULL(strstr(Serial.output, "ack:cmd:1#"));
}

void test_duplicate_command_only_acknowledges_without_reexecuting(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    EnableACS = true;
    strcpy(Serial.input, "cmd:4:ACS_G:off#cmd:4:ACS_G:on#");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();

    TEST_ASSERT_FALSE(EnableACS);
    TEST_ASSERT_EQUAL(1, EEPROM.update_count);
    const char* firstAck = strstr(Serial.output, "ack:cmd:4#");
    TEST_ASSERT_NOT_NULL(firstAck);
    TEST_ASSERT_NOT_NULL(strstr(firstAck + 1, "ack:cmd:4#"));
}

void test_setpoint_command_uses_existing_limits(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    strcpy(Serial.input, "cmd:2:TEMP_ACS:99#");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();
    TEST_ASSERT_EQUAL_UINT8(48, SetP_ACS);
    TEST_ASSERT_EQUAL_UINT8(48, SetP_ACS_Edit);
    TEST_ASSERT_NOT_NULL(strstr(Serial.output, "ack:cmd:2#"));
}

void test_status_queue_sends_then_retries_until_limit(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    TEST_ASSERT_TRUE(enqueueEspMessage("status:9:STATE_MACH:0001#"));
    sendToSerial(&Serial);
    TEST_ASSERT_TRUE(pendingStatusActive);
    TEST_ASSERT_EQUAL(1, pendingStatusAttempts);
    fake_millis_now = 2000;
    sendToSerial(&Serial);
    TEST_ASSERT_EQUAL(2, pendingStatusAttempts);
    fake_millis_now = 4000;
    sendToSerial(&Serial);
    TEST_ASSERT_EQUAL(3, pendingStatusAttempts);
    fake_millis_now = 6000;
    sendToSerial(&Serial);
    TEST_ASSERT_FALSE(pendingStatusActive);
}

void test_status_ack_clears_pending_message(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    enqueueEspMessage("status:12:STATE_MACH:0001#");
    sendToSerial(&Serial);
    TEST_ASSERT_TRUE(pendingStatusActive);
    strcpy(Serial.input, "ack:status:12#");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();
    TEST_ASSERT_FALSE(pendingStatusActive);
}

void test_invalid_and_overlong_frames_are_not_executed(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    EnableACS = true;
    strcpy(Serial.input, "cmd:x:ACS_G:off#cmd:5:ACS_G:off");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();
    TEST_ASSERT_TRUE(EnableACS);
    TEST_ASSERT_EQUAL(0, EEPROM.update_count);
    TEST_ASSERT_EQUAL(0, Serial.outputLength);
}

void test_all_inbound_commands_update_their_target(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    strcpy(Serial.input, "cmd:1:ACS_G:off#cmd:2:ACS_DT_E:off#cmd:3:ACS_E:on#cmd:4:ALARM:reset#cmd:5:MODO_FRIO:on#cmd:6:TEMP_ACS:31#");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();

    TEST_ASSERT_TRUE(EnableElectricACS);
    TEST_ASSERT_FALSE(EnableACS);
    TEST_ASSERT_FALSE(EnableACS_DeltaElectrico);
    TEST_ASSERT_TRUE(modoFrio);
    TEST_ASSERT_EQUAL_UINT8(31, SetP_ACS);
    TEST_ASSERT_EQUAL_UINT8(31, SetP_ACS_Edit);
    TEST_ASSERT_NOT_NULL(strstr(Serial.output, "ack:cmd:6#"));
}

void test_status_snapshot_contains_all_expected_frames(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    EnableACS = true;
    EnableACS_DeltaElectrico = false;
    EnableElectricACS = true;
    modoFrio = true;
    Estado_Maquina = 3;
    Nro_Alarma = 9;
    strcpy(Serial.input, "cmd:1:ACS_G:on#");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();

    TEST_ASSERT_EQUAL(16, espQueue.item_count());
    EspMessage first = espQueue.dequeue();
    TEST_ASSERT_EQUAL_STRING("status:1:contrl:ACS_GEO___:1#", first.text);
    EspMessage second = espQueue.dequeue();
    TEST_ASSERT_EQUAL_STRING("status:2:contrl:ACS_DT_ELE:0#", second.text);
}

void test_wrong_status_ack_does_not_clear_pending(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    enqueueEspMessage("status:12:STATE_MACH:0001#");
    sendToSerial(&Serial);
    strcpy(Serial.input, "ack:status:99#");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();
    TEST_ASSERT_TRUE(pendingStatusActive);
}

void test_non_numeric_status_sequence_is_not_sent(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    TEST_ASSERT_TRUE(enqueueEspMessage("status:x:STATE_MACH:0001#"));
    sendToSerial(&Serial);
    TEST_ASSERT_FALSE(pendingStatusActive);
}

void test_protocol_cases_cover_real_mega_handlers(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);

    strcpy(Serial.input, MEGA_PROTOCOL_CASES[0].frame);
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();
    TEST_ASSERT_NOT_NULL(strstr(Serial.output, "ack:cmd:1#"));

    resetFixtures();
    SerialEsp8266 invalidEsp(&Serial);
    strcpy(Serial.input, MEGA_PROTOCOL_CASES[5].frame);
    Serial.inputLength = strlen(Serial.input);
    invalidEsp.handleEspSerial();
    TEST_ASSERT_EQUAL(0, Serial.outputLength);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_valid_command_updates_state_and_sends_ack);
    RUN_TEST(test_duplicate_command_only_acknowledges_without_reexecuting);
    RUN_TEST(test_setpoint_command_uses_existing_limits);
    RUN_TEST(test_status_queue_sends_then_retries_until_limit);
    RUN_TEST(test_status_ack_clears_pending_message);
    RUN_TEST(test_invalid_and_overlong_frames_are_not_executed);
    RUN_TEST(test_all_inbound_commands_update_their_target);
    RUN_TEST(test_status_snapshot_contains_all_expected_frames);
    RUN_TEST(test_wrong_status_ack_does_not_clear_pending);
    RUN_TEST(test_non_numeric_status_sequence_is_not_sent);
    RUN_TEST(test_protocol_cases_cover_real_mega_handlers);
    return UNITY_END();
}
