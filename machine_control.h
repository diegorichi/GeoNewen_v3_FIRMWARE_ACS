
#ifndef machine_control__
#define machine_control__
#include "vars.h"
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"
#include "measurement_and_calculations.h"

void changeModo(); // funcion extra para cambio de Modo de Funcionamiento

void setupDigitalInputs();

void setupDigitalOuputs();

void initializeDigitalOuputs();

void refreshDataToShow();

void processStartStopSignal();

uint8_t normalizeAcsTemp(volatile uint8_t* acsValue);
bool heatingCheck();
bool coolingCheck();
bool longPeriodRunningCheck();

void takeRestControl();

#endif
