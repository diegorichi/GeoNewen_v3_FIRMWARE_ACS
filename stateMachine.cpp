#include "stateMachine.h"

const float MAX_TEMP_OUT_H_ACS = 50.0;

unsigned long e1ToE2 = 60000;  // 1 minuto para pasar de E1 a E2
unsigned long e2ToE3 = 15000;  // 15 segundos para pasar de E2 a E3

void initializeStateMachine() {
    estadoMaquina = 0;
    compressorStart = 0;
    dontStuckPumpsStartActivation = 0;
    dontStuckPumpsStart = 0;
    valorDoVacs = LOW;
    valorDoV4v = LOW;
    modoFrio = false;
    heatingOff = false;
}

// Estado inicial del sistema, tanto el compresor como las bombas de circulación están apagados
void stateMachine0() {
    if (estadoMaquina == 0) {
        flagMarchaOn = false;
        valorDoCompressor = LOW;
        compressorStart = 0;
        valorDoBombas = LOW;
        pumpStart = 0;
        dontStuckPumpsStart = millis();

        valorDoVacs = LOW;  // paso a losa radiante
        valvulaAcsStart = millis();

        valorDoV4v = modoFrio ? LOW /* modo frio*/ : HIGH /* modo calor*/;

        if (!heatingOff)
            estadoMaquina = 1;
    }
}

// Aquí se espera la señal de Marcha_ON para iniciar la operacion del sistema
void stateMachine1() {
    if (estadoMaquina == 1) {
        if (heatingOff) {
            estadoMaquina = 0;
            return;
        }

        // rutina para activar las bombas una vez por dia durante 10 segundos, para evitar daños por inactividad (86400000)
        if ((millis() - dontStuckPumpsStart) > 86400000 && dontStuckPumpsStartActivation == 0) {
            buzzerBip();
            valorDoBombas = HIGH;
            dontStuckPumpsStartActivation = millis();
        }
        if ((millis() - dontStuckPumpsStartActivation) > 10000) {
            valorDoBombas = LOW;
            dontStuckPumpsStart = millis();
            dontStuckPumpsStartActivation = 0;
        }

        if ((millis() - valvulaAcsStart) > 15000) {
            if GENERATE_ACS {
                // la generacion de ACS requiere que la valv 4v este activa
                valorDoVacs = HIGH;
                // abre la valvula de 4v para calentar el agua
                valorDoV4v = HIGH;
                valvulaAcsStart = millis();
                estadoMaquina = 7;  // Generacion ACS
                ingresoE7 = millis();
            }
        }

        if (senalStart && !flagMarchaOn) {
            saltoE1 = millis();
            flagMarchaOn = true;
        }

        if (senalStop && flagMarchaOn) {
            flagMarchaOn = false;
        }

        // E1_E2 no puede ser menor a 15000 ya que si no viola la condicion
        //     if ((millis() - valvulaAcsStart) > 15000)
        // Se espera un tiempo para que abran las electrovalvulas de la loza radiante
        if ((millis() - saltoE1 > e1ToE2) && senalStart) {
            estadoMaquina = 2;
        }
    }
}

// Arranque Compresor y Bombas
void stateMachine2() {
    if (estadoMaquina == 2) {
        if (heatingOff || senalStop) {
            estadoMaquina = 0;
            return;
        }

        checkFlagsForAlarms();
        if (estadoMaquina == 4) {
            return;
        }

        if (valorDoBombas == LOW) {
            valorDoBombas = HIGH;
            pumpStart = millis();
        }

        if (millis() - pumpStart > 25000) {
            if (valorDoCompressor == LOW) {
                valorDoCompressor = HIGH;
                compressorStart = millis();
            }

            // Transcurrido un cierto tiempo, se avanza al siguiente estado
            if (millis() - compressorStart > e2ToE3) {
                estadoMaquina = 3;
                ingresoE3 = millis();
            }
        }
    }
}

// Este es el estado final del sistema, donde se controlan las condiciones de alarma
void stateMachine3() {
    if (estadoMaquina == 3) {
        // Las alarmas tienen prioridad sobre apagado, descanso y generación de ACS.
        checkFlagsForAlarms();

        if (estadoMaquina == 4) {
            return;
        }

        /* Si hay que generar ACS, pasamos por este estado 0 */
        if ((heatingOff || senalStop) || GENERATE_ACS) {
            estadoMaquina = 0;
            return;
        }

        takeRestControl();
    }
}

// Estado de Alarma
void stateMachine4() {
    if (estadoMaquina == 4) {
        valorDoCompressor = LOW;
        compressorStart = 0;
        valorDoBombas = LOW;
        pumpStart = 0;
        digitalWrite(doTriac01, LOW);

        if (!alarmaActiva) {
            ConvertFlagToAlarm();
            alarmaActiva = true;
        }
        if (nroAlarma != 0) {
            if (!flagBuzzer) {
                Timer1.pwm(doBuzzer, 100, 1000000);
                flagBuzzer = true;
            }
        }
        wdt_reset();
    }
}

// Estado de descanso
void stateMachine6() {
    if (estadoMaquina == 6) {
        valorDoCompressor = LOW;
        valorDoBombas = LOW;
        // una vez en el descanso, se espera antes de enviar el sistema al estado inicial
        //  6 min
        if ((millis() - ingresoDescanso > 400000) || heatingOff) {
            estadoMaquina = 0;
            return;
        }
    }
}

// Generacion ACS
void stateMachine7() {
    if (estadoMaquina == 7) {
        if (heatingOff) {
            estadoMaquina = 0;
            return;
        }

        if ((tempAcs >= acsSetpoint) || !enableAcs) {
            estadoMaquina = 0;
            return;
        }

        if (millis() - valvulaAcsStart > 15000) {
            // solo prendo las bombas si paso el tiempo para abrir las valuvlas
            valorDoBombas = HIGH;  // se mantiene HIGH hasta que se va a estado 0 (puede pasar por 71 u 8)
            pumpStart = millis();
        }

        if ((millis() - ingresoE7) > 20000) {
            checkFlagsForAlarms();
            if (estadoMaquina == 4) {
                return;
            }
            valorDoCompressor = HIGH;
        }

        // se le da energia al ACS de a saltos para evitar pasar de presion y temperatura el circuito de gas
        if (tempOutH > MAX_TEMP_OUT_H_ACS || tempDescarga > 80.0) {
            estadoMaquina = 71;
            ingresoE71 = millis();
        }

    }
}

void stateMachine71()  // Generacion ACS: Estado con bombas andando y compresor apagado
{
    if (estadoMaquina == 71) {
        if (heatingOff) {
            estadoMaquina = 0;
            return;
        }

        valorDoCompressor = LOW;

        /*
        Ahora lo hace por tiempo, pero:
        Debe volver cuando se haya transferido la temperatura
        y esten casi igualadas, o con un gap minimo
        en ese momento si no se alcanzo la temperatura deseada,
        se debe volver a 7 para prender el compresor
        */
        if ((tempAcs > acsSetpoint) || (tempAcs > (tempOutH - GAP_ACS)) || ((millis() - ingresoE71) > 90000) || !enableAcs) {
            estadoMaquina = 7;
            ingresoE7 = millis();
        }
    }  // FIn Estado 71
}
