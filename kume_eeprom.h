
#ifndef eeprom__
#define eeprom__

#include <EEPROM.h>  //Libreria para manejo de la memoria EEPROM del Arduino

#include "vars.h"

const int modoFrioAddress = 3;
const int acsSetpointAddress = 5;  // address 5 y 6 tomadas por ACS
const int heatingOffAddress = 7;
const int alarmaAddress = 11;
const int enableFlowAlarmAddress = 15;
const int enableAcsDeltaElectricoAddress = 19;
const int enableAcsAddress = 21;
const int enableElectricAcsAddress = 23;

uint8_t eepromReadUint8(int address);

void eepromWrite(int address, bool flag);

void eepromWrite(int address, uint8_t number);

void EEPROMLectura();  // Función de lectura de valores almacenados en memoria EEPROM

#endif
