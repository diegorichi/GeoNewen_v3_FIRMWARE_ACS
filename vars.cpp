#include "vars.h"

/**************************/
/*DECLARACION DE VARIABLES*/
/**************************/

// PINES DIGITALES

// 0x28, 0xAE, 0x16, 0xFF, 0x1B, 0x19, 0x01, 0xD1 }; //n5

const int diCaudT = 18;     // Entrada de caudalimetro tierra
const int diCaudH = 19;     // Entrada de caudalimetro hogar
const int diMarchaOn = 33;  // Entrada de señal de Marcha
const int diPresHi = 35;    // Preostato de alta
const int diPresLow = 37;   // Presotato de baja

const int doCalentador = 23;    // Calentador
const int doCompressor = 25;    // Compresor
const int doBombas = 27;        // Bombas
const int doValvula4Vias = 29;  // Valvula 4 Vias
const int doValvulaAcs = 31;    // Valvula ACS

const int doTriac01 = 11;  // Triacs, Pin salida PWM (no se puede modificar)
const int doBuzzer = 12;    // Pin de salida de buzzer

volatile int estadoMaquina = 0;

// VARIABLES DEL PROGRAMA

int caudT;
int c1T = 0;
int c2T = 0;
int c3T = 0;
int caudTacu = 0;

int caudH;
int c1H = 0;
int c2H = 0;
int c3H = 0;
int caudHacu = 0;

int contTempDes = 0;

// Contadores auxiliares de alarmas

int contTempCompressor = 0;
int contPressHi = 0;
int contPressLow = 0;
int contTempDescarga = 0;

float tempCompressor;
float t5Comp = 0;
float t4Comp = 0;
float t3Comp = 0;
float t2Comp = 0;
float t1Comp = 0;
float tempCompressorAcu = 0;

float tempAcs;
float t1Acs = 0;
float t2Acs = 0;
float t3Acs = 0;
float tempAcsAcu = 0;

float tempOutH;
float tempInH;
float tempOutT;
float tempInT;
float tempDescarga;
float tempAdmision;

float t1Oh = 0;
float t2Oh = 0;
float t3Oh = 0;
float tempOutHacu = 0;
float t1Ih = 0;
float t2Ih = 0;
float t3Ih = 0;
float tempInHacu = 0;
float t1Des = 0;
float t2Des = 0;
float t3Des = 0;
float tempDescargaAcu = 0;

bool flagTempCompressor = false;
bool flagTempDescarga = false;

unsigned long valvulaAcsStart = 0;
unsigned long pumpStart = 0;
unsigned long ingresoE7 = 0;
unsigned long ingresoE71 = 0;

const uint8_t GAP_ACS = 5;

unsigned long periodoRefresco;
unsigned long compressorStart;
unsigned long saltoE1;
unsigned long dontStuckPumpsStartActivation;
unsigned long dontStuckPumpsStart;
unsigned long ingresoE3;

unsigned long ingresoDescanso = 0;

volatile MenuId menuActual = MENU_HOME;

volatile uint8_t nroAlarma = 0;
volatile uint8_t acsSetpoint = 0;
volatile uint8_t acsSetpointEdit = 0;

uint8_t alarmaEeprom;

// FLAGS     //Banderas de uso general para el funcionamiento del programa

bool deltaAcsElectricResult = false;
bool flagCaudT = false;
bool flagCaudH = false;
bool flagPresHi = false;
bool flagPresLow = false;

bool flagTempAdm = false;
volatile bool modoFrio = false;  // Frio = true , Calor = false
volatile bool alarmaActiva;
bool flagMarchaOn;
bool senalStart;  // senal de marcha, segun modoFrio
// se trabaja con 1 termostato.
bool senalStop;
volatile bool heatingOff = false;
volatile bool flagBuzzer;

volatile bool enableAcs = true;
volatile bool enableAcsDeltaElectrico = true;
volatile bool enableFlowAlarm;
volatile bool enableElectricAcs = false;

// IMAGENES DE ENTRADAS/SALIDAS
int valorDoBombas;
int valorDoCalentador;
int valorDoCompressor;
int valorDoVacs;
int valorDoV4v;
volatile int valorDoBuzzer;
