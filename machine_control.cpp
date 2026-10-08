#include "machine_control.h"

const float MAX_TEMP_OUT_H_HEATING = 42.0;
const float MIN_TEMP_OUT_H_COOLING = 10.0;
const float MIN_TEMP_OUT_H_HEATING = -2.0;
const float MAX_TEMP_OUT_T = 40.0;
const float MIN_TEMP_OUT_T = -6.0;
const float MIN_TEMP_ADMISION = -7.0;
const uint8_t MAX_ACS = 48;
const uint8_t MIN_ACS = 30;
const unsigned long MODE_CHANGE_LOCKOUT_MS = 3000;

unsigned long lastModeChangeAt = 0;

void frioCalor(bool paramModoFrio)  // Función de cambio de Modo de Funcionamiento  (Bromberg: modo frio = valvula de 4 vias APAGADA)
{
    modoFrio = paramModoFrio;
    valorDoV4v = modoFrio ? LOW /* modo frio*/ : HIGH /* modo calor*/;

}

void changeModo(bool paramModoFrio) {
    if (estadoMaquina != 1 || paramModoFrio == modoFrio || isModeChangeLocked()) {
        return;
    }

    frioCalor(paramModoFrio);
    eepromWrite(modoFrioAddress, modoFrio);
    lastModeChangeAt = millis();
}

bool isModeChangeLocked() {
    return lastModeChangeAt != 0 && millis() - lastModeChangeAt < MODE_CHANGE_LOCKOUT_MS;
}

void setupDigitalInputs() {
    pinMode(diMarchaOn, INPUT);
    pinMode(diPresHi, INPUT);
    pinMode(diPresLow, INPUT);
    pinMode(diCaudT, INPUT);
    pinMode(diCaudH, INPUT);
}

void setupDigitalOuputs() {
    pinMode(doCompressor, OUTPUT);
    pinMode(doValvulaAcs, OUTPUT);
    pinMode(doBombas, OUTPUT);
    pinMode(doCalentador, OUTPUT);
    pinMode(doValvula4Vias, OUTPUT);

    pinMode(doTriac01, OUTPUT);
    pinMode(doBuzzer, OUTPUT);
}

void initializeDigitalOuputs() {
    digitalWrite(doCompressor, LOW);
    digitalWrite(doValvulaAcs, LOW);  // Inicia con paso a loza encendida
    digitalWrite(doValvula4Vias, LOW);
    digitalWrite(doBombas, LOW);
    digitalWrite(doCalentador, LOW);
    digitalWrite(doTriac01, LOW);
    digitalWrite(doBuzzer, LOW);
}

void writeOutput() {
        // IMAGEN DE SALIDAS
        digitalWrite(doBombas, valorDoBombas);
        digitalWrite(doCalentador, valorDoCalentador);
        digitalWrite(doValvula4Vias, valorDoV4v);
        digitalWrite(doCompressor, valorDoCompressor);
        digitalWrite(doValvulaAcs, valorDoVacs);
        // El buzzer se controla con tone() para los avisos breves y con
        // Timer1.pwm() durante una alarma. No escribir el pin aquí: esa
        // escritura interrumpe el tono en cada vuelta del loop.
}

void refreshDataToShow() {
    if (millis() - periodoRefresco > 500) {
        temperatureCalculation();

        lcdRefreshValues();

        periodoRefresco = millis();
    }
}

void calculateStartStopSignal() {
    /*
  Si modo calor:
    di marcha on == HIGH -> arrancar
    di marcha on == LOW -> parar
  Si modo frio:
    di marcha on == LOW -> arrancar
    di marcha on == HIGH -> parar

  digitalRead(diMarchaOn) == HIGH
  modoFrio

  */

    senalStart = ((digitalRead(diMarchaOn) == HIGH) && !modoFrio) || ((digitalRead(diMarchaOn) == LOW) && modoFrio);

    senalStop = ((digitalRead(diMarchaOn) == LOW) && !modoFrio) || ((digitalRead(diMarchaOn) == HIGH) && modoFrio);
}

uint8_t normalizeAcsTemp(volatile uint8_t* acsValue) {
    if (*acsValue < MIN_ACS) {
        *acsValue = MIN_ACS;
    }
    if (*acsValue > MAX_ACS) {
        *acsValue = MAX_ACS;
    }
    return *acsValue;
}

bool heatingCheck() {
    // le damos tiempo a que las bombas funcionen antes controlar
    // para que si viene de calentar agua, circule el agua caliente.
    return ((millis() - ingresoE3) > 120000) && !modoFrio &&
           ((tempOutH > MAX_TEMP_OUT_H_HEATING)     // control por temp losa
            || (tempOutH < MIN_TEMP_OUT_H_HEATING)  // condicion de arranque en invierno
            || (tempOutT < MIN_TEMP_OUT_T)          // condicion de corte
            || (tempAdmision < MIN_TEMP_ADMISION)    // condicion de corte
           );
}

bool coolingCheck() {
    return modoFrio && ((tempOutH < MIN_TEMP_OUT_H_COOLING)  // condicion de corte
                        || (tempOutT > MAX_TEMP_OUT_T)       // condicion de corte
                       );
}

bool longPeriodRunningCheck() {
    return ((millis() - ingresoE3) > 43200000);  // 12 horas
}

void takeRestControl() {
    if (heatingCheck() || coolingCheck() || longPeriodRunningCheck()) {
        estadoMaquina = 6;
        ingresoDescanso = millis();
    }
}

void buzzerControl() {
    if (estadoMaquina == 4) return;
    if (flagBuzzer) {
        buzzerBip();
        flagBuzzer = false;
    }
}

void buzzerStop() {
    flagBuzzer = false;
    valorDoBuzzer = LOW;
    noTone(doBuzzer);
    Timer1.disablePwm(doBuzzer);
    digitalWrite(doBuzzer, LOW);
}

void buzzerBip() {
    tone(doBuzzer, 1500, 150);
}
