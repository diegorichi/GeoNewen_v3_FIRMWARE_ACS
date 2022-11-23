
#ifndef eeprom__
#define eeprom__

#include "vars.h"

#include <EEPROM.h> //Libreria para manejo de la memoria EEPROM del Arduino

const int modo_frio_address = 3;
const int SetP_ACS_Address = 5; // address 5 y 6 tomadas por ACS
const int heating_off_address = 7;
const int Alarma_Address = 11;
const int Alarma_EN_Address = 15;
const int ACS_DT_EN_Address = 19;
const int ACS_EN_Address = 21;
const int ACS_EN_ELECT_Address = 23;

uint8_t EEPROMreaduint8_t(int address);

bool EEPROMbool(int address);

void EEPROMwrite(int address, bool flag);

void EEPROMwrite(int address, uint8_t number);

void EEPROMLectura(); // Función de lectura de valores almacenados en memoria EEPROM

#endif
