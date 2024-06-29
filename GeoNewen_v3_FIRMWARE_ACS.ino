#include "vars.h"
#include "keyboard.h"
#include "functionsLCDMenu.h"
#include "kume_eeprom.h"
#include "thingspeak.h"
#include "machine_control.h"
#include "connectivity.h"
#include "machine_control.h"
#include "measurement_and_calculations.h"
#include "stateMachine.h"

/*******/
/*SETUP*/
/*******/

void setup()
{ // Inicializacion de I/O y variables generales

  wdt_disable();

  keyboardSetup(); // setup de pin modes y demas

  setupDigitalInputs(); // setup de los pines de entrada

  setupDigitalOuputs(); // setup de los pines de salida

  initializeDigitalOuputs();

  attachInterrupt(1, AtencionTeclado, FALLING); // Asignación de Interrupciones (se define el número de la interrupción, no del pin; la rutina de interrupción y el modo de activación)
  attachInterrupt(4, Caudal1, FALLING);         // Pin 19
  attachInterrupt(5, Caudal2, FALLING);         // Pin 18

  lcdCreateSpecialChars();

  Caud_T = 0; // Inicialización de variables
  Caud_H = 0;
  Pulsos_Caud_T = 0;
  Pulsos_Caud_H = 0;
  Ventana_Caudal1 = 0;
  Ventana_Caudal2 = 0;
  Estado_Maquina = 0;
  Estado_Bombas = 0;
  Estado_Comp = 0;
  Activacion_Comp = 0;
  Periodo_Refresco_Wifi = 0;
  Estado_ConfigWIFI = 0;
  Activacion_Bombas = 0;
  Periodo_Bombas = 0;
  A_RMS = 0;
  Valor_DO_VACS = HIGH;
  Valor_DO_V4V = LOW;

  Flag_TempIntXT_Baja = false; // inicializacion  de banderas
  Flag_TempIntXT_Alta = false;
  Flag_TempIntXH_Baja = false;
  Flag_TempIntXH_Alta = false;
  Flag_TempTriacs = false;
  Flag_TempComp01 = false;
  Flag_CaudT = false;
  Flag_CaudH = false;
  Flag_PresHI = false;
  Flag_PresLOW = false;
  Flag_Alarma_General = false;
  ModoFrioCalor = false;
  Modo_Funcionamiento = false;

  Cont_Temp_intX_T = 0;
  Cont_Temp_intX_T2 = 0;
  Cont_Temp_intX_H = 0;
  Cont_Temp_intX_H2 = 0;
  Cont_Temp_Comp_01 = 0;
  Cont_Caud_T = 0;
  Cont_Caud_H = 0;
  Cont_Press_HI = 0;
  Cont_Press_LOW = 0;
  W = "";
  w = ' ';

  Timer1.initialize(100000);

  Serial.begin(115200);
  Serial3.begin(115200);

  setupConnectivity();

  EEPROMLectura(); // Carga parametros guardados en la memoria EEPROM

  if (SetP_ACS < MIN_ACS)
  {
    SetP_ACS = MIN_ACS;
  }
  if (SetP_ACS > MAX_ACS)
  {
    SetP_ACS = MAX_ACS;
  }
  SetP_ACS_Edit = SetP_ACS;

  MenuActual = 0;
  MenuCero();

  wdt_enable(WDTO_8S);
}

void mainLoop()
{
  // CÁLCULO DE TEMPERATURAS, CAUDALES, EFICIENCIA TÉRMICA Y CONSUMO DE ENERGÍA

  temperatureMeasurement();

  flowsCalculation(); // Determinación de Caudal

  thermalEfficiencyCalculation(); // Cálculo de Eficiencia Térmica

  powerCalculation(); // Cálculo de la Potecia

  checkEspWifiConnected(); // Verificacion de conexion a Wifi

  sendAndReceiveDataCloud(); // Envio de datos a ThingSpeak

  wdt_reset();

  refreshDataToShow(); // INFORMACIÓN A REFRESCAR (depende del menú en el que nos encontremos)

  //***************************
  // COMPROBACIONES DE SEGURIDAD (Aquí se determinan las posibles causas de alarmas
  //***************************

  flowControl(); // Control de Caudales

  temperatureControl(); // Control de Temperaturas

  presureControl(); // Comprobación de Presiones

  auxiliaryACSHeatingControl(); // Control de calentamiento ACS auxiliar con cartucho electrico.

  // TRANSICIÓN DE ESTADOS  //Definición del funcionamiento del equipo

  processStartStopSignal(); // determina si la bomba tiene senal de marcha del termostato

  stateMachine0();  // Estado inicial del sistema, tanto el compresor como las bombas de circulación están apagados
  stateMachine1();  // Aquí se espera la señal de Marcha_ON para iniciar la operacion del sistema
  stateMachine2();  // Arranque Compresor y Bombas
  stateMachine3();  // Este es el estado final del sistema, donde se controlan las condiciones de alarma
  stateMachine4();  // Estado de Alarma
  stateMachine6();  // Estado de descanso
  stateMachine7();  // Generacion ACS
  stateMachine71(); // Generacion ACS: Estado con bombas andando y compresor apagado
  stateMachine8();  // Estado de transicion al finalizar la generacion de ACS y el sistema esta en modo frio, evita circular agua muy caliente al circuito frio

  // IMAGEN DE SALIDAS
  digitalWrite(DO_Bombas, Valor_DO_Bombas);
  digitalWrite(DO_Calentador, Valor_DO_Calentador);
  digitalWrite(DO_Val2, Valor_DO_V4V);
  digitalWrite(DO_Comp_01, Valor_DO_Comp_01);
  digitalWrite(DO_Val1, Valor_DO_VACS);

  wdt_reset();
}

/***************/
/*LAZO INFINITO*/
/***************/

void loop()
{

  if (Estado_ConfigWIFI == 0)
  {
    mainLoop();
  }
  else if (Estado_ConfigWIFI == 1) // Esto se ejecuta cuando se activa la configuracion WIFI, el resto del codigo no se ejecuta hasta que se sale de este modo
  {
    configWifi();
  } // Fin del estado config WIFI

} // Fin del loop
