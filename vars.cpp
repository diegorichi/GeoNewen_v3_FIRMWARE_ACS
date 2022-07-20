#include "vars.h"

/*-----------------ESP8266 Serial WiFi Module---------------*/ //Informacion para la conexion a internet del modulo ESP8266 a Thingspeak
//#define SSID "KumeNewenLLC"     // "SSID-WiFiname"            //los datos de SSID y pass ya no son requeridos, la conexion se realiza mediante la app "ESP8266 SmartConfig"
//#define PASS "newenkume"        // "password"
//#define IP "184.106.153.149"                     // thingspeak.com ip                //ip de la pagina web de Thingspeak
//#define IP "api.thingspeak.com" //"54.158.47.123"  // api.thingspeak.com
String api_key_kume = "UML8C4KGN6IVTJE7"; //key para enviar datos a Thingspeak, se obtiene de la pagina web
String api_key_dr = "230X8WDK4WACGI95"; //key para enviar datos a Thingspeak, se obtiene de la pagina web
String local_ip = "0.0.0.0";
/*-----------------------------------------------------------*/

/**************************/
/*DECLARACION DE VARIABLES*/
/**************************/

//PINES DIGITALES

const int ACS_EN_Address = 21;
const int ACS_DT_EN_Address = 19;
const int ACS_EN_ELECT_Address = 23;

uint8_t DI_Temp_in_H[8] = { 0x28, 0xDA, 0xB6, 0xF8, 0x1A, 0x19, 0x01, 0x8B };      //n1    ok
uint8_t DI_Temp_Compresor[8] = { 0x28, 0xE5, 0xAC, 0x26, 0x1B, 0x19, 0x01, 0x3C }; //n2    ok
uint8_t DI_Temp_Admision[8] = { 0x28, 0x34, 0x76, 0x57, 0x1A, 0x19, 0x01, 0xA0 };  //n10   ok
uint8_t DI_Temp_out_T[8] = { 0x28, 0xD6, 0x3C, 0xE0, 0x1B, 0x19, 0x01, 0x0F };     //n7   ok

uint8_t DI_Temp_Descarga[8] = { 0x28, 0x49, 0x4B, 0x01, 0x1C, 0x19, 0x01, 0x1A }; //n11
uint8_t DI_Temp_ACS[8] = { 0x28, 0xAE, 0x16, 0xFF, 0x1B, 0x19, 0x01, 0xD1 };      //n12
uint8_t DI_Temp_out_H[8] = { 0x28, 0x87, 0x9F, 0xE9, 0x1B, 0x19, 0x01, 0xF8 };    //n13
uint8_t DI_Temp_in_T[8] = { 0x28, 0x83, 0x69, 0x3D, 0x1B, 0x19, 0x01, 0x11 };     //n14

// 0x28, 0xAE, 0x16, 0xFF, 0x1B, 0x19, 0x01, 0xD1 }; //n5

const int DI_Caud_T = 18; //ENTRADAS DE CAUDALIMETROS (no se pueden modificar)
const int DI_Caud_H = 19;

const int DO_Calentador = 23; //Compresor
const int DO_Comp_01 = 25;    //Boombas de circulacion
const int DO_Bombas = 27;     //Valvula Calefaccion
const int DO_Val2 = 29;       //V4V
const int DO_Val1 = 31;       //V ACS

const int DO_Triac_01 = 11; //Triacs,Pin salida PWM (no se puede modificar)
const int DO_Buzzer = 12;   //
//const int DO_Contraste     = 13;  //Control de contraste display (sin uso)

const int DI_Marcha_on = 33; //Entrada de señal de Marcha
const int DI_Pres_HI = 35;   //Preostato de alta
const int DI_Pres_LOW = 37;  //Presotato de baja

const int STEP = 9;       //Pines para control del EasyDriver
const int DIR = 8;        //STEP es el pin por donde se envian los pasos para que el motor gire, DIR indica la direccion
const int ED_ENABLE = 10; //Pin para "dormir" al EasyDriver, evitando consumo de energia

//const int DI_Alarma_Trif   = 22;  //Pin de entrada a la señal de alarma proveniente de la placa de medicion trifasica

unsigned long EntradaConfigWifi = 0;
//Pines de comunicación Serie

//PINES ANALOGICOS    //Usados para la lectura de los sensores analogicos LM35 y los sensores de corriente ACS712
const int AI_Temp_intX_T = A10; //desactivado
const int AI_Temp_intX_H = A1;  //desactivado
const int AI_Temp_Comp_01 = A2;
const int AI_Temp_ACS = A3;
const int AI_Temp_out_H = A4;
const int AI_Temp_in_H = A5;
const int AI_Temp_out_T = A7;
const int AI_Temp_in_T = A6;
const int AI_Temp_Descarga = A9;
//const int AI_Temp_Admision   = A9;
const int AI_TI = A0;
const int AI_Sensor_Tension = A11;

//VARIABLES DEL PROGRAMA
int Val_Temp_intX_T;
int Val_Temp_intX_H;
int Val_Temp_Comp_01;
int Val_Temp_ACS;
int Val_Temp_out_H;
int Val_Temp_in_H;
int Val_Temp_out_T;
int Val_Temp_in_T;
int Val_Temp_Descarga;
int Val_Temp_Admision;
int Valor_ADC = 0;
int Val_TI;
int Valor_Pico_V = 0;
int descanso = 0;
int mVperAmp = 160;
int contador;
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
int Estado_Maquina = 0;
int Estado_Bombas = 0;
int Cont_CaudT = 0;
int Cont_CaudH = 0;
int Cont_Temp_Des = 0;
int Pos_Valv = 0;
int Pos_Valv_Edit = 0;

String W = " ";
char w;
int Estado_ConfigWIFI = 0;

//Contadores auxiliares de alarmas
int Cont_Temp_intX_T = 0;
int Cont_Temp_intX_T2 = 0;
int Cont_Temp_intX_H = 0;
int Cont_Temp_intX_H2 = 0;
int Cont_Temp_Comp_01 = 0;
int Cont_Caud_T = 0;
int Cont_Caud_H = 0;
int Cont_Press_HI = 0;
int Cont_Press_LOW = 0;
int Cont_Temp_Descarga = 0;

float Temp_intX_T;
float Temp_intX_H;
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
float TI;
float AuxTempH;
float AuxTempT;

float DC_V = 0;
float AC_V = 0;
float Fesc = 1.982; //Actualizado el 22/01/18 1.982;
float FCal = 1.055; //1.77;// caudalimetro  sen - hz21wa                    //1.9 caudalimetros  geo v1.0 y 2.0;
float Ef_Termica_1;
float Ef_Termica_2;
float CosFI = 0.90;
float FescTI = (1024 / 8.24); //Factor de escala del TI
float Aporte_E;
float Temp_out_Hacu = 0;
float T1_OH = 0;
float T2_OH = 0;
float T3_OH = 0;
float T1_IH = 0;
float T2_IH = 0;
float T3_IH = 0;
float Temp_in_Hacu = 0;
float T1_Des = 0;
float T2_Des = 0;
float T3_Des = 0;
float Temp_Descargaacu = 0;
float Temp_Comp_01aux; //Valores a mostrar de las temperaturas
float Temp_out_Haux;
float Temp_in_Haux;
float Temp_out_Taux;
float Temp_in_Taux;
float Temp_Admisionaux;
float Temp_Descargaaux;
float Temp_ACSaux;

unsigned long Equipo_en_regimen = 230000; //5 minutos para que el equipo este en condiciones nominales
unsigned long E1_a_E2 = 120000;           //2 minutos para pasar de E1 a E2
unsigned long E2_a_E3 = 10000;            //10 segundos para pasar de E2 a E3
unsigned long Tpo_Mod_Rap = 3000;         //5 segundos para la modulacion rapida de la VE
unsigned long Tpo_Mod_Lento = 120000;     //2min para la modulacion lenta de la VE
unsigned long Tpo_Mod_Lento2 = 60000;     //60 segundos para la modulacion lenta con SC > 8
unsigned long Tpo_Refresco_TS = 600000;   //10 minutos de refresco de TS
unsigned long Arranque_Frio = 600000;     //Periodo de calentamiento de compresor para arranque en frio

bool Flag_ACS_EN = true;
bool Flag_ACS_DT_EN = true;
bool Flag_TempComp01 = false;
bool Flag_Temp_Descarga = false;
bool Flag_retardo_e7 = false;

String S_Temp_in_T; //Variables auxiliares para el envío de datos a Thingspeak
String S_Temp_out_T;
String S_Temp_in_H;
String S_Temp_out_H;
String S_Temp_Comp_01;
String S_A_RMS;
String S_Caud_T;
String S_Caud_H;
String S_Temp_Descarga;
String S_Temp_Admision;
String S_TI;
String S_Temp_intX_T;
String S_Temp_intX_H;
String S_Sensor_Pres_PSI;
String S_Temp_ACS;

float V_RMS = 0;
float A_RMS = 0;
float Potencia;
float Voltage = 0;

unsigned long RefrescoWifi = 0;
unsigned long EsperaValv = 0;
unsigned long Ingreso_E7 = 0;
unsigned long Ingreso_E71 = 0;
unsigned long Ingreso_E9 = 0;
unsigned long Arranque_Equipo = 0;
unsigned long Periodo_Fin_ACS = 0;
unsigned long Periodo_Com = 0;

unsigned long Muestras_Tension = 0; //las variables de tipo unsignedlong se usan para periodos de tiempo
unsigned long Ventana_Caudal1;
unsigned long Ventana_Caudal2;
unsigned long PreviousMillis;
unsigned long CurrentMillis;
unsigned long Periodo_Refresco;
unsigned long Desact_Comp;
unsigned long Activacion_Comp;
unsigned long Apagado_Comp;
unsigned long Periodo_Temps;
unsigned long Refresco_Caudal;
unsigned long Valor_Per_Esp_C;
unsigned long Salto_E1;
unsigned long Ventana_Alarma_CaudalT;
unsigned long Ventana_Alarma_CaudalH;
unsigned long Periodo_Refresco_Wifi;
unsigned long Activacion_Bombas;
unsigned long Periodo_Bombas;
unsigned long PrimerEnvio = 0;
unsigned long Ingreso_E3;
unsigned long LecturaDSB;
unsigned long PeriodoLeg = 0;
unsigned long Temp_Leg = 0;

unsigned long Ingreso_Descanso = 0;

volatile int MenuActual; //"volatile" se debe indicar para variables que se modifican en las rutinas de interrupción
volatile byte Pulsos_Caud_T;
volatile byte Pulsos_Caud_H;
volatile byte Pulsos_Caud_T_Bis;
volatile byte Pulsos_Caud_H_Bis;
volatile int Ciclo_Trabajo;
volatile int Ciclo_Trabajoedit;
volatile int Estado_Comp = 0;
volatile byte Nro_Alarma = 0;
volatile byte SetP_ACS = 0;
volatile byte SetP_ACS_Edit = 0;

volatile int Per_Esp_C; //Variables modificables por teclado
volatile int Var_Mod_2;
volatile int Var_Mod_3;

volatile int Per_Esp_C_Edit;
volatile int Var_Mod_2_Edit;
volatile int Var_Mod_3_Edit;

byte H; //usados para escribir y leer variables de la memoria EEPROM
byte L;
//char s;

const int DutyCAddress = 0; //Direcciones de memoria EEPROM
const int ModeAddress = 3;
const int Mode_F_Address = 4;
const int SetP_ACS_Address = 5;
const int Var_2_Address = 7;
const int Var_3_Address = 9;
const int Alarma_Address = 11;
const int Pos_Valv_Address = 13;
const int Alarma_EN_Address = 15;
volatile byte Alarma_Eeprom;




//FLAGS     //Banderas de uso general para el funcionamiento del programa
bool Flag_TempIntXT_Baja = false;
bool Flag_TempIntXT_Alta = false;
bool Flag_TempIntXH_Baja = false;
bool Flag_TempIntXH_Alta = false;
bool Flag_TempTriacs = false;
bool Flag_ACS_EN_ELECT = false;

bool flag_dtElectrico_final = false;
bool Flag_CaudT = false;
bool Flag_CaudH = false;
bool Flag_PresHI = false;
bool Flag_PresLOW = false;
bool TempIntXTOK = false;
bool TempIntXHOK = false;
bool TempsOK = false;
bool PressOK = false;
bool Flag_Alarma_General = false;
bool Flag_Estado_Comp = false;
bool Flag_Aporte_E = false;
bool Flag_RetornoLiq = false;
bool Flag_Temp_Adm = false;
bool Flag_Temp_Des = false;
volatile bool ModoFrioCalor = false; //Frio = true , Calor = false
bool Alarma_Activa;
bool Flag_Marcha_ON;
volatile bool Modo_Funcionamiento = false; //false = Automatico, true = Manual
volatile bool Flag_Buzzer;
volatile bool Flag_Alarma_Caudal;
bool Flag_Corriente = false;
bool Flag_Wifi;
bool Flag_ESP;
bool Flag_Caldera;
bool Flag_Temp_Caldera;
bool Flag_Alarma_Trif;
bool Flag_Alarma_EN;
bool Flag_Primer_Envio = false;
bool Flag_Temp_Leg = false;
bool Cal_ACS = true;
bool Cal = true;

//IMAGENES DE ENTRADAS/SALIDAS
int Valor_DO_Bombas;
int Valor_DO_Calentador;
int Valor_DO_Comp_01;
int Valor_DO_Marcha_Ext;
int Valor_DO_VACS;
int Valor_DO_V4V;
int Valor_DO_Aux;
int SenalComp;
