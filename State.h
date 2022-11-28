
#ifndef state__
#define state__
#include "vars.h"

class Machine;

class State {

        protected:
        Machine* machine;
        bool flagStartSignal;
        unsigned long dontStuckPpumpsDeactivationMillis;
        unsigned long compressorActivationMillis;
        unsigned long valveSwitchActivationMillis;

        public:
        State();
        State(Machine* _machine) {
                this->machine = _machine;
        }

        virtual void handle();

};

#endif
