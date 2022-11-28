
#ifndef waiting_sginal_state__
#define waiting_sginal_state__
#include "Machine.h"

class WaitingSignalState : public State {

        private:
        State* resetState;

        public:
        WaitingSignalState(Machine* _machine) {
                this->machine = _machine;
                this->init();
        }

        void init() {

        }

        void handle() {
                this->machine->outputState->setDOBombas(LOW);
                this->machine->outputState->setDOVACS(LOW);
                this->machine->outputState->setDOV4V(this->machine->modoFrio ? LOW /* modo frio*/ : HIGH /* modo calor*/);


                if (!this->machine->heatingOff)
                        this->machine->setState(this->resetState);
        }
};

#endif
