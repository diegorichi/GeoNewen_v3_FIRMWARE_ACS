
#ifndef vars__
#define vars__
#include <TimerOne.h>  //Librería para el control de salidas PWM
#include <arduino-timer.h>
#include <avr/wdt.h>  //Libreria para uso de watchdog de Arduino
extern Timer<2, millis> timer_things;

/**************************/
/*DECLARACION DE VARIABLES*/
/**************************/

// PINES DIGITALES

// 0x28, 0xAE, 0x16, 0xFF, 0x1B, 0x19, 0x01, 0xD1 }; //n5

extern const int DI_Caud_T;  // 18; //ENTRADAS DE CAUDALIMETROS (no se pueden modificar)
extern const int DI_Caud_H;  // 19;

extern const int DO_Calentador;  // 23; //Compresor
extern const int DO_Comp_01;     // 25;    //Boombas de circulacion
extern const int DO_Bombas;      // 27;     //Valvula Calefaccion
extern const int DO_Val2;        // 29;       //V4V
extern const int DO_Val1;        // 31;       //V ACS

extern const int DO_Triac_01;  // 11; //Triacs,Pin salida PWM (no se puede modificar)
extern const int DO_Buzzer;    // 12;   //
// extern const int DO_Contraste     ; // 13;  //Control de contraste display (sin uso)

extern const int DI_Marcha_on;  // 33; //Entrada de señal de Marcha
extern const int DI_Pres_HI;    // 35;   //Preostato de alta
extern const int DI_Pres_LOW;   // 37;  //Presotato de baja

extern const int STEP;       // 9;       //Pines para control del EasyDriver
extern const int DIR;        // 8;        //STEP es el pin por donde se envian los pasos para que el motor gire, DIR indica la direccion
extern const int ED_ENABLE;  // 10; //Pin para "dormir" al EasyDriver, evitando consumo de energia

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

extern int Cont_Temp_Comp_01;   // 0;
extern int Cont_Press_HI;       // 0;
extern int Cont_Press_LOW;      // 0;
extern int Cont_Temp_Descarga;  // 0;

extern float Temp_Comp_01;
extern float T5_Comp;        // 0;
extern float T4_Comp;        // 0;
extern float T3_Comp;        // 0;
extern float T2_Comp;        // 0;
extern float T1_Comp;        // 0;
extern float Temp_comp_acu;  // 0;

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

const uint8_t MAX_ACS = 48;
const uint8_t MIN_ACS = 30;
const uint8_t GAP_ACS = 2;
const uint8_t DELTA_ACS_ELECTRICO = 7;

const float MAX_TEMP_OUT_H_ACS = 50.0;
const float MAX_TEMP_OUT_H_HEATING = 37.0;
const float MIN_TEMP_OUT_H_COOLING = 10.0;
const float MIN_TEMP_OUT_H_HEATING = 1.0;
const float MAX_TEMP_OUT_T = 40.0;
const float MIN_TEMP_OUT_T = -6.0;
const float MIN_TEMP_ADMISION = -7.0;

extern bool Flag_TempComp01;     // false;
extern bool Flag_Temp_Descarga;  // false;
extern bool Flag_retardo_e7;     // flag que indica que ACS esta efectivamente andando (luego de la espera)

extern unsigned long E1_a_E2;              // 120000;           //2 minutos para pasar de E1 a E2
extern unsigned long E2_a_E3;              // 10000;            //10 segundos para pasar de E2 a E3
extern volatile byte MAX_ACS;              // 48
extern volatile byte MIN_ACS;              // 30
extern volatile byte GAP_ACS;              // 2
extern volatile byte DELTA_ACS_ELECTRICO;  // 7

extern unsigned long EsperaValv;       // 0;
extern unsigned long Ingreso_E7;       // 0;
extern unsigned long Ingreso_E71;      // 0;
extern unsigned long Periodo_Fin_ACS;  // 0;

extern unsigned long Periodo_Refresco;
extern unsigned long Activacion_Comp;
extern unsigned long Salto_E1;
extern unsigned long dont_stuck_pumps_activation;
extern unsigned long dont_stuck_pumps;
extern unsigned long Ingreso_E3;
extern unsigned long LecturaDSB;

extern unsigned long Ingreso_Descanso;  // 0;

extern volatile int MenuActual;  //"volatile" se debe indicar para variables que se modifican en las rutinas de interrupción

extern volatile uint8_t Nro_Alarma;     // 0;
extern volatile uint8_t SetP_ACS;       // 0;
extern volatile uint8_t SetP_ACS_Edit;  // 0;

extern uint8_t Alarma_Eeprom;

// FLAGS     //Banderas de uso general para el funcionamiento del programa

extern bool flag_dtElectrico_final;  // false;
extern bool Flag_CaudT;              // false;
extern bool Flag_CaudH;              // false;
extern bool Flag_PresHI;             // false;
extern bool Flag_PresLOW;            // false;

extern bool PressOK;                       // false;

extern bool Flag_Temp_Adm;      // false;
extern volatile bool modoFrio;  // false; //Frio ; // true , Calor ; // false
extern volatile bool Alarma_Activa;
extern bool Flag_Marcha_ON;  // control de salto e1
extern bool senal_start;     // senal de marcha, segun modoFrio
// se trabaja con 1 termostato.
extern bool senal_stop;
extern volatile bool heating_off;
extern volatile bool Flag_Buzzer;

extern volatile bool Flag_Alarma_EN;
extern volatile bool Flag_ACS_EN;        // true;
extern volatile bool Flag_ACS_DT_EN;     // true;
extern volatile bool Flag_ACS_EN_ELECT;  // false;

// IMAGENES DE ENTRADAS/SALIDAS
extern int Valor_DO_Bombas;
extern int Valor_DO_Calentador;
extern int Valor_DO_Comp_01;
extern int Valor_DO_VACS;
extern int Valor_DO_V4V;

#endif
