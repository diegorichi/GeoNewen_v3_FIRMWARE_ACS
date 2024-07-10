#include "vars.h"

/**************************/
/*DECLARACION DE VARIABLES*/
/**************************/

Timer<2, millis> timer_things;

// PINES DIGITALES

// 0x28, 0xAE, 0x16, 0xFF, 0x1B, 0x19, 0x01, 0xD1 }; //n5

const int DI_Caud_T = 18; // ENTRADAS DE CAUDALIMETROS (no se pueden modificar)
const int DI_Caud_H = 19;

const int DO_Calentador = 23; // Compresor
const int DO_Comp_01 = 25;    // Boombas de circulacion
const int DO_Bombas = 27;     // Valvula Calefaccion
const int DO_Val2 = 29;       // V4V
const int DO_Val1 = 31;       // V ACS

const int DO_Triac_01 = 11; // Triacs,Pin salida PWM (no se puede modificar)
const int DO_Buzzer = 12;   //
// const int DO_Contraste     = 13;  //Control de contraste display (sin uso)

const int DI_Marcha_on = 33; // Entrada de señal de Marcha
const int DI_Pres_HI = 35;   // Preostato de alta
const int DI_Pres_LOW = 37;  // Presotato de baja

const int STEP = 9;       // Pines para control del EasyDriver
const int DIR = 8;        // STEP es el pin por donde se envian los pasos para que el motor gire, DIR indica la direccion
const int ED_ENABLE = 10; // Pin para "dormir" al EasyDriver, evitando consumo de energia


volatile int Estado_Maquina = 0;

// VARIABLES DEL PROGRAMA

int Caud_T;
int C1_T = 0;
int C2_T = 0;
int C3_T = 0;
int Caud_Tacu = 0;

int Caud_H;
int C1_H = 0;
int C2_H = 0;
int C3_H = 0;
int Caud_Hacu = 0;


int Cont_Temp_Des = 0;

// Contadores auxiliares de alarmas

int Cont_Temp_Comp_01 = 0;
int Cont_Press_HI = 0;
int Cont_Press_LOW = 0;
int Cont_Temp_Descarga = 0;

float Temp_Comp_01;
float T5_Comp = 0;
float T4_Comp = 0;
float T3_Comp = 0;
float T2_Comp = 0;
float T1_Comp = 0;
float Temp_comp_acu = 0;

float Temp_ACS;
float T1_ACS = 0;
float T2_ACS = 0;
float T3_ACS = 0;
float Temp_ACSacu = 0;

float Temp_out_H;
float Temp_in_H;
float Temp_out_T;
float Temp_in_T;
float Temp_Descarga;
float Temp_Admision;

float T1_OH = 0;
float T2_OH = 0;
float T3_OH = 0;
float Temp_out_Hacu = 0;
float T1_IH = 0;
float T2_IH = 0;
float T3_IH = 0;
float Temp_in_Hacu = 0;
float T1_Des = 0;
float T2_Des = 0;
float T3_Des = 0;
float Temp_Descargaacu = 0;

bool Flag_TempComp01 = false;
bool Flag_Temp_Descarga = false;
bool Flag_retardo_e7 = false;

unsigned long EsperaValv = 0;
unsigned long PumpStart = 0;
unsigned long Ingreso_E7 = 0;
unsigned long Ingreso_E71 = 0;
unsigned long Periodo_Fin_ACS = 0;

const uint8_t GAP_ACS = 2;

unsigned long Periodo_Refresco;
unsigned long Activacion_Comp;
unsigned long Salto_E1;
unsigned long dont_stuck_pumps_activation;
unsigned long dont_stuck_pumps;
unsigned long Ingreso_E3;
unsigned long LecturaDSB;

unsigned long Ingreso_Descanso = 0;

volatile int MenuActual; //"volatile" se debe indicar para variables que se modifican en las rutinas de interrupción

volatile uint8_t Nro_Alarma = 0;
volatile uint8_t SetP_ACS = 0;
volatile uint8_t SetP_ACS_Edit = 0;

uint8_t Alarma_Eeprom;

// FLAGS     //Banderas de uso general para el funcionamiento del programa

bool flag_dtElectrico_final = false;
bool Flag_CaudT = false;
bool Flag_CaudH = false;
bool Flag_PresHI = false;
bool Flag_PresLOW = false;

bool PressOK = false;

bool Flag_Temp_Adm = false;
volatile bool modoFrio = false; // Frio = true , Calor = false
volatile bool Alarma_Activa;
bool Flag_Marcha_ON;
bool senal_start; // senal de marcha, segun modoFrio
// se trabaja con 1 termostato.
bool senal_stop;
volatile bool heating_off = false;
volatile bool Flag_Buzzer;

volatile bool Flag_ACS_EN = true;
volatile bool Flag_ACS_DT_EN = true;
volatile bool Flag_Alarma_EN;
volatile bool Flag_ACS_EN_ELECT = false;

// IMAGENES DE ENTRADAS/SALIDAS
int Valor_DO_Bombas;
int Valor_DO_Calentador;
int Valor_DO_Comp_01;
int Valor_DO_VACS;
int Valor_DO_V4V;
