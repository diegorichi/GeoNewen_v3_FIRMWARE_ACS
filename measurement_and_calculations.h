
#ifndef measurement_and_calculation__
#define measurement_and_calculation__
#include "vars.h"
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"

void temperatureMeasuement(); 

void flowsCalculation(); 

void thermalEfficiencyCalculation(); 

void powerCalculation(); 

void flowControl(); 

void temperatureControl(); 

void presureControl(); 

void auxiliaryACSHeatingControl();

void calculateValuesToShow();

#endif
