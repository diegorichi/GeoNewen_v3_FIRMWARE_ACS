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

void beginLcd();

void lcdRefreshValues();

// Solo en la ventana de bienvenida se muestra el estado actual del sistema y de la señal de marcha
void refreshHomeScreen();

// Este menú es la primer ventana del monitor de temperaturas y consumo eléctrico
void refreshMonitorScreen1();

// La visualización de la segunda ventana del monitor depende del modo de funcionamiento, ya que en función de este se cambian de lugar las variables motradas
void refreshMonitorScreen2();

// Modo Frio / Calor
void refreshModeScreen();

// Editar valor ACS
void refreshAcsEditScreen();

// Alarmas
void refreshFlowAlarmScreen();

// Funcionamiento Auto/Manual
void refreshHeatingScreen();

// Activacion/desactivacion de ACS
void refreshAcsEnableScreen();

// Activacion/desactivacion de Delta ACS Electrico
void refreshAcsDeltaScreen();

// Activacion/desactivacion de ACS electrico
void refreshAcsElectricScreen();

// En función al número de alarma devuelto por la función ConvertFlagToAlarm(), se muestra el mensaje indicando la causa de la misma
void refreshAlarmMessage(uint8_t _nro_Alarma);

// En función al número de alarma devuelto por la función ConvertFlagToAlarm(), se muestra el mensaje indicando la causa de la misma
void refreshActiveAlarmScreen();

// En función al número de alarma devuelto por la función ConvertFlagToAlarm(), se muestra el mensaje indicando la causa de la misma
void refreshAlarmHistoryScreen();

void drawHomeScreen();  // Pantalla de Inicio/Bienvenida

void drawMonitorMenu();  // Menú de Monitor de Temps y Consumo de Energía

void drawMonitorScreen1();  // 1º Ventana de Monitor

void drawMonitorScreen2();  // 2º Ventana de Monitor

void drawConfigurationMenu();  // Menu de Configuración del Sistema

void drawModeScreen();  // Menú de Cambio de Modo de Funcionamiento

void drawAcsConfigurationScreen();  // Configuracion de temp de ACS

void drawAcsEditScreen();  // Seteo de temp de ACS

void drawFlowAlarmScreen();  // Habilitacion alarmas de caudal

void drawHeatingScreen();  // Cambio a modo manual o automatico

void drawAlarmMenu();  // Menú de Visualización de Alarmas

void drawActiveAlarmScreen();  // Visualización de Alarmas Activas

void drawAlarmHistoryMenu();  // Registro de alarmas

void drawAlarmHistoryScreen();  // Registro de alarmas

void drawAcsEnableScreen();  // Habilitacion de Generacion de ACS

void drawAcsDeltaScreen();  // Habilitacion de Delta de ACS

void drawAcsElectricScreen();  // ACS electrico

#endif
