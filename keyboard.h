#ifndef keyboard__
#define keyboard__

#include "vars.h"
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"
#include "menu.h"
#include "machine_control.h"
#include "alarm.h"


void keyboardSetup(); // setup de pin modes y demas

void AtencionTeclado(); //Función de Navegación entre Menús y Modificación de Parámetros

#endif
