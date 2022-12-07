
#ifndef waiting_sginal_state__
#define waiting_sginal_state__
#include "Machine.h"
#include <TimerOne.h>
#include "Thermostat.h"

class WaitingSignalState : public State {
    private:
    static long dontStuckPumps;
    long startHeatingMillis;
    bool started;
    Timer<1, millis> timer_pumps;
    static OutputState* output;

    static bool stopDontStuckPumpsStatic(void*) {
        output->setDOBombas(LOW);
        dontStuckPumps = millis();
        return false;
    }

    public:
    WaitingSignalState(Machine* _machine) {
        this->machine = _machine;
        output = _machine->outputState;
        this->init();
    }

    void init() {
        dontStuckPumps = 0;
        startHeatingMillis = 0;
        started = false;
    }
    void reset() {
        init();
    }

    void stop() {
        init();
        this->machine->setState(this->machine->RESET_STATE);
    }

    void start() {
        if (dontStuckPumps == 0) {
            dontStuckPumps = millis();
        }
        startHeatingMillis = 0;
        started = true;
    }

    void checkHealtSystem() { }

    void handle() {
        this->machine->outputState->setDOBombas(LOW);
        this->machine->outputState->setDOVACS(LOW);
        this->machine->outputState->setDOV4V(this->machine->modoFrio ? LOW /* modo frio*/ : HIGH /* modo calor*/);

        if (this->machine->heatingOff)
            stop();

        if (!started) start();

        // rutina para activar las bombas una vez por dia durante 10 segundos, para evitar daños por inactividad (86400000)
        if ((millis() - dontStuckPumps) > this->machine->configuration->DONT_STUCK_PUMPS_PERIOD) {
            this->machine->outputState->setDOBombas(HIGH);
            this->machine->buzzerHandler->beep(this->machine->configuration->DONT_STUCK_PUMPS_DURATION);
            //lanzar timer para apagar bombas y resetear dontStuckPumps
            timer_pumps.in(this->machine->configuration->DONT_STUCK_PUMPS_DURATION, stopDontStuckPumpsStatic);
        } else {
            if (this->machine->acsHandler->shouldGenerateACS()) {
                reset();
                this->machine->setState(this->machine->ACS_STATE);
            }

            if (this->machine->thermostat->isOn() && startHeatingMillis == 0) //
            {
                startHeatingMillis = millis();
            }

            if (this->machine->thermostat->isOff() && startHeatingMillis > 0) //
            {
                startHeatingMillis = 0;
            }

            //senal start ->
            //espera 2 min antes de pasar a estado 2
            if (millis() - startHeatingMillis > this->machine->configuration->DELAY_START_HEATING
                && this->machine->thermostat->isOn()) {
                reset();
                this->machine->setState(this->machine->START_HEATING_STATE);
            };

        }

    }
};

#endif
