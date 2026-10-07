#include <unity.h>
#include "measurement_and_calculations.cpp"

unsigned long fake_millis_now = 0;
int fake_digital_inputs[64] = {};
float fake_temperature_by_sensor[256] = {};
bool fake_conversion_complete = true;
int fake_temperature_requests = 0;
TimerOneFake Timer1;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

unsigned long millis() { return fake_millis_now; }
void digitalWrite(int, int) {}
int digitalRead(int pin) { return fake_digital_inputs[pin]; }
void attachInterrupt(int, void (*)(), int) {}
void detachInterrupt(int) {}
void EEPROMwrite(int, bool) {}
void EEPROMwrite(int, uint8_t) {}

void resetFixtures() {
    fake_millis_now = 0;
    for (float& value : fake_temperature_by_sensor) value = 0;
    for (int& value : fake_digital_inputs) value = HIGH;
    fake_conversion_complete = true;
    fake_temperature_requests = 0;
    Caud_T = Caud_H = C1_T = C2_T = C3_T = C1_H = C2_H = C3_H = 0;
    Caud_Tacu = Caud_Hacu = 0;
    Pulsos_Caud_T = Pulsos_Caud_H = 0;
    Ventana_Caudal_T = Ventana_Caudal_H = 0;
    Temp_out_H = Temp_in_H = Temp_out_T = Temp_in_T = 0;
    Temp_Compressor = Temp_Descarga = Temp_Admision = Temp_ACS = 0;
    Temp_CompressorAcu = Temp_DescargaAcu = Temp_ACSacu = 0;
    Cont_Temp_Compressor = Cont_Temp_Descarga = 0;
    Cont_Press_HI = Cont_Press_LOW = 0;
    Flag_TempCompressor = Flag_Temp_Descarga = Flag_Temp_Adm = false;
    Flag_PresHI = Flag_PresLOW = Flag_CaudT = Flag_CaudH = false;
    EnableACS = true;
    EnableACS_DeltaElectrico = true;
    EnableElectricACS = false;
    EnableFlowAlarm = false;
    Estado_Maquina = 0;
}

void test_flow_pulses_and_one_second_calculation(void) {
    resetFixtures();
    initializeFlowState();
    caudalTierra();
    caudalTierra();
    caudalHogar();
    caudalHogar();
    caudalHogar();
    fake_millis_now = 1001;

    flowsCalculation();

    TEST_ASSERT_TRUE(Caud_T > 100);
    TEST_ASSERT_TRUE(Caud_H > Caud_T);
    TEST_ASSERT_EQUAL_UINT16(0, Pulsos_Caud_T);
    TEST_ASSERT_EQUAL_UINT16(0, Pulsos_Caud_H);
}

void test_temperature_measurement_accepts_valid_and_keeps_invalid_values(void) {
    resetFixtures();
    Temp_out_H = 12;
    Temp_in_H = 13;
    Temp_out_T = 14;
    Temp_in_T = 15;
    Temp_Compressor = 16;
    Temp_Descarga = 17;
    Temp_Admision = 18;
    Temp_ACS = 19;
    fake_temperature_by_sensor[DI_Temp_out_H[1]] = 25;
    fake_temperature_by_sensor[DI_Temp_in_H[1]] = -5;
    fake_temperature_by_sensor[DI_Temp_out_T[1]] = 0;
    fake_temperature_by_sensor[DI_Temp_in_T[1]] = -127;
    fake_temperature_by_sensor[DI_Temp_Compresor[1]] = 85;
    fake_temperature_by_sensor[DI_Temp_Descarga[1]] = 89;
    fake_temperature_by_sensor[DI_Temp_Admision[1]] = 0;
    fake_temperature_by_sensor[DI_Temp_ACS[1]] = 79;

    delayedTemperatureMeasurement(nullptr);

    TEST_ASSERT_EQUAL_FLOAT(25, Temp_out_H);
    TEST_ASSERT_EQUAL_FLOAT(-5, Temp_in_H);
    TEST_ASSERT_EQUAL_FLOAT(0, Temp_out_T);
    TEST_ASSERT_EQUAL_FLOAT(15, Temp_in_T);
    TEST_ASSERT_EQUAL_FLOAT(16, Temp_Compressor);
    TEST_ASSERT_EQUAL_FLOAT(89, Temp_Descarga);
    TEST_ASSERT_EQUAL_FLOAT(18, Temp_Admision);
    TEST_ASSERT_EQUAL_FLOAT(79, Temp_ACS);
}

void test_temperature_measurement_requests_then_waits_for_conversion(void) {
    resetFixtures();
    initializeTemperatureMeasurement();
    fake_millis_now = 1000;

    temperatureMeasurement();
    TEST_ASSERT_EQUAL(1, fake_temperature_requests);
    fake_conversion_complete = false;
    fake_millis_now = 2000;
    temperatureMeasurement();
    TEST_ASSERT_EQUAL(1, fake_temperature_requests);
    fake_conversion_complete = true;
    temperatureMeasurement();
    TEST_ASSERT_FALSE(ConversionTemperaturaPendiente);
}

void test_flow_control_requires_start_alarm_enable_and_running_state(void) {
    resetFixtures();
    senal_start = true;
    EnableFlowAlarm = true;
    Estado_Maquina = 3;
    Caud_Tacu = 99;
    Caud_Hacu = 101;

    flowControl();

    TEST_ASSERT_TRUE(Flag_CaudT);
    TEST_ASSERT_FALSE(Flag_CaudH);

    Estado_Maquina = 2;
    Flag_CaudT = false;
    flowControl();
    TEST_ASSERT_FALSE(Flag_CaudT);
}

void test_temperature_and_pressure_alarm_thresholds(void) {
    resetFixtures();
    Temp_CompressorAcu = 81;
    Temp_DescargaAcu = 86;
    Temp_Admision = -8;
    for (int i = 0; i < 3; ++i) temperatureControl();
    TEST_ASSERT_FALSE(Flag_TempCompressor);
    TEST_ASSERT_FALSE(Flag_Temp_Descarga);
    temperatureControl();
    TEST_ASSERT_TRUE(Flag_TempCompressor);
    TEST_ASSERT_TRUE(Flag_Temp_Descarga);
    TEST_ASSERT_TRUE(Flag_Temp_Adm);

    fake_digital_inputs[DI_Pres_HI] = LOW;
    fake_digital_inputs[DI_Pres_LOW] = LOW;
    for (int i = 0; i < 3; ++i) presureControl();
    TEST_ASSERT_FALSE(Flag_PresHI);
    TEST_ASSERT_FALSE(Flag_PresLOW);
    presureControl();
    TEST_ASSERT_TRUE(Flag_PresHI);
    TEST_ASSERT_TRUE(Flag_PresLOW);
}

void test_auxiliary_acs_heater_follows_existing_delta_rules(void) {
    resetFixtures();
    SetP_ACS = 45;
    EnableElectricACS = false;
    Temp_ACSacu = 50;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Calentador);

    Temp_ACSacu = 46;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(HIGH, Valor_DO_Calentador);

    EnableACS_DeltaElectrico = false;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Calentador);

    EnableElectricACS = true;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(HIGH, Valor_DO_Calentador);
}

void test_temperature_calculation_updates_rolling_averages(void) {
    resetFixtures();
    Temp_out_H = 10;
    Temp_in_H = 20;
    Temp_Compressor = 30;
    Temp_ACS = 40;
    Temp_Descarga = 50;
    Caud_T = 60;
    Caud_H = 70;

    temperatureCalculation();

    TEST_ASSERT_EQUAL_FLOAT(10.0f / 3.0f, Temp_out_Hacu);
    TEST_ASSERT_EQUAL_FLOAT(20.0f / 3.0f, Temp_in_Hacu);
    TEST_ASSERT_EQUAL_FLOAT(30.0f / 5.0f, Temp_CompressorAcu);
    TEST_ASSERT_EQUAL_FLOAT(40.0f / 3.0f, Temp_ACSacu);
    TEST_ASSERT_EQUAL_FLOAT(50.0f / 3.0f, Temp_DescargaAcu);
    TEST_ASSERT_EQUAL(20, Caud_Tacu);
    TEST_ASSERT_EQUAL(70 / 3, Caud_Hacu);
}

void test_pressure_flags_clear_when_inputs_recover(void) {
    resetFixtures();
    fake_digital_inputs[DI_Pres_HI] = LOW;
    fake_digital_inputs[DI_Pres_LOW] = LOW;
    for (int i = 0; i < 4; ++i) presureControl();
    TEST_ASSERT_TRUE(Flag_PresHI);
    TEST_ASSERT_TRUE(Flag_PresLOW);
    fake_digital_inputs[DI_Pres_HI] = HIGH;
    fake_digital_inputs[DI_Pres_LOW] = HIGH;
    presureControl();
    TEST_ASSERT_FALSE(Flag_PresHI);
    TEST_ASSERT_FALSE(Flag_PresLOW);
    TEST_ASSERT_EQUAL(0, Cont_Press_HI);
    TEST_ASSERT_EQUAL(0, Cont_Press_LOW);
}

void test_flow_control_does_not_latch_outside_running_states(void) {
    resetFixtures();
    senal_start = true;
    EnableFlowAlarm = true;
    Caud_Tacu = 0;
    Caud_Hacu = 0;
    Estado_Maquina = 2;
    Flag_CaudT = Flag_CaudH = false;
    flowControl();
    TEST_ASSERT_FALSE(Flag_CaudT);
    TEST_ASSERT_FALSE(Flag_CaudH);
    Estado_Maquina = 7;
    flowControl();
    TEST_ASSERT_TRUE(Flag_CaudT);
    TEST_ASSERT_TRUE(Flag_CaudH);
}

void test_auxiliary_heater_requires_temperature_inside_delta_window(void) {
    resetFixtures();
    SetP_ACS = 45;
    EnableElectricACS = false;
    EnableACS = true;
    EnableACS_DeltaElectrico = true;
    Temp_ACSacu = 44;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Calentador);
    Temp_ACSacu = 46;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(HIGH, Valor_DO_Calentador);
    Temp_ACSacu = 53;
    auxiliaryACSHeatingControl();
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Calentador);
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
