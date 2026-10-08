#include <unity.h>
#include "measurement_and_calculations.cpp"

unsigned long fakeMillisNow = 0;
int fakeDigitalInputs[64] = {};
float fakeTemperatureBySensor[256] = {};
bool fakeConversionComplete = true;
int fakeTemperatureRequests = 0;
TimerOneFake Timer1;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

unsigned long millis() { return fakeMillisNow; }
void digitalWrite(int, int) {}
int digitalRead(int pin) { return fakeDigitalInputs[pin]; }
void attachInterrupt(int, void (*)(), int) {}
void detachInterrupt(int) {}
void eepromWrite(int, bool) {}
void eepromWrite(int, uint8_t) {}

void resetFixtures() {
    fakeMillisNow = 0;
    for (float& value : fakeTemperatureBySensor) value = 0;
    for (int& value : fakeDigitalInputs) value = HIGH;
    fakeConversionComplete = true;
    fakeTemperatureRequests = 0;
    caudT = caudH = c1T = c2T = c3T = c1H = c2H = c3H = 0;
    caudTacu = caudHacu = 0;
    pulsosCaudT = pulsosCaudH = 0;
    ventanaCaudalT = ventanaCaudalH = 0;
    tempOutH = tempInH = tempOutT = tempInT = 0;
    tempCompressor = tempDescarga = tempAdmision = tempAcs = 0;
    tempCompressorAcu = tempDescargaAcu = tempAcsAcu = 0;
    contTempCompressor = contTempDescarga = 0;
    contPressHi = contPressLow = 0;
    flagTempCompressor = flagTempDescarga = flagTempAdm = false;
    flagPresHi = flagPresLow = flagCaudT = flagCaudH = false;
    enableAcs = true;
    enableAcsDeltaElectrico = true;
    enableElectricAcs = false;
    enableFlowAlarm = false;
    estadoMaquina = 0;
}

void test_flow_pulses_and_one_second_calculation(void) {
    resetFixtures();
    initializeFlowState();
    caudalTierra();
    caudalTierra();
    caudalHogar();
    caudalHogar();
    caudalHogar();
    fakeMillisNow = 1001;

    flowsCalculation();

    TEST_ASSERT_TRUE(caudT > 100);
    TEST_ASSERT_TRUE(caudH > caudT);
    TEST_ASSERT_EQUAL_UINT16(0, pulsosCaudT);
    TEST_ASSERT_EQUAL_UINT16(0, pulsosCaudH);
}

void test_temperature_measurement_accepts_valid_and_keeps_invalid_values(void) {
    resetFixtures();
    tempOutH = 12;
    tempInH = 13;
    tempOutT = 14;
    tempInT = 15;
    tempCompressor = 16;
    tempDescarga = 17;
    tempAdmision = 18;
    tempAcs = 19;
    fakeTemperatureBySensor[diTempOutH[1]] = 25;
    fakeTemperatureBySensor[diTempInH[1]] = -5;
    fakeTemperatureBySensor[diTempOutT[1]] = 0;
    fakeTemperatureBySensor[diTempInT[1]] = -127;
    fakeTemperatureBySensor[diTempCompresor[1]] = 85;
    fakeTemperatureBySensor[diTempDescarga[1]] = 89;
    fakeTemperatureBySensor[diTempAdmision[1]] = 0;
    fakeTemperatureBySensor[diTempAcs[1]] = 79;

    delayedTemperatureMeasurement(nullptr);

    TEST_ASSERT_EQUAL_FLOAT(25, tempOutH);
    TEST_ASSERT_EQUAL_FLOAT(-5, tempInH);
    TEST_ASSERT_EQUAL_FLOAT(0, tempOutT);
    TEST_ASSERT_EQUAL_FLOAT(15, tempInT);
    TEST_ASSERT_EQUAL_FLOAT(16, tempCompressor);
    TEST_ASSERT_EQUAL_FLOAT(89, tempDescarga);
    TEST_ASSERT_EQUAL_FLOAT(18, tempAdmision);
    TEST_ASSERT_EQUAL_FLOAT(79, tempAcs);
}

void test_temperature_measurement_requests_then_waits_for_conversion(void) {
    resetFixtures();
    initializeTemperatureMeasurement();
    fakeMillisNow = 1000;

    temperatureMeasurement();
    TEST_ASSERT_EQUAL(1, fakeTemperatureRequests);
    fakeConversionComplete = false;
    fakeMillisNow = 2000;
    temperatureMeasurement();
    TEST_ASSERT_EQUAL(1, fakeTemperatureRequests);
    fakeConversionComplete = true;
    temperatureMeasurement();
    TEST_ASSERT_FALSE(conversionTemperaturaPendiente);
}

void test_flow_control_requires_start_alarm_enable_and_running_state(void) {
    resetFixtures();
    senalStart = true;
    enableFlowAlarm = true;
    estadoMaquina = 3;
    caudTacu = 99;
    caudHacu = 101;

    flowControl();

    TEST_ASSERT_TRUE(flagCaudT);
    TEST_ASSERT_FALSE(flagCaudH);

    estadoMaquina = 2;
    flagCaudT = false;
    flowControl();
    TEST_ASSERT_FALSE(flagCaudT);
}

void test_temperature_and_pressure_alarm_thresholds(void) {
    resetFixtures();
    tempCompressorAcu = 81;
    tempDescargaAcu = 86;
    tempAdmision = -8;
    for (int i = 0; i < 3; ++i) temperatureControl();
    TEST_ASSERT_FALSE(flagTempCompressor);
    TEST_ASSERT_FALSE(flagTempDescarga);
    temperatureControl();
    TEST_ASSERT_TRUE(flagTempCompressor);
    TEST_ASSERT_TRUE(flagTempDescarga);
    TEST_ASSERT_TRUE(flagTempAdm);

    fakeDigitalInputs[diPresHi] = LOW;
    fakeDigitalInputs[diPresLow] = LOW;
    for (int i = 0; i < 3; ++i) presureControl();
    TEST_ASSERT_FALSE(flagPresHi);
    TEST_ASSERT_FALSE(flagPresLow);
    presureControl();
    TEST_ASSERT_TRUE(flagPresHi);
    TEST_ASSERT_TRUE(flagPresLow);
}

void test_auxiliary_acs_heater_follows_existing_delta_rules(void) {
    resetFixtures();
    acsSetpoint = 45;
    enableElectricAcs = false;
    tempAcsAcu = 50;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(LOW, valorDoCalentador);

    tempAcsAcu = 46;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(HIGH, valorDoCalentador);

    enableAcsDeltaElectrico = false;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(LOW, valorDoCalentador);

    enableElectricAcs = true;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(HIGH, valorDoCalentador);
}

void test_temperature_calculation_updates_rolling_averages(void) {
    resetFixtures();
    tempOutH = 10;
    tempInH = 20;
    tempCompressor = 30;
    tempAcs = 40;
    tempDescarga = 50;
    caudT = 60;
    caudH = 70;

    temperatureCalculation();

    TEST_ASSERT_EQUAL_FLOAT(10.0f / 3.0f, tempOutHacu);
    TEST_ASSERT_EQUAL_FLOAT(20.0f / 3.0f, tempInHacu);
    TEST_ASSERT_EQUAL_FLOAT(30.0f / 5.0f, tempCompressorAcu);
    TEST_ASSERT_EQUAL_FLOAT(40.0f / 3.0f, tempAcsAcu);
    TEST_ASSERT_EQUAL_FLOAT(50.0f / 3.0f, tempDescargaAcu);
    TEST_ASSERT_EQUAL(20, caudTacu);
    TEST_ASSERT_EQUAL(70 / 3, caudHacu);
}

void test_pressure_flags_clear_when_inputs_recover(void) {
    resetFixtures();
    fakeDigitalInputs[diPresHi] = LOW;
    fakeDigitalInputs[diPresLow] = LOW;
    for (int i = 0; i < 4; ++i) presureControl();
    TEST_ASSERT_TRUE(flagPresHi);
    TEST_ASSERT_TRUE(flagPresLow);
    fakeDigitalInputs[diPresHi] = HIGH;
    fakeDigitalInputs[diPresLow] = HIGH;
    presureControl();
    TEST_ASSERT_FALSE(flagPresHi);
    TEST_ASSERT_FALSE(flagPresLow);
    TEST_ASSERT_EQUAL(0, contPressHi);
    TEST_ASSERT_EQUAL(0, contPressLow);
}

void test_flow_control_does_not_latch_outside_running_states(void) {
    resetFixtures();
    senalStart = true;
    enableFlowAlarm = true;
    caudTacu = 0;
    caudHacu = 0;
    estadoMaquina = 2;
    flagCaudT = flagCaudH = false;
    flowControl();
    TEST_ASSERT_FALSE(flagCaudT);
    TEST_ASSERT_FALSE(flagCaudH);
    estadoMaquina = 7;
    flowControl();
    TEST_ASSERT_TRUE(flagCaudT);
    TEST_ASSERT_TRUE(flagCaudH);
}

void test_auxiliary_heater_requires_temperature_inside_delta_window(void) {
    resetFixtures();
    acsSetpoint = 45;
    enableElectricAcs = false;
    enableAcs = true;
    enableAcsDeltaElectrico = true;
    tempAcsAcu = 44;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(LOW, valorDoCalentador);
    tempAcsAcu = 46;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(HIGH, valorDoCalentador);
    tempAcsAcu = 53;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(LOW, valorDoCalentador);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_flow_pulses_and_one_second_calculation);
    RUN_TEST(test_temperature_measurement_accepts_valid_and_keeps_invalid_values);
    RUN_TEST(test_temperature_measurement_requests_then_waits_for_conversion);
    RUN_TEST(test_flow_control_requires_start_alarm_enable_and_running_state);
    RUN_TEST(test_temperature_and_pressure_alarm_thresholds);
    RUN_TEST(test_auxiliary_acs_heater_follows_existing_delta_rules);
    RUN_TEST(test_temperature_calculation_updates_rolling_averages);
    RUN_TEST(test_pressure_flags_clear_when_inputs_recover);
    RUN_TEST(test_flow_control_does_not_latch_outside_running_states);
    RUN_TEST(test_auxiliary_heater_requires_temperature_inside_delta_window);
    return UNITY_END();
}
