
#ifndef machine_control__
#define machine_control__
#include "kume_eeprom.h"
#include "measurement_and_calculations.h"
#include "vars.h"

inline bool isAcsGenerationRequired() {
    return tempAcs < (acsSetpoint - GAP_ACS) && enableAcs;
}

inline bool isElectricAcsActive() {
    return enableElectricAcs && valorDoCalentador == HIGH;
}

inline bool isDeltaAcsElectricActive() {
    return enableAcs && enableAcsDeltaElectrico && deltaAcsElectricResult && valorDoCalentador == HIGH;
}

void changeModo(bool paramModoFrio);  // funcion extra para cambio de Modo de Funcionamiento
bool isModeChangeLocked();

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

void buzzerControl();

void buzzerStop();

void buzzerBip();

#endif
