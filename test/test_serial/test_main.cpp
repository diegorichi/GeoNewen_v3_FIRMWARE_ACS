#include <unity.h>
#include "SerialEsp8266.h"
#include "../protocol_cases.h"

unsigned long fakeMillisNow = 0;
int fakeDigitalInputs[64] = {};
HardwareSerial Serial;
TimerOneFake Timer1;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

unsigned long millis() { return fakeMillisNow; }
void digitalWrite(int, int) {}
void stateMachine0() { estadoMaquina = 1; }
void resetAlarms() { estadoMaquina = 0; }
void changeModo(bool value) { modoFrio = value; }
uint8_t normalizeAcsTemp(volatile uint8_t* value) {
    if (*value < 30) *value = 30;
    if (*value > 48) *value = 48;
    return *value;
}
void eepromWrite(int address, bool value) { EEPROM.update(address, value); }
void eepromWrite(int address, uint8_t value) { EEPROM.update(address, value); }

SerialEsp8266* activeEsp = nullptr;

void resetFixtures() {
    fakeMillisNow = 0;
    Serial.input[0] = '\0';
    Serial.inputLength = 0;
    Serial.output[0] = '\0';
    Serial.outputLength = 0;
    EEPROM.update_count = 0;
    enableAcs = true;
    enableAcsDeltaElectrico = true;
    enableElectricAcs = false;
    heatingOff = false;
    acsSetpoint = 45;
    acsSetpointEdit = 45;
    estadoMaquina = 1;
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

    TEST_ASSERT_TRUE(enableAcs);
    TEST_ASSERT_EQUAL(1, EEPROM.update_count);
    TEST_ASSERT_NOT_NULL(strstr(Serial.output, "ack:cmd:1#"));
}

void test_duplicate_command_only_acknowledges_without_reexecuting(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    enableAcs = true;
    strcpy(Serial.input, "cmd:4:ACS_G:off#cmd:4:ACS_G:on#");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();

    TEST_ASSERT_FALSE(enableAcs);
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
    TEST_ASSERT_EQUAL_UINT8(48, acsSetpoint);
    TEST_ASSERT_EQUAL_UINT8(48, acsSetpointEdit);
    TEST_ASSERT_NOT_NULL(strstr(Serial.output, "ack:cmd:2#"));
}

void test_heating_off_command_updates_state_and_persists(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    strcpy(Serial.input, "cmd:2:HEATING_OFF:on#");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();
    TEST_ASSERT_TRUE(heatingOff);
    TEST_ASSERT_EQUAL(1, EEPROM.update_count);
    TEST_ASSERT_NOT_NULL(strstr(Serial.output, "ack:cmd:2#"));
}

void test_status_request_enqueues_snapshot_without_state_change(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    strcpy(Serial.input, "cmd:2:STATUS:publish#");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();
    TEST_ASSERT_EQUAL(20, espQueue.item_count());
    TEST_ASSERT_NOT_NULL(strstr(Serial.output, "ack:cmd:2#"));
}

void test_discrete_change_enqueues_snapshot_but_continuous_change_does_not(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    esp.detectAndEnqueueChangedStatus();
    TEST_ASSERT_EQUAL(20, espQueue.item_count());
    espQueue.clear();

    tempAcsAcu = 31.25;
    esp.detectAndEnqueueChangedStatus();
    TEST_ASSERT_TRUE(espQueue.isEmpty());

    estadoMaquina = 3;
    esp.detectAndEnqueueChangedStatus();
    TEST_ASSERT_EQUAL(20, espQueue.item_count());
}

void test_status_queue_sends_then_retries_until_limit(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    TEST_ASSERT_TRUE(enqueueEspMessage("status:9:STATE_MACH:0001#"));
    sendToSerial(&Serial);
    TEST_ASSERT_TRUE(pendingStatusActive);
    TEST_ASSERT_EQUAL(1, pendingStatusAttempts);
    fakeMillisNow = 2000;
    sendToSerial(&Serial);
    TEST_ASSERT_EQUAL(2, pendingStatusAttempts);
    fakeMillisNow = 4000;
    sendToSerial(&Serial);
    TEST_ASSERT_EQUAL(3, pendingStatusAttempts);
    fakeMillisNow = 6000;
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
    enableAcs = true;
    strcpy(Serial.input, "cmd:x:ACS_G:off#cmd:5:ACS_G:off");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();
    TEST_ASSERT_TRUE(enableAcs);
    TEST_ASSERT_EQUAL(0, EEPROM.update_count);
    TEST_ASSERT_EQUAL(0, Serial.outputLength);
}

void test_all_inbound_commands_update_their_target(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    strcpy(Serial.input, "cmd:1:ACS_G:off#cmd:2:ACS_DT_E:off#cmd:3:ACS_E:on#cmd:4:ALARM:reset#cmd:5:MODO_FRIO:on#cmd:6:TEMP_ACS:31#");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();

    TEST_ASSERT_TRUE(enableElectricAcs);
    TEST_ASSERT_FALSE(enableAcs);
    TEST_ASSERT_FALSE(enableAcsDeltaElectrico);
    TEST_ASSERT_TRUE(modoFrio);
    TEST_ASSERT_EQUAL_UINT8(31, acsSetpoint);
    TEST_ASSERT_EQUAL_UINT8(31, acsSetpointEdit);
    TEST_ASSERT_NOT_NULL(strstr(Serial.output, "ack:cmd:6#"));
}

void test_status_snapshot_contains_all_expected_frames(void) {
    resetFixtures();
    SerialEsp8266 esp(&Serial);
    enableAcs = true;
    enableAcsDeltaElectrico = false;
    enableElectricAcs = true;
    modoFrio = true;
    estadoMaquina = 3;
    nroAlarma = 9;
    strcpy(Serial.input, "cmd:1:ACS_G:on#");
    Serial.inputLength = strlen(Serial.input);
    esp.handleEspSerial();
    esp.detectAndEnqueueChangedStatus();

    TEST_ASSERT_EQUAL(20, espQueue.item_count());
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
    RUN_TEST(test_heating_off_command_updates_state_and_persists);
    RUN_TEST(test_status_request_enqueues_snapshot_without_state_change);
    RUN_TEST(test_discrete_change_enqueues_snapshot_but_continuous_change_does_not);
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
