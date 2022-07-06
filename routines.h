
#ifndef routines__
#define routines__
#include "vars.h"
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"


void FrioCalor(); //Función de cambio de Modo de Funcionamiento  (Bromberg: modo frio = valvula de 4 vias APAGADA)

int Alarmas(); //Función de identificación de Alarma Activa

void ResetFlags(); //Luego de ocurrida una alarma y revisada por parte del usuario, esta funcion resetea los flagas y contadores a cero

void checkWifi(); //Función de chequeo de conexión a internet

void checkESP(); //Función de chequeo de inicializacion de ESP8266

void setupDigitalInputs();

void setupDigitalOuputs();

void initializeDigitalOuputs();


#endif
