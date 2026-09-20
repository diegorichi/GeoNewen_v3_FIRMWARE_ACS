#ifndef keyboard__
#define keyboard__

#include "menu_navigation.h"
#include "vars.h"

void keyboardSetup();  // setup de pin modes y demas

void AtencionTeclado();  // ISR: solo registra que hubo una interrupción

void procesarTeclado();  // Lectura y debounce fuera del contexto de interrupción

#endif
