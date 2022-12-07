
#ifndef acs_handler__
#define acs_handler__
#include "vars.h"
#include "Machine.h"


class ElectricHeating {
    bool enabled;

    public:
    void turnOn();
    void turnOff();
    void switchState();

};

class ACSHandler {

    bool enabled;

    public:
    ElectricHeating* electricHeating;

    Machine* machine;

    ACSHandler(Machine* _machine) {
        machine = _machine;
    };

    void init() {
        electricHeating = new ElectricHeating();
    };

    void handleAuxiliaryACSHeating() {
        /*
          if (((Temp_ACSacu >= (SetP_ACS + DELTA_ACS_ELECTRICO)) && Flag_ACS_EN) // Si la temp ACS alcanza el objetivo, apagamos el calentador
            || ((Temp_ACSacu <= (SetP_ACS - GAP_ACS)) && Flag_ACS_EN)          // Si la temp es menor al seteo, lo apago porque estado = 7 -> generar acs
            || !Flag_ACS_EN || !Flag_ACS_DT_EN)                                // si apago generac ACS no hay delta t final.
        {
            flag_dtElectrico_final = false;
        }

        // Si la temperatura baja del gap del objetivo, volvemos a prender el calendador
        //  pero solo si es mayor a la seteada, de manera tal que usamos el cartucho solo en el
        // ultimo tramo de ACS.
        if (Temp_ACSacu < (SetP_ACS + DELTA_ACS_ELECTRICO - GAP_ACS) && (Temp_ACSacu > SetP_ACS) && Flag_ACS_EN && Flag_ACS_DT_EN) {
            flag_dtElectrico_final = true;
        }

        // si acs elect apagado -> lo apagamos
        // si acs apagado -> lo apagamos
        if (Flag_ACS_EN_ELECT || (Flag_ACS_EN && Flag_ACS_DT_EN && flag_dtElectrico_final)) {
            Valor_DO_Calentador = HIGH;
        } else {
            Valor_DO_Calentador = LOW;
        }
        */
    }

    bool shouldGenerateACS() {
        return false;
    };
    bool shouldStopGenerateACS() {
        return false;
    };
};

#endif
