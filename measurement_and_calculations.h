
#ifndef measurement_and_calculation__
#define measurement_and_calculation__
#include "vars.h"
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"
#include <OneWire.h>
#include <DallasTemperature.h>

void initializeFlowState();

void Caudal1();

void Caudal2();

void temperatureMeasurement();

void flowsCalculation();

void flowControl();

void temperatureControl();

void presureControl();

void auxiliaryACSHeatingControl();

void calculateValuesToShow();

#endif
