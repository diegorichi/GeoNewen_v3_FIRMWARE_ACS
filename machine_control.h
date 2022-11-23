
#ifndef machine_control__
#define machine_control__
#include "vars.h"
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"
#include "measurement_and_calculations.h"

void frioCalor(); // Función de cambio de Modo de Funcionamiento  (Bromberg: modo frio = valvula de 4 vias APAGADA)

void changeModo(); // funcion extra para cambio de Modo de Funcionamiento

void setupDigitalInputs();

void setupDigitalOuputs();

void initializeDigitalOuputs();

void refreshDataToShow();

void processStartStopSignal();

volatile uint8_t normalizeAcsTemp(volatile uint8_t *acsValue);

#endif
