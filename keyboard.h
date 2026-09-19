#ifndef keyboard__
#define keyboard__

#include "alarm.h"
#include "functionsLCDMenu.h"
#include "machine_control.h"
#include "vars.h"

void keyboardSetup();  // setup de pin modes y demas

void AtencionTeclado();  // ISR: solo registra que hubo una interrupción

void procesarTeclado();  // Navegación fuera del contexto de interrupción

#endif
