
#ifndef alarm__
#define alarm__
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"
#include "machine_control.h"
#include "vars.h"

void ConvertFlagToAlarm();  // Función de identificación de Alarma Activa

void ResetFlags();  // Luego de ocurrida una alarma y revisada por parte del usuario, esta funcion resetea los flagas y contadores a cero

void checkFlagsForAlarms();

void resetAlarms();

#endif
