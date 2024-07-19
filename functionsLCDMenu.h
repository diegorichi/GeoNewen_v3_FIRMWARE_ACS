#ifndef LCD__
#define LCD__
#include <LiquidCrystal.h>  //Libreria para el control del display

#include "kume_eeprom.h"
#include "vars.h"

extern LiquidCrystal lcd;  //(30, 32, 34, 36, 38, 40, 42, 44, 46, 48); //Se definen los pines RS, EN y D0 a D7 del display (en ese orden)
/****************/
/*FUNCIONES*/
/****************/

void lcdCreateSpecialChars();

void lcdRefreshValues();

// Solo en la ventana de bienvenida se muestra el estado actual del sistema y de la señal de marcha
void RefreshMenuCero();

// Este menú es la primer ventana del monitor de temperaturas y consumo eléctrico
void RefreshMenuUnoCero();

// La visualización de la segunda ventana del monitor depende del modo de funcionamiento, ya que en función de este se cambian de lugar las variables motradas
void RefreshMenuUnoUno();

// Modo Frio / Calor
void RefreshMenuDosCero();

// Editar valor ACS
void RefreshMenuDosUnoCero();

// Alarmas
void RefreshMenuDosTres();

// Funcionamiento Auto/Manual
void RefreshMenuDosCuatro();

// Activacion/desactivacion de ACS
void RefreshMenuDosOcho();

// Activacion/desactivacion de Delta ACS Electrico
void RefreshMenuDosOchoUno();

// Activacion/desactivacion de ACS electrico
void RefreshMenuDosNueve();

// En función al número de alarma devuelto por la función ConvertFlagToAlarm(), se muestra el mensaje indicando la causa de la misma
void RefreshAlarma(uint8_t _nro_Alarma);

// En función al número de alarma devuelto por la función ConvertFlagToAlarm(), se muestra el mensaje indicando la causa de la misma
void RefreshMenuCuatroCero();

// En función al número de alarma devuelto por la función ConvertFlagToAlarm(), se muestra el mensaje indicando la causa de la misma
void RefreshMenuCincoCero();

void MenuCero();  // Pantalla de Inicio/Bienvenida

void MenuUno();  // Menú de Monitor de Temps y Consumo de Energía

void MenuUnoCero();  // 1º Ventana de Monitor

void MenuUnoUno();  // 2º Ventana de Monitor

void MenuDos();  // Menu de Configuración del Sistema

void MenuDosCero();  // Menú de Cambio de Modo de Funcionamiento

void MenuDosUno();  // Configuracion de temp de ACS

void MenuDosUnoCero();  // Seteo de temp de ACS

void MenuDosTres();  // Habilitacion alarmas de caudal

void MenuDosCuatro();  // Cambio a modo manual o automatico

void MenuCuatro();  // Menú de Visualización de Alarmas

void MenuCuatroCero();  // Visualización de Alarmas Activas

void MenuCinco();  // Registro de alarmas

void MenuCincoCero();  // Registro de alarmas

void MenuDosOcho();  // Habilitacion de Generacion de ACS

void MenuDosOchoUno();  // Habilitacion de Delta de ACS

void MenuDosNueve();  // ACS electrico

#endif
