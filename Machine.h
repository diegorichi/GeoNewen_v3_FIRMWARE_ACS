
#ifndef machine__
#define machine__
#include "vars.h"
#include "Keyboard_.h"
#include "SerialEsp8266.h"
#include "Menu.h"
#include "OutputState.h"
#include "State.h"
#include "Pinout.h"

class Machine {
        public:

        static ResetState* RESET_STATE;
        static WaitingSignalState* WAITING_SIGNAL_STATE;

        SerialEsp8266* esp8266;
        Keyboard* keyboard;
        Pinout* pinout;
        Configuration* configuration;
        Menu* menuActivo;
        OutputState* outputState;
        State* state;
        //ElectricHeating* electricHeating;
        //LcdManagement* lcdManagement;
        //BuzzerHandler* buzzerHandler;

        bool heatingOff;
        volatile bool modoFrio;

        void init();

        void setState(State* newState) {
                this->state = newState;
        }

};

#include "StateReset.h"

void Machine::init() {
        RESET_STATE = new ResetState(this);
        WAITING_SIGNAL_STATE = new WaitingSignalState(this);
        this->pinout = new Pinout();
        this->esp8266 = new SerialEsp8266(&Serial2);
        this->menuActivo = new Menu();
        this->outputState = new OutputState(this->pinout);
        this->heatingOff = false;
        //create menu
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