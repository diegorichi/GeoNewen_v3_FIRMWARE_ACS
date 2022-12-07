
#ifndef startheatingstate__
#define startheatingstate__
#include "Machine.h"
#include "Thermostat.h"

class StartHeatingState : public State {
    private:
    long heatingStartMillis = 0;

    public:
    StartHeatingState(Machine* _machine) {
        this->machine = _machine;
        this->init();
    }

    void init() {
        heatingStartMillis = 0;
    }

    void start() {
        if (heatingStartMillis == 0) {
            heatingStartMillis = millis();
        }
    };
    void reset() {

    }

    void stop() {
        heatingStartMillis = 0;
        this->machine->setState(this->machine->RESET_STATE);
    };

#define CHECK_TEMPS true
    void checkHealtSystem() {
        if (millis() - heatingStartMillis > this->machine->configuration->DELAY_SYSTEM_HEALT_CHECK) {
            //check alarms
        };

        // checkear condiciones de descanso
        if (CHECK_TEMPS) {
            reset();
            this->machine->setState(this->machine->TAKE_REST_STATE);
        }

    };

    void handle() {
        start();
        this->machine->outputState->setDOBombas(HIGH);

        if (millis() - heatingStartMillis > this->machine->configuration->DELAY_START_COMPRESSOR) {
            this->machine->outputState->setDOCompressor(HIGH);
        };

        if (this->machine->heatingOff
            || this->machine->thermostat->isOff()
            || this->machine->acsHandler->shouldGenerateACS()
            )
            stop();

    }
};

#endif
