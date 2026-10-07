#include <unity.h>
#include "machine_control.cpp"

unsigned long fake_millis_now = 0;
int fake_digital_inputs[64] = {};
int fake_digital_outputs[64] = {};
TimerOneFake Timer1;
EEPROMFake EEPROM;

#include "../fakes/mega_globals.h"

unsigned long millis() { return fake_millis_now; }
int digitalRead(int pin) { return fake_digital_inputs[pin]; }
void digitalWrite(int pin, int value) { fake_digital_outputs[pin] = value; }
void pinMode(int, int) {}
void tone(int, unsigned int, unsigned long) {}
void noTone(int) {}
void noInterrupts() {}
void interrupts() {}
void attachInterrupt(int, void (*)(), int) {}
void detachInterrupt(int) {}
void EEPROMwrite(int address, bool value) { EEPROM.update(address, value); }
void EEPROMwrite(int address, uint8_t value) { EEPROM.update(address, value); }
void temperatureCalculation() {}
void lcdRefreshValues() {}

void resetFixtures() {
    fake_millis_now = 0;
    for (int& value : fake_digital_inputs) value = LOW;
    for (int& value : fake_digital_outputs) value = LOW;
    Estado_Maquina = 1;
    modoFrio = false;
    heating_off = false;
    Temp_out_H = Temp_out_T = Temp_Admision = 0;
    Ingreso_E3 = 0;
    Periodo_Refresco = 0;
    Flag_Buzzer = false;
    Valor_DO_Buzzer = LOW;
    EEPROM.update_count = 0;
    lastModeChangeAt = 0;
}

void test_start_stop_signal_follows_mode(void) {
    resetFixtures();
    fake_digital_inputs[DI_Marcha_on] = HIGH;
    calculateStartStopSignal();
    TEST_ASSERT_TRUE(senal_start);
    TEST_ASSERT_FALSE(senal_stop);

    modoFrio = true;
    calculateStartStopSignal();
    TEST_ASSERT_FALSE(senal_start);
    TEST_ASSERT_TRUE(senal_stop);

    fake_digital_inputs[DI_Marcha_on] = LOW;
    calculateStartStopSignal();
    TEST_ASSERT_TRUE(senal_start);
    TEST_ASSERT_FALSE(senal_stop);
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
    fake_millis_now = 1;
    changeModo(true);
    TEST_ASSERT_TRUE(modoFrio);
    TEST_ASSERT_EQUAL(1, EEPROM.update_count);
    TEST_ASSERT_TRUE(isModeChangeLocked());

    changeModo(false);
    TEST_ASSERT_TRUE(modoFrio);
    fake_millis_now = 3001;
    changeModo(false);
    TEST_ASSERT_FALSE(modoFrio);

    Estado_Maquina = 3;
    changeModo(true);
    TEST_ASSERT_FALSE(modoFrio);
}

void test_heating_cooling_and_long_period_checks(void) {
    resetFixtures();
    fake_millis_now = 120001;
    Temp_out_H = 43;
    TEST_ASSERT_TRUE(heatingCheck());

    modoFrio = true;
    Temp_out_H = 9;
    TEST_ASSERT_TRUE(coolingCheck());

    fake_millis_now = 43200001;
    TEST_ASSERT_TRUE(longPeriodRunningCheck());
}

void test_take_rest_control_enters_rest_for_each_condition(void) {
    resetFixtures();
    fake_millis_now = 120001;
    Temp_out_H = 43;
    takeRestControl();
    TEST_ASSERT_EQUAL(6, Estado_Maquina);
    TEST_ASSERT_EQUAL(120001, Ingreso_Descanso);

    resetFixtures();
    modoFrio = true;
    Temp_out_T = 41;
    takeRestControl();
    TEST_ASSERT_EQUAL(6, Estado_Maquina);
}

void test_outputs_and_buzzer_follow_existing_control(void) {
    resetFixtures();
    Valor_DO_Bombas = HIGH;
    Valor_DO_Calentador = HIGH;
    Valor_DO_V4V = HIGH;
    Valor_DO_Compressor = HIGH;
    Valor_DO_VACS = HIGH;
    writeOutput();
    TEST_ASSERT_EQUAL(HIGH, fake_digital_outputs[DO_Bombas]);
    TEST_ASSERT_EQUAL(HIGH, fake_digital_outputs[DO_Compressor]);
    TEST_ASSERT_EQUAL(HIGH, fake_digital_outputs[DO_ValvulaACS]);

    Flag_Buzzer = true;
    Estado_Maquina = 1;
    buzzerControl();
    TEST_ASSERT_FALSE(Flag_Buzzer);
    Estado_Maquina = 4;
    Flag_Buzzer = true;
    buzzerControl();
    TEST_ASSERT_TRUE(Flag_Buzzer);
    buzzerStop();
    TEST_ASSERT_FALSE(Flag_Buzzer);
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Buzzer);
}

void test_output_initialization_turns_every_output_off(void) {
    resetFixtures();
    for (int& value : fake_digital_outputs) value = HIGH;
    initializeDigitalOuputs();
    TEST_ASSERT_EQUAL(LOW, fake_digital_outputs[DO_Compressor]);
    TEST_ASSERT_EQUAL(LOW, fake_digital_outputs[DO_ValvulaACS]);
    TEST_ASSERT_EQUAL(LOW, fake_digital_outputs[DO_Bombas]);
    TEST_ASSERT_EQUAL(LOW, fake_digital_outputs[DO_Calentador]);
    TEST_ASSERT_EQUAL(LOW, fake_digital_outputs[DO_Valvula4Vias]);
    TEST_ASSERT_EQUAL(LOW, fake_digital_outputs[DO_Triac_01]);
    TEST_ASSERT_EQUAL(LOW, fake_digital_outputs[DO_Buzzer]);
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
