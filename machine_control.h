
#ifndef machine_control__
#define machine_control__
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"
#include "measurement_and_calculations.h"
#include "vars.h"

void changeModo(bool paramModoFrio);  // funcion extra para cambio de Modo de Funcionamiento

void setupDigitalInputs();

void setupDigitalOuputs();

void initializeDigitalOuputs();

void writeOutput();

void refreshDataToShow();

void calculateStartStopSignal();

uint8_t normalizeAcsTemp(volatile uint8_t* acsValue);
bool heatingCheck();
bool coolingCheck();
bool longPeriodRunningCheck();

void takeRestControl();

void buzzerStart();

void buzzerStop(bool force);

#endif
