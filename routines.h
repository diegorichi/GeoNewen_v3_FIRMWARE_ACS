
#ifndef routines__
#define routines__
#include "vars.h"
#include "functionsLCDMenu.h"
#include "eeprom.h"


void FrioCalor(); //Función de cambio de Modo de Funcionamiento  (Bromberg: modo frio = valvula de 4 vias APAGADA)

int Alarmas(); //Función de identificación de Alarma Activa

void ResetFlags(); //Luego de ocurrida una alarma y revisada por parte del usuario, esta funcion resetea los flagas y contadores a cero

#endif
