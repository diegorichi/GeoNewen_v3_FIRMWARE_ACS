#include "SerialEsp8266.h"
#include "functionsLCDMenu.h"
#include "keyboard.h"
#include "kume_eeprom.h"
#include "machine_control.h"
#include "measurement_and_calculations.h"
#include "stateMachine.h"
#include "vars.h"
/*******/
/*SETUP*/
/*******/

SerialEsp8266 esp8266(&Serial2);

void setup() {  // Inicializacion de I/O y variables generales

    wdt_disable();

    keyboardSetup();  // setup de pin modes y demas

    setupDigitalInputs();  // setup de los pines de entrada

    setupDigitalOuputs();  // setup de los pines de salida

    initializeDigitalOuputs();

    attachInterrupt(1, AtencionTeclado, FALLING);
    // Asignación de Interrupciones (se define el número de la interrupción, no del pin; la rutina de interrupción y el modo de activación)

    lcdCreateSpecialChars();

    initializeFlowState();

    initializeStateMachine();

    ResetFlags();

    Timer1.initialize(100000);

    Serial.begin(115200);

    EEPROMLectura();  // Carga parametros guardados en la memoria EEPROM

    SetP_ACS_Edit = normalizeAcsTemp(&SetP_ACS);

    MenuActual = 0;
    MenuCero();
    // initializeAndSetupMenu();

    // menuActivo->show();

    wdt_enable(WDTO_8S);
}

void mainLoop() {
    // CÁLCULO DE TEMPERATURAS, CAUDALES, EFICIENCIA TÉRMICA Y CONSUMO DE ENERGÍA

    temperatureMeasurement();

    flowsCalculation();  // Determinación de Caudal

    esp8266.handleEspSerial();

    wdt_reset();

    // INFORMACIÓN A REFRESCAR (depende del menú en el que nos encontremos)
    refreshDataToShow();

    buzzerStop(false);

    //***************************
    // COMPROBACIONES DE SEGURIDAD (Aquí se determinan las posibles causas de alarmas
    //***************************

    // Control de Caudales
    flowControl();

    // Control de Temperaturas
    temperatureControl();

    // Comprobación de Presiones
    presureControl();

    // Control de calentamiento ACS auxiliar con cartucho electrico.
    auxiliaryACSHeatingControl();

    // determina si la bomba tiene senal de marcha del termostato
    calculateStartStopSignal();

    // TRANSICIÓN DE ESTADOS  //Definición del funcionamiento del equipo
    stateMachine0();   // Estado inicial del sistema, tanto el compresor como las bombas de circulación están apagados
    stateMachine1();   // Aquí se espera la señal de Marcha_ON para iniciar la operacion del sistema
    stateMachine2();   // Arranque Compresor y Bombas
    stateMachine3();   // Este es el estado final del sistema, donde se controlan las condiciones de alarma
    stateMachine4();   // Estado de Alarma
    stateMachine6();   // Estado de descanso
    stateMachine7();   // Generacion ACS
    stateMachine71();  // Generacion ACS: Estado con bombas andando y compresor apagado

    // IMAGEN DE SALIDAS
    digitalWrite(DO_Bombas, Valor_DO_Bombas);
    digitalWrite(DO_Calentador, Valor_DO_Calentador);
    digitalWrite(DO_Valvula4Vias, Valor_DO_V4V);
    digitalWrite(DO_Compressor, Valor_DO_Compressor);
    digitalWrite(DO_ValvulaACS, Valor_DO_VACS);
    if (Estado_Maquina != 4) {
        digitalWrite(DO_Buzzer, Valor_DO_Buzzer);
    }

    timer_things.tick();
    wdt_reset();
}

/***************/
/*LAZO INFINITO*/
/***************/

void loop() {
    mainLoop();
}  // Fin del loop
