
#ifndef resetstate__
#define resetstate__
#include "Machine.h"
#include "StateWaitingSignal.h"

class ResetState : public State {

        private:
        State* waitingSignalState;
        State* resetState;

        public:
        ResetState(Machine* _machine) {
                this->machine = _machine;
                this->init();
        }

        void init() {
                this->waitingSignalState = this->machine->WAITING_SIGNAL_STATE;
                this->resetState = this->machine->RESET_STATE;
        }

        void handle() {
                this->machine->outputState->setDOBombas(LOW);
                this->machine->outputState->setDOCompressor(LOW);
                this->machine->outputState->setDOVACS(LOW);
                this->machine->outputState->setDOV4V(this->machine->modoFrio ? LOW /* modo frio*/ : HIGH /* modo calor*/);

                this->flagStartSignal = false;
                this->dontStuckPpumpsDeactivationMillis = millis();
                this->compressorActivationMillis = 0;
                this->valveSwitchActivationMillis = millis();

                if (!this->machine->heatingOff)
                        this->machine->setState(this->resetState);
        }
};

#endif
