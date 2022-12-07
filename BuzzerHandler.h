
#ifndef buzzer_handler__
#define buzzer_handler__
#include "vars.h"
#include "Pinout.h"
#include "Configuration.h"
#include "OutputState.h"
#include <TimerOne.h>

class BuzzerHandler {
    private:
    static Timer<2, millis> timer_buzzer;

    static bool stopBeepStatic(void*) {
        output->setDOBuzzer(LOW);
        return false;
    }

    static bool cancelTimer(void*) {
        timer_buzzer.cancel();
        return false;
    }
    public:
    Pinout* pinoutConfiguration;
    static OutputState* output;
    Configuration* configuration;



    BuzzerHandler(OutputState* _output, Configuration* configuration,
        Pinout* pinout) {
        output = _output;
        pinoutConfiguration = pinout;
        configuration = configuration;
    };

    bool stopBeep(void*) {
        output->setDOBuzzer(LOW);
        return false;
    }

    bool startBeep(void*) {
        output->setDOBuzzer(HIGH);
        return false;
    }

    void init() { };

    void beep(long duration) {
        output->setDOBuzzer(HIGH);
        timer_buzzer.in(duration, stopBeepStatic);
    }

    void beep() {
        output->setDOBuzzer(HIGH);
        timer_buzzer.in(configuration->BEEP_LONG, stopBeepStatic);
    }

    void beepEveryDuring(long every, long during) {
        timer_buzzer.every(every, [ ](void*) -> bool {
            output->setDOBuzzer(HIGH);
            return true;
            });

        timer_buzzer.in(during, cancelTimer);
    }
};

#endif
