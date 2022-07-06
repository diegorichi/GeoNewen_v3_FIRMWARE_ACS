
#ifndef vars__
#define vars__
#include <TimerOne.h>        //Librería para el control de salidas PWM
#include <avr/wdt.h>         //Libreria para uso de watchdog de Arduino
#include <stdlib.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/*-----------------ESP8266 Serial WiFi Module---------------*/ 
//Informacion para la conexion a internet del modulo ESP8266 a Thingspeak
//los datos de SSID y pass para WIFI ya no son requeridos, 
//la conexion se realiza mediante la app "ESP8266 SmartConfig"

#define IP "54.158.47.123" //"api.thingspeak.com"  // thingspeak.com ip de la pagina web de Thingspeak
extern String api_key_kume;   //key para enviar datos a Thingspeak, se obtiene de la pagina web
extern String api_key_dr;     //key para enviar datos a Thingspeak, se obtiene de la pagina web
extern String local_ip;
/*-----------------------------------------------------------*/

/**************************/
/*DECLARACION DE VARIABLES*/
/**************************/

//PINES DIGITALES

extern const int ACS_EN_Address ; // 21;
extern const int ACS_EN_ELECT_Address ; // 23;

extern uint8_t DI_Temp_in_H[8] ; // {0x28, 0xDA, 0xB6, 0xF8, 0x1A, 0x19, 0x01, 0x8B};      //n1    ok
extern uint8_t DI_Temp_Compresor[8] ; // {0x28, 0xE5, 0xAC, 0x26, 0x1B, 0x19, 0x01, 0x3C}; //n2    ok
extern uint8_t DI_Temp_Admision[8] ; // {0x28, 0x34, 0x76, 0x57, 0x1A, 0x19, 0x01, 0xA0};  //n10   ok
extern uint8_t DI_Temp_out_T[8] ; // {0x28, 0xD6, 0x3C, 0xE0, 0x1B, 0x19, 0x01, 0x0F};     //n7   ok

extern uint8_t DI_Temp_Descarga[8] ; // {0x28, 0x49, 0x4B, 0x01, 0x1C, 0x19, 0x01, 0x1A}; //n11
extern uint8_t DI_Temp_ACS[8] ; // {0x28, 0xAE, 0x16, 0xFF, 0x1B, 0x19, 0x01, 0xD1};      //n12
extern uint8_t DI_Temp_out_H[8] ; // {0x28, 0x87, 0x9F, 0xE9, 0x1B, 0x19, 0x01, 0xF8};    //n13
extern uint8_t DI_Temp_in_T[8] ; // {0x28, 0x83, 0x69, 0x3D, 0x1B, 0x19, 0x01, 0x11};     //n14

// 0x28, 0xAE, 0x16, 0xFF, 0x1B, 0x19, 0x01, 0xD1 }; //n5

extern const int DI_Caud_T ; // 18; //ENTRADAS DE CAUDALIMETROS (no se pueden modificar)
extern const int DI_Caud_H ; // 19;

extern const int DO_Calentador ; // 23; //Compresor
extern const int DO_Comp_01 ; // 25;    //Boombas de circulacion
extern const int DO_Bombas ; // 27;     //Valvula Calefaccion
extern const int DO_Val2 ; // 29;       //V4V
extern const int DO_Val1 ; // 31;       //V ACS

extern const int DO_Triac_01 ; // 11; //Triacs,Pin salida PWM (no se puede modificar)
extern const int DO_Buzzer ; // 12;   //
//extern const int DO_Contraste     ; // 13;  //Control de contraste display (sin uso)

extern const int DI_Marcha_on ; // 33; //Entrada de señal de Marcha
extern const int DI_Pres_HI ; // 35;   //Preostato de alta
extern const int DI_Pres_LOW ; // 37;  //Presotato de baja

extern const int STEP ; // 9;       //Pines para control del EasyDriver
extern const int DIR ; // 8;        //STEP es el pin por donde se envian los pasos para que el motor gire, DIR indica la direccion
extern const int ED_ENABLE ; // 10; //Pin para "dormir" al EasyDriver, evitando consumo de energia

//extern const int DI_Alarma_Trif   ; // 22;  //Pin de entrada a la señal de alarma proveniente de la placa de medicion trifasica

extern unsigned long EntradaConfigWifi ; // 0;
//Pines de comunicación Serie

//PINES ANALOGICOS    //Usados para la lectura de los sensores analogicos LM35 y los sensores de corriente ACS712
extern const int AI_Temp_intX_T ; // A10; //desactivado
extern const int AI_Temp_intX_H ; // A1;  //desactivado
extern const int AI_Temp_Comp_01 ; // A2;
extern const int AI_Temp_ACS ; // A3;
extern const int AI_Temp_out_H ; // A4;
extern const int AI_Temp_in_H ; // A5;
extern const int AI_Temp_out_T ; // A7;
extern const int AI_Temp_in_T ; // A6;
extern const int AI_Temp_Descarga ; // A9;
//extern const int AI_Temp_Admision   ; // A9;
extern const int AI_TI ; // A0;
extern const int AI_Sensor_Tension ; // A11;

//VARIABLES DEL PROGRAMA
extern int Val_Temp_intX_T;
extern int Val_Temp_intX_H;
extern int Val_Temp_Comp_01;
extern int Val_Temp_ACS;
extern int Val_Temp_out_H;
extern int Val_Temp_in_H;
extern int Val_Temp_out_T;
extern int Val_Temp_in_T;
extern int Val_Temp_Descarga;
extern int Val_Temp_Admision;
extern int Valor_ADC ; // 0;
extern int Val_TI;
extern int Valor_Pico_V ; // 0;
extern int descanso ; // 0;
extern int mVperAmp ; // 160;
extern int contador;
extern int Caud_T;
extern int C1_T ; // 0;
extern int C2_T ; // 0;
extern int C3_T ; // 0;
extern int Caud_Tacu ; // 0;
extern int Caud_H;
extern int C1_H ; // 0;
extern int C2_H ; // 0;
extern int C3_H ; // 0;
extern int Caud_Hacu ; // 0;
extern int Estado_Maquina ; // 0;
extern int Estado_Bombas ; // 0;
extern int Cont_CaudT ; // 0;
extern int Cont_CaudH ; // 0;
extern int Cont_Temp_Des ; // 0;
extern int Pos_Valv ; // 0;
extern int Pos_Valv_Edit ; // 0;

extern String W ; // " ";
extern char w;
extern int Estado_ConfigWIFI ; // 0;

//Contadores auxiliares de alarmas
extern int Cont_Temp_intX_T ; // 0;
extern int Cont_Temp_intX_T2 ; // 0;
extern int Cont_Temp_intX_H ; // 0;
extern int Cont_Temp_intX_H2 ; // 0;
extern int Cont_Temp_Comp_01 ; // 0;
extern int Cont_Caud_T ; // 0;
extern int Cont_Caud_H ; // 0;
extern int Cont_Press_HI ; // 0;
extern int Cont_Press_LOW ; // 0;
extern int Cont_Temp_Descarga ; // 0;

extern float Temp_intX_T;
extern float Temp_intX_H;
extern float Temp_Comp_01;
extern float T5_Comp ; // 0;
extern float T4_Comp ; // 0;
extern float T3_Comp ; // 0;
extern float T2_Comp ; // 0;
extern float T1_Comp ; // 0;
extern float Temp_comp_acu ; // 0;

extern float Temp_ACS;
extern float T1_ACS ; // 0;
extern float T2_ACS ; // 0;
extern float T3_ACS ; // 0;
extern float Temp_ACSacu ; // 0;
extern float Temp_out_H;
extern float Temp_in_H;
extern float Temp_out_T;
extern float Temp_in_T;
extern float Temp_Descarga;
extern float Temp_Admision;
extern float TI;
extern float AuxTempH;
extern float AuxTempT;

extern float DC_V ; // 0;
extern float AC_V ; // 0;
extern float Fesc ; // 1.982; //Actualizado el 22/01/18 1.982;
extern float FCal ; // 1.055; //1.77;// caudalimetro  sen - hz21wa                    //1.9 caudalimetros  geo v1.0 y 2.0;
extern float Ef_Termica_1;
extern float Ef_Termica_2;
extern float CosFI ; // 0.90;
extern float FescTI ; // (1024 / 8.24); //Factor de escala del TI
extern float Aporte_E;
extern float Temp_out_Hacu ; // 0;
extern float T1_OH ; // 0;
extern float T2_OH ; // 0;
extern float T3_OH ; // 0;
extern float T1_IH ; // 0;
extern float T2_IH ; // 0;
extern float T3_IH ; // 0;
extern float Temp_in_Hacu ; // 0;
extern float T1_Des ; // 0;
extern float T2_Des ; // 0;
extern float T3_Des ; // 0;
extern float Temp_Descargaacu ; // 0;
extern float Temp_Comp_01aux; //Valores a mostrar de las temperaturas
extern float Temp_out_Haux;
extern float Temp_in_Haux;
extern float Temp_out_Taux;
extern float Temp_in_Taux;
extern float Temp_Admisionaux;
extern float Temp_Descargaaux;
extern float Temp_ACSaux;

extern unsigned long Equipo_en_regimen ; // 230000; //5 minutos para que el equipo este en condiciones nominales
extern unsigned long E1_a_E2 ; // 120000;           //2 minutos para pasar de E1 a E2
extern unsigned long E2_a_E3 ; // 10000;            //10 segundos para pasar de E2 a E3
extern unsigned long Tpo_Mod_Rap ; // 3000;         //5 segundos para la modulacion rapida de la VE
extern unsigned long Tpo_Mod_Lento ; // 120000;     //2min para la modulacion lenta de la VE
extern unsigned long Tpo_Mod_Lento2 ; // 60000;     //60 segundos para la modulacion lenta con SC > 8
extern unsigned long Tpo_Refresco_TS ; // 600000;   //10 minutos de refresco de TS
extern unsigned long Arranque_Frio ; // 600000;     //Periodo de calentamiento de compresor para arranque en frio

extern bool Flag_ACS_EN ; // true;
extern bool Flag_TempComp01 ; // false;
extern bool Flag_Temp_Descarga ; // false;
extern bool Flag_retardo_e7 ; // false;

extern String S_Temp_in_T; //Variables auxiliares para el envío de datos a Thingspeak
extern String S_Temp_out_T;
extern String S_Temp_in_H;
extern String S_Temp_out_H;
extern String S_Temp_Comp_01;
extern String S_A_RMS;
extern String S_Caud_T;
extern String S_Caud_H;
extern String S_Temp_Descarga;
extern String S_Temp_Admision;
extern String S_TI;
extern String S_Temp_intX_T;
extern String S_Temp_intX_H;
extern String S_Sensor_Pres_PSI;
extern String S_Temp_ACS;

extern float V_RMS ; // 0;
extern float A_RMS ; // 0;
extern float Potencia;
extern float Voltage ; // 0;

extern unsigned long RefrescoWifi ; // 0;
extern unsigned long EsperaValv ; // 0;
extern unsigned long Ingreso_E7 ; // 0;
extern unsigned long Ingreso_E71 ; // 0;
extern unsigned long Ingreso_E9 ; // 0;
extern unsigned long Arranque_Equipo ; // 0;
extern unsigned long Periodo_Fin_ACS ; // 0;
extern unsigned long Periodo_Com ; // 0;

extern unsigned long Muestras_Tension ; // 0; //las variables de tipo unsignedlong se usan para periodos de tiempo
extern unsigned long Ventana_Caudal1;
extern unsigned long Ventana_Caudal2;
extern unsigned long PreviousMillis;
extern unsigned long CurrentMillis;
extern unsigned long Periodo_Refresco;
extern unsigned long Desact_Comp;
extern unsigned long Activacion_Comp;
extern unsigned long Apagado_Comp;
extern unsigned long Periodo_Temps;
extern unsigned long Refresco_Caudal;
extern unsigned long Valor_Per_Esp_C;
extern unsigned long Salto_E1;
extern unsigned long Ventana_Alarma_CaudalT;
extern unsigned long Ventana_Alarma_CaudalH;
extern unsigned long Periodo_Refresco_Wifi;
extern unsigned long Activacion_Bombas;
extern unsigned long Periodo_Bombas;
extern unsigned long PrimerEnvio ; // 0;
extern unsigned long Ingreso_E3;
extern unsigned long LecturaDSB;
extern unsigned long PeriodoLeg ; // 0;
extern unsigned long Temp_Leg ; // 0;

extern unsigned long Ingreso_Descanso ; // 0;

extern volatile int MenuActual; //"volatile" se debe indicar para variables que se modifican en las rutinas de interrupción
extern volatile byte Pulsos_Caud_T;
extern volatile byte Pulsos_Caud_H;
extern volatile byte Pulsos_Caud_T_Bis;
extern volatile byte Pulsos_Caud_H_Bis;
extern volatile int Ciclo_Trabajo;
extern volatile int Ciclo_Trabajoedit;
extern volatile int Estado_Comp ; // 0;
extern volatile byte Nro_Alarma ; // 0;
extern volatile byte Nro_Alarma2 ; // 0;
extern volatile byte SetP_ACS ; // 0;
extern volatile byte SetP_ACS_Edit ; // 0;

extern volatile int Per_Esp_C; //Variables modificables por teclado
extern volatile int Var_Mod_2;
extern volatile int Var_Mod_3;

extern volatile int Per_Esp_C_Edit;
extern volatile int Var_Mod_2_Edit;
extern volatile int Var_Mod_3_Edit;

extern byte H; //usados para escribir y leer variables de la memoria EEPROM
extern byte L;
//char s;

extern const int DutyCAddress ; // 0; //Direcciones de memoria EEPROM
extern const int ModeAddress ; // 3;
extern const int Mode_F_Address ; // 4;
extern const int SetP_ACS_Address ; // 5;
extern const int Var_2_Address ; // 7;
extern const int Var_3_Address ; // 9;
extern const int Alarma_Address ; // 11;
extern const int Pos_Valv_Address ; // 13;
extern const int Alarma_EN_Address ; // 15;
extern volatile byte Alarma_Eeprom;




//FLAGS     //Banderas de uso general para el funcionamiento del programa
extern bool Flag_TempIntXT_Baja ; // false;
extern bool Flag_TempIntXT_Alta ; // false;
extern bool Flag_TempIntXH_Baja ; // false;
extern bool Flag_TempIntXH_Alta ; // false;
extern bool Flag_TempTriacs ; // false;
extern bool Flag_ACS_EN_ELECT ; // false;

extern bool flag_dtElectrico_final ; // false;
extern bool Flag_CaudT ; // false;
extern bool Flag_CaudH ; // false;
extern bool Flag_PresHI ; // false;
extern bool Flag_PresLOW ; // false;
extern bool TempIntXTOK ; // false;
extern bool TempIntXHOK ; // false;
extern bool TempsOK ; // false;
extern bool PressOK ; // false;
extern bool Flag_Alarma_General ; // false;
extern bool Flag_Estado_Comp ; // false;
extern bool Flag_Aporte_E ; // false;
extern bool Flag_RetornoLiq ; // false;
extern bool Flag_Temp_Adm ; // false;
extern bool Flag_Temp_Des ; // false;
extern volatile bool ModoFrioCalor ; // false; //Frio ; // true , Calor ; // false
extern bool Alarma_Activa;
extern bool Flag_Marcha_ON;
extern volatile bool Modo_Funcionamiento ; // false; //false ; // Automatico, true ; // Manual
extern volatile bool Flag_Buzzer;
extern volatile bool Flag_Alarma_Caudal;
extern bool Flag_Corriente ; // false;
extern bool Flag_Wifi;
extern bool Flag_ESP;
extern bool Flag_Caldera;
extern bool Flag_Temp_Caldera;
extern bool Flag_Alarma_Trif;
extern bool Flag_Alarma_EN;
extern bool Flag_Primer_Envio ; // false;
extern bool Flag_Temp_Leg ; // false;
extern bool Cal_ACS ; // true;
extern bool Cal ; // true;

//IMAGENES DE ENTRADAS/SALIDAS
extern int Valor_DO_Bombas;
extern int Valor_DO_Calentador;
extern int Valor_DO_Comp_01;
extern int Valor_DO_Marcha_Ext;
extern int Valor_DO_VACS;
extern int Valor_DO_V4V;
extern int Valor_DO_Aux;
extern int SenalComp;

#endif
