#include "vars.h"

/**************************/
/*DECLARACION DE VARIABLES*/
/**************************/

Timer<1, millis> timer_things;

// PINES DIGITALES

// 0x28, 0xAE, 0x16, 0xFF, 0x1B, 0x19, 0x01, 0xD1 }; //n5

const int DI_Caud_T = 18;     // Entrada de caudalimetro tierra
const int DI_Caud_H = 19;     // Entrada de caudalimetro hogar
const int DI_Marcha_on = 33;  // Entrada de señal de Marcha
const int DI_Pres_HI = 35;    // Preostato de alta
const int DI_Pres_LOW = 37;   // Presotato de baja

const int DO_Calentador = 23;    // Calentador
const int DO_Compressor = 25;    // Compresor
const int DO_Bombas = 27;        // Bombas
const int DO_Valvula4Vias = 29;  // Valvula 4 Vias
const int DO_ValvulaACS = 31;    // Valvula ACS

const int DO_Triac_01 = 11;  // Triacs, Pin salida PWM (no se puede modificar)
const int DO_Buzzer = 12;    // Pin de salida de buzzer

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

int Cont_Temp_Compressor = 0;
int Cont_Press_HI = 0;
int Cont_Press_LOW = 0;
int Cont_Temp_Descarga = 0;

float Temp_Compressor;
float T5_Comp = 0;
float T4_Comp = 0;
float T3_Comp = 0;
float T2_Comp = 0;
float T1_Comp = 0;
float Temp_CompressorAcu = 0;

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
float Temp_DescargaAcu = 0;

bool Flag_TempCompressor = false;
bool Flag_Temp_Descarga = false;

unsigned long valvulaACSStart = 0;
unsigned long PumpStart = 0;
unsigned long BuzzerStart = 0;
unsigned long Ingreso_E7 = 0;
unsigned long Ingreso_E71 = 0;

const uint8_t GAP_ACS = 5;

unsigned long Periodo_Refresco;
unsigned long compressorStart;
unsigned long Salto_E1;
unsigned long dontStuckPumpsStart_activation;
unsigned long dontStuckPumpsStart;
unsigned long Ingreso_E3;

unsigned long Ingreso_Descanso = 0;

volatile int MenuActual;  //"volatile" se debe indicar para variables que se modifican en las rutinas de interrupción

volatile uint8_t Nro_Alarma = 0;
volatile uint8_t SetP_ACS = 0;
volatile uint8_t SetP_ACS_Edit = 0;

uint8_t Alarma_Eeprom;

// FLAGS     //Banderas de uso general para el funcionamiento del programa

bool deltaACSElectricResult = false;
bool Flag_CaudT = false;
bool Flag_CaudH = false;
bool Flag_PresHI = false;
bool Flag_PresLOW = false;

bool Flag_Temp_Adm = false;
volatile bool modoFrio = false;  // Frio = true , Calor = false
volatile bool Alarma_Activa;
bool Flag_Marcha_ON;
bool senal_start;  // senal de marcha, segun modoFrio
// se trabaja con 1 termostato.
bool senal_stop;
volatile bool heating_off = false;
volatile bool Flag_Buzzer;

volatile bool EnableACS = true;
volatile bool EnableACS_DeltaElectrico = true;
volatile bool EnableFlowAlarm;
volatile bool EnableElectricACS = false;

// IMAGENES DE ENTRADAS/SALIDAS
int Valor_DO_Bombas;
int Valor_DO_Calentador;
int Valor_DO_Compressor;
int Valor_DO_VACS;
int Valor_DO_V4V;
int Valor_DO_Buzzer;
