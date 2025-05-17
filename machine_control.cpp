#include "machine_control.h"

const float MAX_TEMP_OUT_H_HEATING = 42.0;
const float MIN_TEMP_OUT_H_COOLING = 10.0;
const float MIN_TEMP_OUT_H_HEATING = -2.0;
const float MAX_TEMP_OUT_T = 40.0;
const float MIN_TEMP_OUT_T = -6.0;
const float MIN_TEMP_ADMISION = -7.0;
const uint8_t MAX_ACS = 48;
const uint8_t MIN_ACS = 30;

void frioCalor(bool paramModoFrio)  // Función de cambio de Modo de Funcionamiento  (Bromberg: modo frio = valvula de 4 vias APAGADA)
{
    lcd.clear();
    lcd.print(F("POR FAVOR, ESPERE"));
    lcd.setCursor(0, 2);
    lcd.print(F("CAMBIANDO MODO..."));

    modoFrio = paramModoFrio;
    Valor_DO_V4V = modoFrio ? LOW /* modo frio*/ : HIGH /* modo calor*/;

    MenuActual = 20;
    MenuDosCero();
}

void changeModo(bool paramModoFrio) {
    if (Estado_Maquina == 1) {
        frioCalor(paramModoFrio);
        EEPROMwrite(modoFrio_address, modoFrio);
    }
}

void setupDigitalInputs() {
    pinMode(DI_Marcha_on, INPUT);
    pinMode(DI_Pres_HI, INPUT);
    pinMode(DI_Pres_LOW, INPUT);
    pinMode(DI_Caud_T, INPUT);
    pinMode(DI_Caud_H, INPUT);
}

void setupDigitalOuputs() {
    pinMode(DO_Compressor, OUTPUT);
    pinMode(DO_ValvulaACS, OUTPUT);
    pinMode(DO_Bombas, OUTPUT);
    pinMode(DO_Calentador, OUTPUT);
    pinMode(DO_Valvula4Vias, OUTPUT);

    pinMode(DO_Triac_01, OUTPUT);
    pinMode(DO_Buzzer, OUTPUT);
}

void initializeDigitalOuputs() {
    digitalWrite(DO_Compressor, LOW);
    digitalWrite(DO_ValvulaACS, LOW);  // Inicia con paso a loza encendida
    digitalWrite(DO_Valvula4Vias, LOW);
    digitalWrite(DO_Bombas, LOW);
    digitalWrite(DO_Calentador, LOW);
    digitalWrite(DO_Triac_01, LOW);
    digitalWrite(DO_Buzzer, LOW);
}

void writeOutput() {
        // IMAGEN DE SALIDAS
        digitalWrite(DO_Bombas, Valor_DO_Bombas);
        digitalWrite(DO_Calentador, Valor_DO_Calentador);
        digitalWrite(DO_Valvula4Vias, Valor_DO_V4V);
        digitalWrite(DO_Compressor, Valor_DO_Compressor);
        digitalWrite(DO_ValvulaACS, Valor_DO_VACS);
        if (Estado_Maquina != 4) {
            digitalWrite(DO_Buzzer, Valor_DO_Buzzer);
        }    
}

void refreshDataToShow() {
    if (millis() - Periodo_Refresco > 500) {
        temperatureCalculation();

        lcdRefreshValues();

        // menuActivo->refresh();

        Periodo_Refresco = millis();
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

  digitalRead(DI_Marcha_on) == HIGH
  modoFrio

  */

    senal_start = ((digitalRead(DI_Marcha_on) == HIGH) && !modoFrio) || ((digitalRead(DI_Marcha_on) == LOW) && modoFrio);

    senal_stop = ((digitalRead(DI_Marcha_on) == LOW) && !modoFrio) || ((digitalRead(DI_Marcha_on) == HIGH) && modoFrio);
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
    return ((millis() - Ingreso_E3) > 120000) && !modoFrio &&
           ((Temp_out_H > MAX_TEMP_OUT_H_HEATING)     // control por temp losa
            || (Temp_out_H < MIN_TEMP_OUT_H_HEATING)  // condicion de arranque en invierno
            || (Temp_out_T < MIN_TEMP_OUT_T)          // condicion de corte
            || (Temp_Admision < MIN_TEMP_ADMISION)    // condicion de corte
           );
}

bool coolingCheck() {
    return modoFrio && ((Temp_out_H < MIN_TEMP_OUT_H_COOLING)  // condicion de corte
                        || (Temp_out_T > MAX_TEMP_OUT_T)       // condicion de corte
                       );
}

bool longPeriodRunningCheck() {
    return ((millis() - Ingreso_E3) > 43200000);  // 12 horas
}

void takeRestControl() {
    if (heatingCheck() || coolingCheck() || longPeriodRunningCheck()) {
        Estado_Maquina = 6;
        Ingreso_Descanso = millis();
    }
}

void buzzerStop(bool force) {
    // este estado maneja una rutina para que las bombas se usen diariamente.
    if (Estado_Maquina == 1 && Valor_DO_Bombas == HIGH && !force) return;
    // este estado maneja el bip con PWM
    if (Estado_Maquina == 4 && !force) return;
    if ((millis() - BuzzerStart > 150) || force) {
        Valor_DO_Buzzer = LOW;
        BuzzerStart = 0;
    }
}

void buzzerStart() {
    // este estado maneja una rutina para que las bombas se usen diariamente.
    if (Estado_Maquina == 1 && Valor_DO_Bombas == HIGH) return;
    // este estado maneja el bip con PWM
    if (Estado_Maquina == 4) return;
    Valor_DO_Buzzer = HIGH;
    BuzzerStart = millis();
}
