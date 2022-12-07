
#ifndef thermostat__
#define thermostat__
#include "vars.h"
#include "Pinout.h"

class Thermostat {

    Pinout* pinout;
    volatile bool* modoFrio;

    public:

    Thermostat(Pinout* _pinout, volatile bool* _modoFrio) {
        pinout = _pinout;
        modoFrio = _modoFrio;
    };

    bool isOn();
    bool isOff();
};


#endif
