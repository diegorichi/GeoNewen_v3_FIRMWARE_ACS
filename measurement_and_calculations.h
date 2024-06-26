
#ifndef measurement_and_calculation__
#define measurement_and_calculation__
#include "vars.h"
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"
#include <OneWire.h>
#include <DallasTemperature.h>

void Caudal1();

void Caudal2();

void temperatureMeasurement();

void flowsCalculation();

void thermalEfficiencyCalculation();

void powerCalculation();

void flowControl();

void temperatureControl();

void presureControl();

void auxiliaryACSHeatingControl();

void temperatureCalculation();

#endif
