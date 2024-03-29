
#ifndef connectivity__
#define connectivity__
#include "vars.h"
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"
#include "machine_control.h"
#include "thingspeak.h"


void checkWifi(); //Función de chequeo de conexión a internet

void checkESP(); //Función de chequeo de inicializacion de ESP8266

void configWifi(); //Funcion de configuracion de conexion a WiFi

void checkEspWifiConnected(); // Verificacion de conexion a Wifi

void sendAndReceiveDataCloud(); // Envio de datos a ThingSpeak

void setupConnectivity(); // inicializa esp y wifi

#endif
