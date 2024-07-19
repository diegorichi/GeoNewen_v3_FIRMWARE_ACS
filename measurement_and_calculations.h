
#ifndef measurement_and_calculation__
#define measurement_and_calculation__
#include <DallasTemperature.h>
#include <OneWire.h>

#include "functionsLCDMenu.h"
#include "kume_eeprom.h"
#include "vars.h"

void initializeFlowState();

void Caudal1();

void Caudal2();

void temperatureMeasurement();

void flowsCalculation();

void flowControl();

void temperatureControl();

void presureControl();

void auxiliaryACSHeatingControl();

void temperatureCalculation();

#endif
