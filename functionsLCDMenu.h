#ifndef LCD__
#define LCD__
#include <LiquidCrystal.h> //Libreria para el control del display
#include "vars.h"
#include "eeprom.h"


extern LiquidCrystal lcd;//(30, 32, 34, 36, 38, 40, 42, 44, 46, 48); //Se definen los pines RS, EN y D0 a D7 del display (en ese orden)
/****************/
/*FUNCIONES*/
/****************/

void lcdCreateSpecialChars();

void lcdRefreshValues();

void MenuCero(); //Pantalla de Inicio/Bienvenida

void MenuUno(); //Menú de Monitor de Temps y Consumo de Energía


void MenuUnoCero(); //1º Ventana de Monitor


void MenuUnoUno(); //2º Ventana de Monitor


void MenuUnoDos(); //3º Ventana de Monitor


void MenuDos(); //Menu de Configuración del Sistema


void MenuDosCero(); //Menú de Cambio de Modo de Funcionamiento


void MenuDosCeroCero(); //Ventana de Cambio de Modo de Funcionamiento


void MenuDosUno();  // Configuracion de temp de ACS


void MenuDosUnoCero();  //Seteo de temp de ACS


void MenuDosDos();


void MenuDosDosCero();


void MenuDosTres();   // Habilitacion alarmas de caudal


void MenuDosTresCero();


void MenuDosCuatro(); // Cambio a modo manual o automatico


void MenuDosCinco();


void MenuDosSeis();


void MenuDosSeisCero();


void MenuDosSiete(); //confiuracion de WIFI


void MenuDosSieteCero();


void MenuTres(); //Menú de Configuración de Caldera Eléctrica


void MenuTresCero(); //Ventana de Variación del Ciclo de Trabajo de la Caldera Eléctrica


void MenuCuatro(); //Menú de Visualización de Alarmas


void MenuCuatroCero(); //Visualización de Alarmas Activas


void MenuCinco();


void MenuCincoCero();


void MenuDosOcho(); // Habilitacion de Generacion de ACS 


void MenuDosOchoCero();


void MenuDosNueve();  // ACS electrico


void MenuDosNueveCero();

#endif
