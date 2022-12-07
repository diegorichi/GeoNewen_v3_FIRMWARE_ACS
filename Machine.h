
#ifndef machine__
#define machine__
#include "vars.h"
#include "Keyboard_.h"
#include "SerialEsp8266.h"
#include "Menu.h"
#include "OutputState.h"
#include "State.h"
#include "Pinout.h"
#include "ACSHandler.h"
#include "BuzzerHandler.h"

class Thermostat;
class Machine {
    public:

    static State* RESET_STATE;
    static State* WAITING_SIGNAL_STATE;
    static State* START_HEATING_STATE;
    static State* ACS_STATE;
    static State* ALARM_STATE;
    static State* TAKE_REST_STATE;

    SerialEsp8266* esp8266;
    Pinout* pinout;
    Configuration* configuration;
    Menu* menuActivo;
    OutputState* outputState;
    State* state;
    //LcdManagement* lcdManagement;
    BuzzerHandler* buzzerHandler;
    ACSHandler* acsHandler;
    Thermostat* thermostat;
    KumeKeyboard* keyboard;


    bool heatingOff;
    volatile bool modoFrio;

    void init();
    void run();

    void setState(State* newState) {
        this->state = newState;
    }

};

#include "StateStartHeating.h"
#include "StateWaitingSignal.h"
#include "StateReset.h"
#include "StateACS.h"
#include "StateTakeRest.h"
#include "StateAlarm.h"
#include "Thermostat.h"

void Machine::init() {
    RESET_STATE = new ResetState(this);
    WAITING_SIGNAL_STATE = new WaitingSignalState(this);
    START_HEATING_STATE = new StartHeatingState(this);
    TAKE_REST_STATE = new TakeRestState(this);
    ALARM_STATE = new AlarmState(this);
    ACS_STATE = new ACSState(this);


    this->pinout = new Pinout();
    this->esp8266 = new SerialEsp8266(&Serial);
    this->menuActivo = new Menu();

    this->acsHandler = new ACSHandler(this);
    this->outputState = new OutputState(this->pinout);
    this->buzzerHandler = new BuzzerHandler(this->outputState,
        this->configuration, this->pinout);
    this->keyboard = new KumeKeyboard(this->pinout, this->menuActivo,
        this->configuration, this->buzzerHandler);

    this->heatingOff = false;
    this->modoFrio = false;
    this->thermostat = new Thermostat(this->pinout, &this->modoFrio);


}

void Machine::run() {

    //medir temperaturas
    //calcular flujos
    //revisar control por serial
    //refrescar menu

    this->state->checkHealtSystem();

    this->acsHandler->handleAuxiliaryACSHeating();

    this->state->handle();

    this->outputState->writeDigitalOuputs();
}
#endif


/*
Activacion_Comp
dont_stuck_pumps_activation: 10000
dont_stuck_pumps 1 dia: 86400000
modoFrio
heating_off
Flag_Marcha_ON
EsperaValv : espera para chequear generacion de acs
Ingreso_E7 // usado para checkear alarmas
senal_start
senal_stop
Salto_E1 : cuando viene señal de start,
                se espera E1_a_E2 (2min) para dar marcha

Activacion_Comp, E2_a_E3 (10 segs):
                En estado 2 se activa el compresor
                y se espera E2_a_E3 para pasar a E3

Ingreso_E3 // usado para checkear alarmas
Ingreso_Descanso
Nro_Alarma
Alarma_Activa
Flag_Alarma_General
Flag_Buzzer
Periodo_Fin_ACS // usado para saber cuando salir de estado 8
*/



/*
Periodo_Refresco_LCD


LecturaDSB -> periodo lectura sensores
                -> delay para toma de datos (200ms)
Ventana_Caudal_H
Ventana_Caudal_T


Periodo_Refresco_Wifi: envio de datos a esp8266

*/