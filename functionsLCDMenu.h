#ifndef LCD__
#define LCD__
#include <LiquidCrystal.h> //Libreria para el control del display
#include "vars.h"
#include "kume_eeprom.h"

extern LiquidCrystal lcd; //(30, 32, 34, 36, 38, 40, 42, 44, 46, 48); //Se definen los pines RS, EN y D0 a D7 del display (en ese orden)
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

// La tercera ventana muestra la eficiencia térmica
void RefreshMenuUnoDos();

// Modo Frio / Calor
void RefreshMenuDosCero();

// Editar valor ACS
void RefreshMenuDosUnoCero();

// Alarmas
void RefreshMenuDosTres();

// Funcionamiento Auto/Manual
void RefreshMenuDosCuatro();

// Pos_Valv
//void RefreshMenuDosSeis();

// Conexion de WIFI (Conectado/No Conectado)
void RefreshMenuDosSiete();

// Activacion/desactivacion de ACS
void RefreshMenuDosOcho();

// Activacion/desactivacion de ACS electrico
void RefreshMenuDosNueve();

// Estado caldera (ENCENDIDA/APAGADA)
void RefreshMenuTresCero();

// En función al número de alarma devuelto por la función Alarmas(), se muestra el mensaje indicando la causa de la misma
void RefreshAlarma(volatile byte _nro_Alarma);

// En función al número de alarma devuelto por la función Alarmas(), se muestra el mensaje indicando la causa de la misma
void RefreshMenuCuatroCero();

// En función al número de alarma devuelto por la función Alarmas(), se muestra el mensaje indicando la causa de la misma
void RefreshMenuCincoCero();


void MenuCero(); // Pantalla de Inicio/Bienvenida

void MenuUno(); // Menú de Monitor de Temps y Consumo de Energía

void MenuUnoCero(); // 1º Ventana de Monitor

void MenuUnoUno(); // 2º Ventana de Monitor

void MenuUnoDos(); // 3º Ventana de Monitor

void MenuDos(); // Menu de Configuración del Sistema

void MenuDosCero(); // Menú de Cambio de Modo de Funcionamiento

void MenuDosUno(); // Configuracion de temp de ACS

void MenuDosUnoCero(); // Seteo de temp de ACS

void MenuDosTres(); // Habilitacion alarmas de caudal

void MenuDosCuatro(); // Cambio a modo manual o automatico

void MenuDosSeis();

//void MenuDosSeisCero();

void MenuDosSiete(); // confiuracion de WIFI

void MenuTres(); // Menú de Configuración de Caldera Eléctrica

void MenuTresCero(); // Ventana de Variación del Ciclo de Trabajo de la Caldera Eléctrica

void MenuCuatro(); // Menú de Visualización de Alarmas

void MenuCuatroCero(); // Visualización de Alarmas Activas

void MenuCinco();

void MenuCincoCero();

void MenuDosOcho(); // Habilitacion de Generacion de ACS

void MenuDosNueve(); // ACS electrico


#endif
