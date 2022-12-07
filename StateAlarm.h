
#ifndef alarmstate__
#define alarmstate__
#include "Machine.h"
#include "Messages.h"

class AlarmState : public State {

    Messages messages;

    public:
    AlarmState(Machine* _machine) {
        this->machine = _machine;
        this->init();
    }

    void init() { }

    void checkHealtSystem() { }

    void handle() {
        this->machine->outputState->setDOBombas(LOW);
        this->machine->outputState->setDOCompressor(LOW);
        //triac -> low
        //Activar alarma "i'm here!!!"

        //chequear flag y determinar nro de alarma.
        //    Escribir en la eprom

            //lanzar el buzzer
                //        Timer1.pwm(DO_Buzzer, 100, 1000000);

        if (this->machine->heatingOff)
            this->machine->setState(this->machine->RESET_STATE);
    }
};

#endif
