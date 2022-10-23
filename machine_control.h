
#ifndef machine_control__
#define machine_control__
#include "vars.h"
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"
#include "measurement_and_calculations.h"

void FrioCalor(); // Función de cambio de Modo de Funcionamiento  (Bromberg: modo frio = valvula de 4 vias APAGADA)

void setupDigitalInputs();

void setupDigitalOuputs();

void initializeDigitalOuputs();

void refreshDataToShow();

void processStartStopSignal();

#endif
