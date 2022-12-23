
#ifndef takereststate__
#define takereststate__
#include "Machine.h"

class TakeRestState : public State {
    private:
    bool started;
    long startTakingRestMillis;

    public:
    TakeRestState(Machine* _machine) {
        this->machine = _machine;
        this->init();
    }

    void init() {
        startTakingRestMillis = 0;
    }

    void start() {
        startTakingRestMillis = millis();
    }

    void stop() {
        init();
        this->machine->setState(this->machine->RESET_STATE);
    }

    void checkHealtSystem() { }

    void handle() {
        this->machine->outputState->setDOBombas(LOW);
        this->machine->outputState->setDOCompressor(LOW);

        if (this->machine->heatingOff)
            this->machine->setState(this->machine->RESET_STATE);

        if (startTakingRestMillis == 0) start();

        // se espera antes de enviar el sistema al estado inicial
        if ((millis() - startTakingRestMillis > this->machine->configuration->TAKE_REST_DURATION)) {
            stop();
        }
    }
};

#endif
