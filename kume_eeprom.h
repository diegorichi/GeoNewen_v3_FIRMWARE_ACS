
#ifndef eeprom__
#define eeprom__

#include "vars.h"

#include <EEPROM.h>        //Libreria para manejo de la memoria EEPROM del Arduino

void EEPROMUpdate(); //Función de guardado de valores en memoria EEPROM

void EEPROMLectura(); //Función de lectura de valores almacenados en memoria EEPROM

#endif
