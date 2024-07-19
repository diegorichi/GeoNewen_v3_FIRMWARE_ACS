
#ifndef state_machine__
#define state_machine__
#include "alarm.h"
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"
#include "machine_control.h"
#include "vars.h"

#define GENERATE_ACS (Temp_ACS < (SetP_ACS - GAP_ACS) && EnableACS)

void initializeStateMachine();

void stateMachine0();   // Estado inicial del sistema, tanto el compresor como las bombas de circulación están apagados
void stateMachine1();   // Aquí se espera la señal de Marcha_ON para iniciar la operacion del sistema
void stateMachine2();   // Arranque Compresor y Bombas
void stateMachine3();   // Este es el estado final del sistema, donde se controlan las condiciones de alarma
void stateMachine4();   // Estado de Alarma
void stateMachine6();   // Estado de descanso
void stateMachine7();   // Generacion ACS
void stateMachine71();  // Generacion ACS: Estado con bombas andando y compresor apagado

#endif
