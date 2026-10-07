#include <unity.h>
#include "stateMachine.cpp"

unsigned long fake_millis_now = 0;
TimerOneFake Timer1;
EEPROMFake EEPROM;
int digital_output_values[64] = {};

unsigned long millis() { return fake_millis_now; }
void digitalWrite(int pin, int value) {
    if (pin >= 0 && pin < 64) digital_output_values[pin] = value;
}

#include "../fakes/mega_globals.h"

bool fake_alarm_active = false;
int fake_buzzer_bips = 0;
int fake_rest_checks = 0;

void checkFlagsForAlarms() { if (fake_alarm_active) Estado_Maquina = 4; }
void ConvertFlagToAlarm() { if (Nro_Alarma == 0) Nro_Alarma = 1; }
void EEPROMwrite(int, bool) {}
void EEPROMwrite(int, uint8_t) {}
void buzzerBip() { fake_buzzer_bips++; }
void buzzerStop() { Flag_Buzzer = false; }
void takeRestControl() { fake_rest_checks++; }

void resetFixtures() {
    fake_millis_now = 0;
    fake_alarm_active = false;
    fake_buzzer_bips = 0;
    fake_rest_checks = 0;
    Timer1 = TimerOneFake{};
    for (int& value : digital_output_values) value = 0;
    Estado_Maquina = 0;
    heating_off = false;
    modoFrio = false;
    senal_start = false;
    senal_stop = false;
    EnableACS = true;
    Temp_ACS = 0;
    Temp_out_H = 0;
    Temp_Descarga = 0;
    SetP_ACS = 45;
    Nro_Alarma = 0;
    Alarma_Activa = false;
    Flag_Buzzer = false;
    Valor_DO_Compressor = LOW;
    Valor_DO_Bombas = LOW;
}

void test_initial_state_is_safe(void) {
    resetFixtures();
    initializeStateMachine();
    stateMachine0();
    TEST_ASSERT_EQUAL(1, Estado_Maquina);
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Compressor);
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Bombas);
    TEST_ASSERT_EQUAL(LOW, Valor_DO_VACS);
    TEST_ASSERT_EQUAL(HIGH, Valor_DO_V4V);
}

void test_start_reaches_pump_state_after_existing_delay(void) {
    resetFixtures();
    Estado_Maquina = 1;
    senal_start = true;
    EnableACS = false;
    stateMachine1();
    TEST_ASSERT_EQUAL(1, Estado_Maquina);
    fake_millis_now = 60001;
    stateMachine1();
    TEST_ASSERT_EQUAL(2, Estado_Maquina);
}

void test_pumps_then_compressor_then_running_state(void) {
    resetFixtures();
    Estado_Maquina = 2;
    stateMachine2();
    TEST_ASSERT_EQUAL(HIGH, Valor_DO_Bombas);
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Compressor);
    fake_millis_now = 25001;
    stateMachine2();
    TEST_ASSERT_EQUAL(HIGH, Valor_DO_Compressor);
    fake_millis_now = 40002;
    stateMachine2();
    TEST_ASSERT_EQUAL(3, Estado_Maquina);
}

void test_stop_returns_to_safe_initial_state(void) {
    resetFixtures();
    Estado_Maquina = 2;
    Valor_DO_Compressor = HIGH;
    Valor_DO_Bombas = HIGH;
    senal_stop = true;
    stateMachine2();
    TEST_ASSERT_EQUAL(0, Estado_Maquina);
}

void test_alarm_has_priority_in_startup(void) {
    resetFixtures();
    Estado_Maquina = 2;
    fake_alarm_active = true;
    stateMachine2();
    TEST_ASSERT_EQUAL(4, Estado_Maquina);
    stateMachine4();
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Compressor);
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Bombas);
    TEST_ASSERT_TRUE(Alarma_Activa);
}

void test_acs_generation_stops_at_setpoint(void) {
    resetFixtures();
    Estado_Maquina = 7;
    Temp_ACS = 45;
    stateMachine7();
    TEST_ASSERT_EQUAL(0, Estado_Maquina);
}

void test_initial_state_respects_heating_off_and_cold_mode(void) {
    resetFixtures();
    initializeStateMachine();
    heating_off = true;
    modoFrio = true;
    stateMachine0();

    TEST_ASSERT_EQUAL(0, Estado_Maquina);
    TEST_ASSERT_EQUAL(LOW, Valor_DO_V4V);
}

void test_state_one_heating_off_returns_to_initial_state(void) {
    resetFixtures();
    Estado_Maquina = 1;
    heating_off = true;

    stateMachine1();

    TEST_ASSERT_EQUAL(0, Estado_Maquina);
}

void test_state_one_starts_acs_generation_after_valve_delay(void) {
    resetFixtures();
    Estado_Maquina = 1;
    EnableACS = true;
    Temp_ACS = 30;
    SetP_ACS = 45;
    valvulaACSStart = 0;
    fake_millis_now = 15001;

    stateMachine1();

    TEST_ASSERT_EQUAL(7, Estado_Maquina);
    TEST_ASSERT_EQUAL(HIGH, Valor_DO_VACS);
    TEST_ASSERT_EQUAL(HIGH, Valor_DO_V4V);
    TEST_ASSERT_EQUAL(15001, Ingreso_E7);
}

void test_state_one_daily_pump_routine_runs_and_stops(void) {
    resetFixtures();
    Estado_Maquina = 1;
    dontStuckPumpsStart = 0;
    EnableACS = false;
    valvulaACSStart = fake_millis_now;
    fake_millis_now = 86400001UL;

    stateMachine1();

    TEST_ASSERT_EQUAL(HIGH, Valor_DO_Bombas);
    TEST_ASSERT_EQUAL(1, fake_buzzer_bips);
    fake_millis_now = 86410002UL;
    stateMachine1();
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Bombas);
}

void test_state_two_stop_and_heating_off_are_safe(void) {
    resetFixtures();
    Estado_Maquina = 2;
    senal_stop = true;
    stateMachine2();
    TEST_ASSERT_EQUAL(0, Estado_Maquina);

    resetFixtures();
    Estado_Maquina = 2;
    heating_off = true;
    stateMachine2();
    TEST_ASSERT_EQUAL(0, Estado_Maquina);
}

void test_state_three_handles_alarm_before_stop_and_rest(void) {
    resetFixtures();
    Estado_Maquina = 3;
    fake_alarm_active = true;
    senal_stop = true;
    stateMachine3();
    TEST_ASSERT_EQUAL(4, Estado_Maquina);

    resetFixtures();
    Estado_Maquina = 3;
    senal_stop = true;
    stateMachine3();
    TEST_ASSERT_EQUAL(0, Estado_Maquina);

    resetFixtures();
    Estado_Maquina = 3;
    EnableACS = false;
    stateMachine3();
    TEST_ASSERT_EQUAL(1, fake_rest_checks);
}

void test_alarm_state_stops_outputs_and_starts_buzzer_once(void) {
    resetFixtures();
    Estado_Maquina = 4;
    Valor_DO_Bombas = HIGH;
    Valor_DO_Compressor = HIGH;
    Nro_Alarma = 6;

    stateMachine4();
    stateMachine4();

    TEST_ASSERT_EQUAL(LOW, Valor_DO_Bombas);
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Compressor);
    TEST_ASSERT_TRUE(Flag_Buzzer);
    TEST_ASSERT_EQUAL(100, Timer1.lastValue);
}

void test_rest_state_exits_after_time_or_heating_off(void) {
    resetFixtures();
    Estado_Maquina = 6;
    Ingreso_Descanso = 0;
    fake_millis_now = 400001;
    stateMachine6();
    TEST_ASSERT_EQUAL(0, Estado_Maquina);

    resetFixtures();
    Estado_Maquina = 6;
    heating_off = true;
    stateMachine6();
    TEST_ASSERT_EQUAL(0, Estado_Maquina);
}

void test_state_seven_starts_pumps_and_compressor_after_delays(void) {
    resetFixtures();
    Estado_Maquina = 7;
    EnableACS = true;
    Temp_ACS = 30;
    SetP_ACS = 45;
    valvulaACSStart = 0;
    Ingreso_E7 = 0;
    fake_millis_now = 15001;
    stateMachine7();
    TEST_ASSERT_EQUAL(HIGH, Valor_DO_Bombas);
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Compressor);

    fake_millis_now = 20001;
    stateMachine7();
    TEST_ASSERT_EQUAL(HIGH, Valor_DO_Compressor);
}

void test_state_seven_stops_for_disable_target_or_heating_off(void) {
    resetFixtures();
    Estado_Maquina = 7;
    EnableACS = false;
    stateMachine7();
    TEST_ASSERT_EQUAL(0, Estado_Maquina);

    resetFixtures();
    Estado_Maquina = 7;
    Temp_ACS = 45;
    SetP_ACS = 45;
    stateMachine7();
    TEST_ASSERT_EQUAL(0, Estado_Maquina);

    resetFixtures();
    Estado_Maquina = 7;
    heating_off = true;
    stateMachine7();
    TEST_ASSERT_EQUAL(0, Estado_Maquina);
}

void test_state_seven_enters_71_for_temperature_safety_or_alarm(void) {
    resetFixtures();
    Estado_Maquina = 7;
    Temp_out_H = 50.1;
    stateMachine7();
    TEST_ASSERT_EQUAL(71, Estado_Maquina);
    TEST_ASSERT_EQUAL(0, Ingreso_E71);

    resetFixtures();
    Estado_Maquina = 7;
    Temp_out_H = 0;
    Temp_Descarga = 80.1;
    stateMachine7();
    TEST_ASSERT_EQUAL(71, Estado_Maquina);

    resetFixtures();
    Estado_Maquina = 7;
    fake_alarm_active = true;
    fake_millis_now = 20001;
    stateMachine7();
    TEST_ASSERT_EQUAL(4, Estado_Maquina);
}

void test_state_71_returns_to_generation_for_each_release_condition(void) {
    resetFixtures();
    Estado_Maquina = 71;
    Temp_ACS = 46;
    SetP_ACS = 45;
    stateMachine71();
    TEST_ASSERT_EQUAL(7, Estado_Maquina);

    resetFixtures();
    Estado_Maquina = 71;
    Temp_ACS = 40;
    Temp_out_H = 50;
    SetP_ACS = 45;
    TEST_ASSERT_EQUAL_FLOAT(40.0, Temp_ACS);
    TEST_ASSERT_EQUAL_FLOAT(50.0, Temp_out_H);
    TEST_ASSERT_EQUAL_UINT8(45, SetP_ACS);
    stateMachine71();
    TEST_ASSERT_EQUAL(71, Estado_Maquina);

    resetFixtures();
    Estado_Maquina = 71;
    Ingreso_E71 = 0;
    Temp_ACS = 40;
    Temp_out_H = 50;
    fake_millis_now = 90001;
    stateMachine71();
    TEST_ASSERT_EQUAL(7, Estado_Maquina);

    resetFixtures();
    Estado_Maquina = 71;
    EnableACS = false;
    stateMachine71();
    TEST_ASSERT_EQUAL(7, Estado_Maquina);
}

void test_state_71_heating_off_returns_to_initial_state(void) {
    resetFixtures();
    Estado_Maquina = 71;
    heating_off = true;

    stateMachine71();

    TEST_ASSERT_EQUAL(0, Estado_Maquina);
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Compressor);
}

void test_state_transitions_require_strict_time_boundaries(void) {
    resetFixtures();
    Estado_Maquina = 1;
    senal_start = true;
    EnableACS = false;
    stateMachine1();
    fake_millis_now = 60000;
    stateMachine1();
    TEST_ASSERT_EQUAL(1, Estado_Maquina);
    fake_millis_now = 60001;
    stateMachine1();
    TEST_ASSERT_EQUAL(2, Estado_Maquina);

    resetFixtures();
    Estado_Maquina = 7;
    valvulaACSStart = 0;
    Temp_ACS = 30;
    SetP_ACS = 45;
    fake_millis_now = 15000;
    stateMachine7();
    TEST_ASSERT_EQUAL(LOW, Valor_DO_Bombas);
    fake_millis_now = 15001;
    stateMachine7();
    TEST_ASSERT_EQUAL(HIGH, Valor_DO_Bombas);
}

void test_state_71_does_not_return_before_timeout(void) {
    resetFixtures();
    Estado_Maquina = 71;
    Ingreso_E71 = 0;
    Temp_ACS = 40;
    Temp_out_H = 50;
    fake_millis_now = 90000;
    stateMachine71();
    TEST_ASSERT_EQUAL(71, Estado_Maquina);
    fake_millis_now = 90001;
    stateMachine71();
    TEST_ASSERT_EQUAL(7, Estado_Maquina);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_initial_state_is_safe);
    RUN_TEST(test_start_reaches_pump_state_after_existing_delay);
    RUN_TEST(test_pumps_then_compressor_then_running_state);
    RUN_TEST(test_stop_returns_to_safe_initial_state);
    RUN_TEST(test_alarm_has_priority_in_startup);
    RUN_TEST(test_acs_generation_stops_at_setpoint);
    RUN_TEST(test_initial_state_respects_heating_off_and_cold_mode);
    RUN_TEST(test_state_one_heating_off_returns_to_initial_state);
    RUN_TEST(test_state_one_starts_acs_generation_after_valve_delay);
    RUN_TEST(test_state_one_daily_pump_routine_runs_and_stops);
    RUN_TEST(test_state_two_stop_and_heating_off_are_safe);
    RUN_TEST(test_state_three_handles_alarm_before_stop_and_rest);
    RUN_TEST(test_alarm_state_stops_outputs_and_starts_buzzer_once);
    RUN_TEST(test_rest_state_exits_after_time_or_heating_off);
    RUN_TEST(test_state_seven_starts_pumps_and_compressor_after_delays);
    RUN_TEST(test_state_seven_stops_for_disable_target_or_heating_off);
    RUN_TEST(test_state_seven_enters_71_for_temperature_safety_or_alarm);
    RUN_TEST(test_state_71_returns_to_generation_for_each_release_condition);
    RUN_TEST(test_state_71_heating_off_returns_to_initial_state);
    RUN_TEST(test_state_transitions_require_strict_time_boundaries);
    RUN_TEST(test_state_71_does_not_return_before_timeout);
    UNITY_END();
    return 0;
}
