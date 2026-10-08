
#ifndef vars__
#define vars__
#include "menu_navigation.h"
#include <TimerOne.h>  //Librería para el control de salidas PWM
#include <avr/wdt.h>  //Libreria para uso de watchdog de Arduino
#include <stdint.h>

// Poner en 0 para una compilacion de produccion sin logs por USB.
#define GEO_DEBUG_SERIAL 1

#if GEO_DEBUG_SERIAL
#define GEO_LOG_BEGIN(baud) Serial.begin(baud)
#define GEO_LOG_PRINT(...) Serial.print(__VA_ARGS__)
#define GEO_LOG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
#define GEO_LOG_BEGIN(baud) do { } while (0)
#define GEO_LOG_PRINT(...) do { } while (0)
#define GEO_LOG_PRINTLN(...) do { } while (0)
#endif

/**************************/
/*DECLARACION DE VARIABLES*/
/**************************/

// PINES DIGITALES

// 0x28, 0xAE, 0x16, 0xFF, 0x1B, 0x19, 0x01, 0xD1 }; //n5

extern const int diCaudT;     // 18; //ENTRADAS DE CAUDALIMETROS (no se pueden modificar)
extern const int diCaudH;     // 19;
extern const int diMarchaOn;  // 33; //Entrada de señal de Marcha
extern const int diPresHi;    // 35;   //Preostato de alta
extern const int diPresLow;   // 37;  //Presotato de baja

extern const int doCalentador;    // 23; //Compresor
extern const int doCompressor;    // 25;    //Boombas de circulacion
extern const int doBombas;        // 27;     //Valvula Calefaccion
extern const int doValvula4Vias;  // 29;       //V4V
extern const int doValvulaAcs;    // 31;       //V ACS

extern const int doTriac01;  // 11; //Triacs,Pin salida PWM (no se puede modificar)
extern const int doBuzzer;    // 12;   //

// VARIABLES DEL PROGRAMA

extern int caudT;
extern int c1T;       // 0;
extern int c2T;       // 0;
extern int c3T;       // 0;
extern int caudTacu;  // 0;

extern int caudH;
extern int c1H;       // 0;
extern int c2H;       // 0;
extern int c3H;       // 0;
extern int caudHacu;  // 0;

extern volatile int estadoMaquina;  // 0;

extern int contTempDes;  // 0;

// Contadores auxiliares de alarmas

extern int contTempCompressor;  // 0;
extern int contPressHi;         // 0;
extern int contPressLow;        // 0;
extern int contTempDescarga;    // 0;

extern float tempCompressor;
extern float t5Comp;             // 0;
extern float t4Comp;             // 0;
extern float t3Comp;             // 0;
extern float t2Comp;             // 0;
extern float t1Comp;             // 0;
extern float tempCompressorAcu;  // 0;

extern float tempAcs;
extern float t1Acs;       // 0;
extern float t2Acs;       // 0;
extern float t3Acs;       // 0;
extern float tempAcsAcu;  // 0;

extern float tempOutH;
extern float tempInH;
extern float tempOutT;
extern float tempInT;
extern float tempDescarga;
extern float tempAdmision;

extern float t1Oh;          // 0;
extern float t2Oh;          // 0;
extern float t3Oh;          // 0;
extern float tempOutHacu;  // 0;
extern float t1Ih;          // 0;
extern float t2Ih;          // 0;
extern float t3Ih;          // 0;
extern float tempInHacu;   // 0;

extern float t1Des;            // 0;
extern float t2Des;            // 0;
extern float t3Des;            // 0;
extern float tempDescargaAcu;  // 0;

extern bool flagTempCompressor;  // false;
extern bool flagTempDescarga;   // false;

extern const uint8_t GAP_ACS;  // 2 grados

extern unsigned long valvulaAcsStart;  // 0;
extern unsigned long pumpStart;        // 0;
extern unsigned long ingresoE7;       // 0;
extern unsigned long ingresoE71;      // 0;

extern unsigned long periodoRefresco;
extern unsigned long compressorStart;
extern unsigned long saltoE1;
extern unsigned long dontStuckPumpsStartActivation;
extern unsigned long dontStuckPumpsStart;
extern unsigned long ingresoE3;

extern unsigned long ingresoDescanso;  // 0;

extern volatile MenuId menuActual;

extern volatile uint8_t nroAlarma;     // 0;
extern volatile uint8_t acsSetpoint;       // 0;
extern volatile uint8_t acsSetpointEdit;  // 0;

extern uint8_t alarmaEeprom;

// FLAGS     //Banderas de uso general para el funcionamiento del programa

extern bool deltaAcsElectricResult;  // false;
extern bool flagCaudT;              // false;
extern bool flagCaudH;              // false;
extern bool flagPresHi;             // false;
extern bool flagPresLow;            // false;

extern bool flagTempAdm;      // false;
extern volatile bool modoFrio;  // false; //Frio ; // true , Calor ; // false
extern volatile bool alarmaActiva;
extern bool flagMarchaOn;  // control de salto e1
extern bool senalStart;     // senal de marcha, segun modoFrio
// se trabaja con 1 termostato.
extern bool senalStop;
extern volatile bool heatingOff;
extern volatile bool flagBuzzer;

extern volatile bool enableFlowAlarm;  // Alarmas de caudal
extern volatile bool enableAcs;
extern volatile bool enableAcsDeltaElectrico;
extern volatile bool enableElectricAcs;

// IMAGENES DE ENTRADAS/SALIDAS
extern int valorDoBombas;
extern int valorDoCalentador;
extern int valorDoCompressor;
extern int valorDoVacs;
extern int valorDoV4v;
extern volatile int valorDoBuzzer;

#endif
