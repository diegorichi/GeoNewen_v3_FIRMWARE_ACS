
#ifndef alarm__
#define alarm__
#include "vars.h"
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"


int Alarmas(); //Función de identificación de Alarma Activa

void ResetFlags(); //Luego de ocurrida una alarma y revisada por parte del usuario, esta funcion resetea los flagas y contadores a cero

void checkFlagsForAlarms();

#endif
