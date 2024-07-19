#include "machine_control.h"

const float MAX_TEMP_OUT_H_HEATING = 37.0;
const float MIN_TEMP_OUT_H_COOLING = 10.0;
const float MIN_TEMP_OUT_H_HEATING = -1.0;
const float MAX_TEMP_OUT_T = 40.0;
const float MIN_TEMP_OUT_T = -6.0;
const float MIN_TEMP_ADMISION = -7.0;
const uint8_t MAX_ACS = 48;
const uint8_t MIN_ACS = 30;

void frioCalor()  // Función de cambio de Modo de Funcionamiento  (Bromberg: modo frio = valvula de 4 vias APAGADA)
{
    lcd.clear();
    lcd.print(F("POR FAVOR, ESPERE"));
    lcd.setCursor(0, 2);
    lcd.print(F("CAMBIANDO MODO..."));
    for (unsigned long i = 0; i < 3000000; i++)  // Se espera un cierto tiempo antes de activar la válvula de 4 vías
    {
        wdt_reset();
    }
    if (modoFrio)  // Se activa la válvula
    {
        Valor_DO_V4V = HIGH;
        modoFrio = false;
    } else {
        Valor_DO_V4V = LOW;
        modoFrio = true;
    }

    for (unsigned long i = 0; i < 200000; i++) {
        wdt_reset();
    }
    MenuActual = 20;
    MenuDosCero();
}

void changeModo() {
    if (Estado_Maquina == 1) {
        frioCalor();
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
    return ((millis() - Ingreso_E3) > 30000) && !modoFrio &&
           ((Temp_out_H > MAX_TEMP_OUT_H_HEATING)     // condicion de corte
            || (Temp_out_H < MIN_TEMP_OUT_H_HEATING)  // condicion de arranque
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
    if (
        (millis() - BuzzerStart > 300 && !(Estado_Maquina == 1 && Valor_DO_Bombas == HIGH)) ||
        (Estado_Maquina == 1 && Valor_DO_Bombas == LOW) ||
        force) {
        Valor_DO_Buzzer = LOW;
        BuzzerStart = 0;
    }
    if (Estado_Maquina == 4) {
        // este estado maneja el bip con PWM
    }
}

void buzzerStart() {
    Valor_DO_Buzzer = HIGH;
    BuzzerStart = millis();
}
