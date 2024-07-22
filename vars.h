
#ifndef vars__
#define vars__
#include <TimerOne.h>  //Librería para el control de salidas PWM
#include <arduino-timer.h>
#include <avr/wdt.h>  //Libreria para uso de watchdog de Arduino
extern Timer<1, millis> timer_things;

/**************************/
/*DECLARACION DE VARIABLES*/
/**************************/

// PINES DIGITALES

// 0x28, 0xAE, 0x16, 0xFF, 0x1B, 0x19, 0x01, 0xD1 }; //n5

extern const int DI_Caud_T;     // 18; //ENTRADAS DE CAUDALIMETROS (no se pueden modificar)
extern const int DI_Caud_H;     // 19;
extern const int DI_Marcha_on;  // 33; //Entrada de señal de Marcha
extern const int DI_Pres_HI;    // 35;   //Preostato de alta
extern const int DI_Pres_LOW;   // 37;  //Presotato de baja

extern const int DO_Calentador;    // 23; //Compresor
extern const int DO_Compressor;    // 25;    //Boombas de circulacion
extern const int DO_Bombas;        // 27;     //Valvula Calefaccion
extern const int DO_Valvula4Vias;  // 29;       //V4V
extern const int DO_ValvulaACS;    // 31;       //V ACS

extern const int DO_Triac_01;  // 11; //Triacs,Pin salida PWM (no se puede modificar)
extern const int DO_Buzzer;    // 12;   //

// VARIABLES DEL PROGRAMA

extern int Caud_T;
extern int C1_T;       // 0;
extern int C2_T;       // 0;
extern int C3_T;       // 0;
extern int Caud_Tacu;  // 0;

extern int Caud_H;
extern int C1_H;       // 0;
extern int C2_H;       // 0;
extern int C3_H;       // 0;
extern int Caud_Hacu;  // 0;

extern volatile int Estado_Maquina;  // 0;

extern int Cont_Temp_Des;  // 0;

// Contadores auxiliares de alarmas

extern int Cont_Temp_Compressor;  // 0;
extern int Cont_Press_HI;         // 0;
extern int Cont_Press_LOW;        // 0;
extern int Cont_Temp_Descarga;    // 0;

extern float Temp_Compressor;
extern float T5_Comp;             // 0;
extern float T4_Comp;             // 0;
extern float T3_Comp;             // 0;
extern float T2_Comp;             // 0;
extern float T1_Comp;             // 0;
extern float Temp_CompressorAcu;  // 0;

extern float Temp_ACS;
extern float T1_ACS;       // 0;
extern float T2_ACS;       // 0;
extern float T3_ACS;       // 0;
extern float Temp_ACSacu;  // 0;

extern float Temp_out_H;
extern float Temp_in_H;
extern float Temp_out_T;
extern float Temp_in_T;
extern float Temp_Descarga;
extern float Temp_Admision;

extern float T1_OH;          // 0;
extern float T2_OH;          // 0;
extern float T3_OH;          // 0;
extern float Temp_out_Hacu;  // 0;
extern float T1_IH;          // 0;
extern float T2_IH;          // 0;
extern float T3_IH;          // 0;
extern float Temp_in_Hacu;   // 0;

extern float T1_Des;            // 0;
extern float T2_Des;            // 0;
extern float T3_Des;            // 0;
extern float Temp_Descargaacu;  // 0;

extern bool Flag_TempCompressor;  // false;
extern bool Flag_Temp_Descarga;   // false;

extern const uint8_t GAP_ACS;  // 2 grados

extern unsigned long valvulaACSStart;  // 0;
extern unsigned long PumpStart;        // 0;
extern unsigned long BuzzerStart;      // 0;
extern unsigned long Ingreso_E7;       // 0;
extern unsigned long Ingreso_E71;      // 0;

extern unsigned long Periodo_Refresco;
extern unsigned long compressorStart;
extern unsigned long Salto_E1;
extern unsigned long dontStuckPumpsStart_activation;
extern unsigned long dontStuckPumpsStart;
extern unsigned long Ingreso_E3;

extern unsigned long Ingreso_Descanso;  // 0;

extern volatile int MenuActual;  //"volatile" se debe indicar para variables que se modifican en las rutinas de interrupción

extern volatile uint8_t Nro_Alarma;     // 0;
extern volatile uint8_t SetP_ACS;       // 0;
extern volatile uint8_t SetP_ACS_Edit;  // 0;

extern uint8_t Alarma_Eeprom;

// FLAGS     //Banderas de uso general para el funcionamiento del programa

extern bool deltaACSElectricResult;  // false;
extern bool Flag_CaudT;              // false;
extern bool Flag_CaudH;              // false;
extern bool Flag_PresHI;             // false;
extern bool Flag_PresLOW;            // false;

extern bool Flag_Temp_Adm;      // false;
extern volatile bool modoFrio;  // false; //Frio ; // true , Calor ; // false
extern volatile bool Alarma_Activa;
extern bool Flag_Marcha_ON;  // control de salto e1
extern bool senal_start;     // senal de marcha, segun modoFrio
// se trabaja con 1 termostato.
extern bool senal_stop;
extern volatile bool heating_off;
extern volatile bool Flag_Buzzer;

extern volatile bool EnableFlowAlarm;  // Alarmas de caudal
extern volatile bool EnableACS;
extern volatile bool EnableACS_DeltaElectrico;
extern volatile bool EnableElectricACS;

// IMAGENES DE ENTRADAS/SALIDAS
extern int Valor_DO_Bombas;
extern int Valor_DO_Calentador;
extern int Valor_DO_Compressor;
extern int Valor_DO_VACS;
extern int Valor_DO_V4V;
extern int Valor_DO_Buzzer;

#endif
