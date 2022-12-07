
#ifndef acsstate__
#define acsstate__
#include "Machine.h"

class ACSState : public State {
    private:
    long acsGenerationStartMillis = 0;

    public:
    ACSState(Machine* _machine) {
        this->machine = _machine;
        init();
    }
    void init() {
        acsGenerationStartMillis = 0;
    }

    void start() {
        if (acsGenerationStartMillis == 0) {
            acsGenerationStartMillis = millis();
        }
    };
    void stop() {
        acsGenerationStartMillis = 0;
        this->machine->setState(this->machine->RESET_STATE);
    };

    void checkHealtSystem() {
        if (millis() - acsGenerationStartMillis > this->machine->configuration->DELAY_SYSTEM_HEALT_CHECK) {
            //check alarms
        };
    };

    void handle() {
        start();
        if (!this->machine->heatingOff
            //|| acs_off
            || this->machine->acsHandler->shouldStopGenerateACS())
            stop();

        this->machine->outputState->setDOBombas(HIGH);
        this->machine->outputState->setDOV4V(HIGH); // abre la valvula de 3v para calentar el agua
        this->machine->outputState->setDOVACS(HIGH);// la generacion de ACS requiere que la valv 4v este activa

        if (millis() - acsGenerationStartMillis > this->machine->configuration->DELAY_START_COMPRESSOR) {
            this->machine->outputState->setDOCompressor(HIGH);
        };

        //pasar a estado 71

        // finalizar periodo de ACS
        //  A) modo calor
        //  B) modo frio

    };
};

#endif
