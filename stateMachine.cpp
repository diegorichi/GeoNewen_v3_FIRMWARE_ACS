#include "stateMachine.h"

const float MAX_TEMP_OUT_H_ACS = 50.0;

unsigned long E1_a_E2 = 60000;  // 1 minuto para pasar de E1 a E2
unsigned long E2_a_E3 = 15000;  // 15 segundos para pasar de E2 a E3

void initializeStateMachine() {
    Estado_Maquina = 0;
    compressorStart = 0;
    dontStuckPumpsStart_activation = 0;
    dontStuckPumpsStart = 0;
    Valor_DO_VACS = LOW;
    Valor_DO_V4V = LOW;
    modoFrio = false;
    heating_off = false;
}

// Estado inicial del sistema, tanto el compresor como las bombas de circulación están apagados
void stateMachine0() {
    if (Estado_Maquina == 0) {
        Flag_Marcha_ON = false;
        Valor_DO_Compressor = LOW;
        compressorStart = 0;
        Valor_DO_Bombas = LOW;
        PumpStart = 0;
        dontStuckPumpsStart = millis();

        Valor_DO_VACS = LOW;  // paso a losa radiante
        valvulaACSStart = millis();

        Valor_DO_V4V = modoFrio ? LOW /* modo frio*/ : HIGH /* modo calor*/;

        if (!heating_off)
            Estado_Maquina = 1;
    }
}

// Aquí se espera la señal de Marcha_ON para iniciar la operacion del sistema
void stateMachine1() {
    if (Estado_Maquina == 1) {
        if (heating_off) {
            Estado_Maquina = 0;
            return;
        }

        // rutina para activar las bombas una vez por dia durante 10 segundos, para evitar daños por inactividad (86400000)
        if ((millis() - dontStuckPumpsStart) > 86400000) {
            buzzerStart();
            Valor_DO_Bombas = HIGH;
            dontStuckPumpsStart_activation = millis();
        }
        if ((millis() - dontStuckPumpsStart_activation) > 10000) {
            Valor_DO_Bombas = LOW;
            dontStuckPumpsStart = millis();
            buzzerStop(true);
        }

        if ((millis() - valvulaACSStart) > 15000) {
            if GENERATE_ACS {
                // la generacion de ACS requiere que la valv 4v este activa
                Valor_DO_VACS = HIGH;
                // abre la valvula de 4v para calentar el agua
                Valor_DO_V4V = HIGH;
                valvulaACSStart = millis();
                Estado_Maquina = 7;  // Generacion ACS
                Ingreso_E7 = millis();
            }
        }

        if (senal_start && !Flag_Marcha_ON) {
            Salto_E1 = millis();
            Flag_Marcha_ON = true;
        }

        if (senal_stop && Flag_Marcha_ON) {
            Flag_Marcha_ON = false;
        }

        // E1_E2 no puede ser menor a 15000 ya que si no viola la condicion
        //     if ((millis() - valvulaACSStart) > 15000)
        // Se espera un tiempo para que abran las electrovalvulas de la loza radiante
        if ((millis() - Salto_E1 > E1_a_E2) && senal_start) {
            Estado_Maquina = 2;
        }
    }
}

// Arranque Compresor y Bombas
void stateMachine2() {
    if (Estado_Maquina == 2) {
        if (heating_off || senal_stop) {
            Estado_Maquina = 0;
            return;
        }

        if (Valor_DO_Bombas == LOW) {
            Valor_DO_Bombas = HIGH;
            PumpStart = millis();
        }

        if (millis() - PumpStart > 25000) {
            if (Valor_DO_Compressor == LOW) {
                Valor_DO_Compressor = HIGH;
                compressorStart = millis();
            }

            // Transcurrido un cierto tiempo, se avanza al siguiente estado
            if (millis() - compressorStart > E2_a_E3) {
                Estado_Maquina = 3;
                Ingreso_E3 = millis();
            }
        }
    }
}

// Este es el estado final del sistema, donde se controlan las condiciones de alarma
void stateMachine3() {
    if (Estado_Maquina == 3) {
        /* Si hay que generar ACS, pasamos por es estado 0 */
        if ((heating_off || senal_stop) || GENERATE_ACS) {
            Estado_Maquina = 0;
            return;
        }

        // Condiciones de Apagado del Compresor
        checkFlagsForAlarms();

        takeRestControl();
    }
}

// Estado de Alarma
void stateMachine4() {
    if (Estado_Maquina == 4) {
        Valor_DO_Compressor = LOW;
        compressorStart = 0;
        Valor_DO_Bombas = LOW;
        PumpStart = 0;
        digitalWrite(DO_Triac_01, LOW);

        if (!Alarma_Activa) {
            ConvertFlagToAlarm();
            Alarma_Activa = true;
        }
        if (Nro_Alarma != 0) {
            if (!Flag_Buzzer) {
                Timer1.pwm(DO_Buzzer, 100, 1000000);
                Flag_Buzzer = true;
            }
        }
        wdt_reset();
    }
}

// Estado de descanso
void stateMachine6() {
    if (Estado_Maquina == 6) {
        Valor_DO_Compressor = LOW;
        Valor_DO_Bombas = LOW;
        // una vez en el descanso, se espera antes de enviar el sistema al estado inicial
        //  6 min
        if ((millis() - Ingreso_Descanso > 400000) || heating_off) {
            Estado_Maquina = 0;
            return;
        }
    }
}

// Generacion ACS
void stateMachine7() {
    if (Estado_Maquina == 7) {
        if (heating_off) {
            Estado_Maquina = 0;
            return;
        }

        if (millis() - valvulaACSStart > 15000) {
            // solo prendo las bombas si paso el tiempo para abrir las valuvlas
            Valor_DO_Bombas = HIGH;  // se mantiene HIGH hasta que se va a estado 0 (puede pasar por 71 u 8)
            PumpStart = millis();
        }

        if ((millis() - Ingreso_E7) > 30000) {
            checkFlagsForAlarms();
            if (Temp_in_H >= Temp_ACS - 2) {
                Valor_DO_Compressor = HIGH;
            }
        }

        // se le da energia al ACS de a saltos para evitar pasar de presion y temperatura el circuito de gas
        if (Temp_out_H > MAX_TEMP_OUT_H_ACS || Temp_Descarga > 80.0) {
            Estado_Maquina = 71;
            Ingreso_E71 = millis();
        }

        // termino el ciclo en modo calor y vuelve a 0
        if ((Temp_ACS >= SetP_ACS) || !EnableACS) {
            Estado_Maquina = 0;
        }
    }
}

void stateMachine71()  // Generacion ACS: Estado con bombas andando y compresor apagado
{
    if (Estado_Maquina == 71) {
        if (heating_off) {
            Estado_Maquina = 0;
            return;
        }

        Valor_DO_Compressor = LOW;

        /*
        Ahora lo hace por tiempo, pero:
        Debe volver cuando se haya transferido la temperatura
        y esten casi igualadas, o con un gap minimo
        en ese momento si no se alcanzo la temperatura deseada,
        se debe volver a 7 para prender el compresor
        */
        if ((Temp_ACS > SetP_ACS) || (Temp_ACS > (Temp_out_H - GAP_ACS)) || ((millis() - Ingreso_E71) > 90000) || !EnableACS) {
            Estado_Maquina = 7;
            Ingreso_E7 = millis();
        }
    }  // FIn Estado 71
}