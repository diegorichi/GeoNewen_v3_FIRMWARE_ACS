
#ifndef eeprom__
#define eeprom__

#include <EEPROM.h>  //Libreria para manejo de la memoria EEPROM del Arduino

#include "vars.h"

const int modoFrio_address = 3;
const int SetP_ACS_Address = 5;  // address 5 y 6 tomadas por ACS
const int heating_off_address = 7;
const int Alarma_Address = 11;
const int EnableFlowAlarm_Address = 15;
const int EnableACS_DeltaElectrico_Address = 19;
const int EnableACS_Address = 21;
const int EnableElectricACS_Address = 23;

uint8_t EEPROMreaduint8_t(int address);

bool EEPROMbool(int address);

void EEPROMwrite(int address, bool flag);

void EEPROMwrite(int address, uint8_t number);

void EEPROMLectura();  // Función de lectura de valores almacenados en memoria EEPROM

#endif
