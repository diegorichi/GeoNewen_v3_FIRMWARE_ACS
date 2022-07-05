#ifndef keyboard__
#define keyboard__

#include "vars.h"

#include "functionsLCDMenu.h"
#include "routines.h"
#include "kume_eeprom.h"


extern const int DI_Teclado_Arriba ; //7; //BOTONES DE TECLADO (no se pueden modificar)
extern const int DI_Teclado_Abajo ; //5;
extern const int DI_Teclado_Enter ; //6;
extern const int DI_Teclado_Atras ; //4;
extern volatile bool BotonArriba ; //false; //Variables auxiliares para el funcionamiento del teclado, usadas en la rutina de atencion de teclado
extern volatile bool BotonAtras ; //false;
extern volatile bool BotonAbajo ; //false;
extern volatile bool BotonEnter ; //false;
extern volatile int BotonArribaT ; //false;
extern volatile int BotonAtrasT ; //false;
extern volatile int BotonAbajoT ; //false;
extern volatile int BotonEnterT ; //false;

void AtencionTeclado(); //Función de Navegación entre Menús y Modificación de Parámetros

#endif
