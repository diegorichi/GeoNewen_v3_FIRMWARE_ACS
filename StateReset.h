
#ifndef resetstate__
#define resetstate__
#include "Machine.h"

class ResetState : public State {

        public:
        ResetState(Machine* _machine) {
                this->machine = _machine;
                this->init();
        }

        void init() {
        }
        
        void checkHealtSystem() {}

        void handle() {
                this->machine->outputState->setDOBombas(LOW);
                this->machine->outputState->setDOCompressor(LOW);
                this->machine->outputState->setDOVACS(LOW);
                this->machine->outputState->setDOV4V(this->machine->modoFrio ? LOW /* modo frio*/ : HIGH /* modo calor*/);

                if (this->machine->heatingOff)
                    this->machine->setState(this->machine->RESET_STATE);
                else {
                    this->machine->setState(this->machine->WAITING_SIGNAL_STATE);
                }
        }
};

#endif
