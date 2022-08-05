#include "functionsLCDMenu.h"
LiquidCrystal lcd(30, 32, 34, 36, 38, 40, 42, 44, 46, 48); // Se definen los pines RS, EN y D0 a D7 del display (en ese orden)

#define byte uint8_t

// DEFINICIÓN DE CARACTERES (símbolos especiales a mostrar)
byte FlechaAbajo[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B01110,
    B00100 };

byte FlechaArriba[8] = {
    B00100,
    B01110,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000 };

void lcdCreateSpecialChars() {
  lcd.createChar(1, FlechaAbajo); // Se crean los caracteres definidos anteriormente
  lcd.createChar(2, FlechaArriba);
  // lcd.createChar(3, Enter);
  // lcd.createChar(4, GradoC);
}

void lcdRefreshValues() {

  if (MenuActual == 0) // Solo en la ventana de bienvenida se muestra el estado actual del sistema y de la señal de marcha
    RefreshMenuCero();

  if (MenuActual == 10) // Este menú es la primer ventana del monitor de temperaturas y consumo eléctrico
    RefreshMenuUnoCero();

  if (MenuActual == 11) // La visualización de la segunda ventana del monitor depende del modo de funcionamiento, ya que en función de este se cambian de lugar las variables motradas
    RefreshMenuUnoUno();

  if (MenuActual == 20) // Modo Frio / Calor
    RefreshMenuDosCero();

  if (MenuActual == 210) // Editar valor ACS
    RefreshMenuDosUnoCero();

  if (MenuActual == 23) // Alarmas
    RefreshMenuDosTres();

  if (MenuActual == 24) // Funcionamiento Auto/Manual
    RefreshMenuDosCuatro();

  if (MenuActual == 27) // Conexion de WIFI (Conectado/No Conectado)
    RefreshMenuDosSiete();

  if (MenuActual == 28) // Activacion/desactivacion de ACS
    RefreshMenuDosOcho();

  if (MenuActual == 281) // Activacion/desactivacion de delta ACS electrico
    RefreshMenuDosOchoUno();

  if (MenuActual == 29) // Activacion/desactivacion de ACS electrico
    RefreshMenuDosNueve();

  if (MenuActual == 40) // En función al número de alarma devuelto por la función Alarmas(), se muestra el mensaje indicando la causa de la misma
    RefreshMenuCuatroCero();

  if (MenuActual == 50) // En función al número de alarma devuelto por la función Alarmas(), se muestra el mensaje indicando la causa de la misma
    RefreshMenuCincoCero();
}

// Solo en la ventana de bienvenida se muestra el estado actual del sistema y de la señal de marcha
void RefreshMenuCero() {

  lcd.setCursor(19, 2);
  lcd.print(Estado_Maquina);

  lcd.setCursor(19, 1);
  if (digitalRead(DI_Marcha_on) == HIGH)
    lcd.print(F("H"));
  else
    lcd.print(F("L"));

  lcd.setCursor(0, 1);
  if (Flag_Wifi)
    lcd.print(F("W "));
  else
    lcd.print(F("NW"));

  lcd.setCursor(0, 2);
  if (Flag_ESP)
    lcd.print(F("E "));
  else
    lcd.print(F("NE"));
}

// Este menú es la primer ventana del monitor de temperaturas y consumo eléctrico
void RefreshMenuUnoCero() {
  lcd.setCursor(5, 0);
  if (ModoFrioCalor) {
    lcd.print(F("FRIO "));
  }
  else {
    lcd.print(F("CALOR"));
  }
  // lcd.setCursor(5, 1);
  //  lcd.print(Temp_intX_T, 1);
  // lcd.setCursor(15, 1);
  //  lcd.print(Temp_intX_H, 1);
  lcd.setCursor(5, 2);
  lcd.print(Temp_comp_acu, 1);
  lcd.setCursor(15, 2);
  lcd.print(Temp_Admision, 1);
  lcd.setCursor(5, 3);
  lcd.print(Temp_Descargaacu, 1);
  lcd.setCursor(15, 3);
  lcd.print(Temp_ACSacu, 1);
  // lcd.setCursor(16,3);
  // lcd.print(Temp_PCB, 1);
}

// La visualización de la segunda ventana del monitor depende del modo de funcionamiento, ya que en función de este se cambian de lugar las variables motradas
void RefreshMenuUnoUno() {
  lcd.setCursor(5, 0);
  if (ModoFrioCalor) {
    lcd.print(F("FRIO "));
    lcd.setCursor(15, 3);
    lcd.print(F("    "));
    lcd.setCursor(15, 3);
    lcd.print(Caud_Tacu);
  }
  else {
    lcd.print(F("CALOR"));
    lcd.setCursor(15, 3);
    lcd.print(F("    "));
    lcd.setCursor(15, 3);
    lcd.print(Caud_T);
  }
  lcd.setCursor(5, 1);
  lcd.print(Temp_in_Hacu, 1);
  lcd.setCursor(15, 1);
  lcd.print(Temp_out_Hacu, 1);
  lcd.setCursor(5, 2);
  lcd.print(Temp_out_T, 1);
  lcd.setCursor(15, 2);
  lcd.print(Temp_in_T, 1);
  lcd.setCursor(5, 3);
  lcd.print(F("    "));
  lcd.setCursor(5, 3);
  lcd.print(Caud_H);
}

// Modo Frio / Calor
void RefreshMenuDosCero() {
  lcd.setCursor(12, 2);
  if (ModoFrioCalor) {
    lcd.print(F("FRIO "));
  }
  else {
    lcd.print(F("CALOR"));
  }
}

// Editar valor ACS
void RefreshMenuDosUnoCero() {
  lcd.setCursor(14, 1);
  lcd.print(SetP_ACS);
  lcd.setCursor(13, 2);
  lcd.print(SetP_ACS_Edit);
}

// Alarmas
void RefreshMenuDosTres() {
  lcd.setCursor(8, 2);
  if (Flag_Alarma_EN) {
    lcd.print(F("ON "));
  }
  else
    lcd.print(F("OFF"));
}

// Funcionamiento Auto/Manual
void RefreshMenuDosCuatro() {
  lcd.setCursor(10, 2);
  if (Modo_Funcionamiento == false) {
    lcd.print(F("AUTO  "));
  }
  else
    lcd.print(F("MANUAL"));
}

// Conexion de WIFI (Conectado/No Conectado)
void RefreshMenuDosSiete() {
  lcd.setCursor(8, 2);
  if (Flag_Wifi) {
    lcd.print(F("Conectado   "));
  }
  else {
    lcd.print(F("No Conectado"));
  }
  lcd.setCursor(4, 1);
  lcd.print(local_ip);
}

// Activacion/desactivacion de ACS
void RefreshMenuDosOcho() {
  lcd.setCursor(15, 1);
  if (Flag_ACS_EN) {
    lcd.print(F("ON "));
  }
  else
    lcd.print(F("OFF"));
}

// Activacion/desactivacion de delta de ACS electrico final
void RefreshMenuDosOchoUno() {
  lcd.setCursor(12, 1);
  if (Flag_ACS_DT_EN) {
    lcd.print(F("ON "));
  }
  else
    lcd.print(F("OFF"));

  lcd.setCursor(0, 2);
  if (flag_dtElectrico_final) {
    lcd.print(F("ENCENDIDO"));
  }
  else
    lcd.print(F("APAGADO"));
}

// Activacion/desactivacion de ACS electrico
void RefreshMenuDosNueve() {
  lcd.setCursor(15, 1);
  if (Flag_ACS_EN_ELECT) {
    lcd.print(F("ON "));
  }
  else
    lcd.print(F("OFF"));

  lcd.setCursor(0, 2);
  if (Valor_DO_Calentador == HIGH) {
    lcd.print(F("ENCENDIDO"));
  }
  else
    lcd.print(F("APAGADO  "));
}

// En función al número de alarma devuelto por la función Alarmas(), se muestra el mensaje indicando la causa de la misma
void RefreshAlarma(volatile byte _nro_Alarma) {
  lcd.setCursor(0, 1);
  switch (_nro_Alarma) {
  case 0:
    {
      lcd.print(F("No hay alarmas act. "));
      lcd.setCursor(0, 2);
      lcd.print(F("                    "));
    }
    break;
  case 1:
    {
      lcd.print(F("Temp Intercambiador "));
      lcd.setCursor(0, 2);
      lcd.print(F("de Tierra es BAJA   "));
    }
    break;
  case 2:
    {
      lcd.print(F("Temp Intercambiador "));
      lcd.setCursor(0, 2);
      lcd.print(F("de Tierra es ALTA   "));
    }
    break;
  case 3:
    {
      lcd.print(F("Temp Intercambiador "));
      lcd.setCursor(0, 2);
      lcd.print(F("de Hogar es BAJA    "));
    }
    break;
  case 4:
    {
      lcd.print(F("Temp Intercambiador "));
      lcd.setCursor(0, 2);
      lcd.print(F("de Hogar es ALTA    "));
    }
    break;
  case 5:
    {
      lcd.print(F("Temp Admision Gas   "));
      lcd.setCursor(0, 2);
      lcd.print(F("es ALTA             "));
    }
    break;
  case 6:
    {
      lcd.print(F("Temp Compresor es   "));
      lcd.setCursor(0, 2);
      lcd.print(F("ALTA                "));
    }
    break;
  case 7:
    {
      lcd.print(F("Caudal de Tierra es "));
      lcd.setCursor(0, 2);
      lcd.print(F("BAJO                "));
    }
    break;
  case 8:
    {
      lcd.print(F("Caudal de Hogar es  "));
      lcd.setCursor(0, 2);
      lcd.print(F("BAJO                "));
    }
    break;
  case 9:
    {
      lcd.print(F("Presion Pres. HI es "));
      lcd.setCursor(0, 2);
      lcd.print(F("ALTA                "));
    }
    break;
  case 10:
    {
      lcd.print(F("Presion Pres. LOW  "));
      lcd.setCursor(0, 2);
      lcd.print(F("BAJA                "));
    }
    break;
  case 11:
    {
      lcd.print(F("Delta T INTX BAJO  "));
    }
    break;
  case 12:
    {
      lcd.print(F("Corriente compresor"));
      lcd.setCursor(0, 2);
      lcd.print(F("fuera de rango     "));
    }
    break;
  case 13:
    {
      lcd.print(F("Temp Caldera Aux   "));
      lcd.setCursor(0, 2);
      lcd.print(F("es ALTA             "));
    }
    break;
  case 14:
    {
      lcd.print(F("Alarma Modulo Trif."));
    }
    break;
  case 15:
    {
      lcd.print(F("Temp ADM elevada"));
    }
    break;
  case 16:
    {
      lcd.print(F("Bajo aporte de kcal"));
    }
    break;
  case 17:
    {
      lcd.print(F("Retorno liquido comp"));
    }
    break;
  case 18:
    {
      lcd.print(F("Temp DES elevada"));
    }
    break;
  }
}

// En función al número de alarma devuelto por la función Alarmas(), se muestra el mensaje indicando la causa de la misma
void RefreshMenuCuatroCero() {
  RefreshAlarma(Nro_Alarma);
}
// En función al número de alarma devuelto por la función Alarmas(), se muestra el mensaje indicando la causa de la misma
void RefreshMenuCincoCero() {
  Alarma_Eeprom = EEPROM.read(Alarma_Address);
  RefreshAlarma(Alarma_Eeprom);
}

/**********************************/
/*FUNCIONES DE DIBUJO DE PANTALLAS*/
/**********************************/

void beginLcd() {
  lcd.begin(20, 4);
  lcd.clear();
}

void showNavigation() {
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuCero() // Pantalla de Inicio/Bienvenida
{
  beginLcd();
  lcd.print(F("     KUME NEWEN     "));
  lcd.setCursor(0, 1);
  lcd.print(F("   BOMBA DE CALOR   "));
  lcd.setCursor(0, 2);
  lcd.print(F("    GeoNewen v3.0   "));
  lcd.setCursor(0, 3);
  lcd.print(F("   Presione ENTER   "));
}

void MenuUno() // Menú de Monitor de Temps y Consumo de Energía
{
  beginLcd();
  lcd.print(F("MONITOR DE TEMPS"));
  lcd.setCursor(0, 1);
  lcd.print(F("Y CONSUMO DE ENERGIA"));
  showNavigation();
}

void MenuUnoCero() // 1º Ventana de Monitor
{
  beginLcd();
  lcd.print(F("MODO:            "));
  lcd.setCursor(0, 1);
  lcd.print(F("IxT: n/a"));
  lcd.setCursor(10, 1);
  lcd.print(F("IxH: n/a"));
  lcd.setCursor(0, 2);
  lcd.print(F("Com:"));
  lcd.setCursor(10, 2);
  lcd.print(F("Adm:"));
  lcd.setCursor(0, 3);
  lcd.print(F("Des:"));
  lcd.setCursor(10, 3);
  lcd.print(F("ACS:"));
  // lcd.setCursor(10, 3);
  // lcd.print(F("Evap:"));
  showNavigation();
}

void MenuUnoUno() // 2º Ventana de Monitor
{
  beginLcd();
  lcd.print(F("MODO:            "));
  lcd.setCursor(0, 1);
  lcd.print(F("TiH:"));
  lcd.setCursor(10, 1);
  lcd.print(F("ToH:"));
  lcd.setCursor(0, 2);
  lcd.print(F("TiT:"));
  lcd.setCursor(10, 2);
  lcd.print(F("ToT:"));
  lcd.setCursor(0, 3);
  lcd.print(F("CauT:"));
  lcd.setCursor(10, 3);
  lcd.print(F("CauH:"));
  showNavigation();
}

void MenuDos() // Menu de Configuración del Sistema
{
  beginLcd();
  lcd.print(F("CONFIGURACION DEL   "));
  lcd.setCursor(0, 1);
  lcd.print(F("     SISTEMA        "));
  lcd.setCursor(0, 3);
  // lcd.print(F("ENTER para ver..."));
  showNavigation();
}

void MenuDosCero() // Menú de Cambio de Modo de Funcionamiento
{
  beginLcd();
  lcd.print(F("MODO FRIO/CALOR     "));
  lcd.setCursor(0, 2);
  lcd.print(F("Modo Activo:"));
  lcd.setCursor(0, 3);
  lcd.print(F("Enter modifica"));
  showNavigation();

}

void MenuDosUno() // Configuracion de temp de ACS
{
  beginLcd();
  lcd.print(F("CONFIGURACION DE    "));
  lcd.setCursor(0, 1);
  lcd.print(F("TEMPERATURA DE ACS  "));
  lcd.setCursor(0, 3);
  lcd.print(F("Enter modifica"));
  showNavigation();
}

void MenuDosUnoCero() // Seteo de temp de ACS
{
  beginLcd();
  lcd.print(F("MODIFIQUE VALOR     "));
  lcd.setCursor(0, 1);
  lcd.print(F("Valor Actual="));
  lcd.setCursor(0, 2);
  lcd.print(F("Nuevo Valor="));
  lcd.setCursor(0, 3);
  lcd.print(F("Enter modifica"));
  showNavigation();
}

void MenuDosTres() // Habilitacion alarmas de caudal
{
  beginLcd();
  lcd.print(F("HABILITACION DE     "));
  lcd.setCursor(0, 1);
  lcd.print(F("ALARMAS DE CAUDAL   "));
  lcd.setCursor(0, 2);
  lcd.print(F("ALARMAS:   "));
  lcd.setCursor(0, 3);
  lcd.print(F("Enter modifica"));
  showNavigation();
}

void MenuDosCuatro() // Cambio a modo manual o automatico
{
  beginLcd();
  lcd.print(F("CAMBIO MODO MANUAL "));
  lcd.setCursor(0, 1);
  lcd.print(F("O AUTOMATICO        "));
  lcd.setCursor(0, 2);
  lcd.print(F("MODO ACT:"));
  lcd.setCursor(0, 3);
  lcd.print(F("Enter modifica"));
  showNavigation();
}

void MenuDosSiete() // configuracion de WIFI
{
  beginLcd();
  lcd.print(F("CONFIGURACION WIFI"));
  lcd.setCursor(0, 1);
  lcd.print(F("IP:               "));
  lcd.setCursor(0, 2);
  lcd.print(F("ESTADO:           "));
  lcd.setCursor(0, 3);
  lcd.print(F("Enter modifica"));
  showNavigation();
}

void MenuDosOcho() // Habilitacion de Generacion de ACS
{
  beginLcd();
  lcd.print(F("HABILITACION DE     "));
  lcd.setCursor(0, 1);
  lcd.print(F("GENERACION ACS:     "));
  lcd.setCursor(0, 3);
  lcd.print(F("Enter modifica"));
  showNavigation();
}

void MenuDosOchoUno() // Habilitacion de Delta electrico
{
  beginLcd();
  lcd.print(F("ACS CON DELTA ELECT "));
  lcd.setCursor(0, 1);
  lcd.print(F("Habilitado:       "));
  lcd.setCursor(0, 3);
  lcd.print(F("Enter modifica"));
  showNavigation();
}

void MenuDosNueve() // ACS electrico
{
  beginLcd();
  lcd.print(F("HABILITACION DE     "));
  lcd.setCursor(0, 1);
  lcd.print(F("ACS ELECTRICO:      "));
  lcd.setCursor(0, 3);
  lcd.print(F("Enter modifica"));
  showNavigation();
}

/*
void MenuTres() // Menú de Configuración de Caldera Eléctrica
{
  beginLcd();
  lcd.print(F("CALDERA AUXILIAR"));
  showNavigation();
}

void MenuTresCero() // Ventana de Variación del Ciclo de Trabajo de la Caldera Eléctrica
{
  beginLcd();
  lcd.print(F("Presione ENTER para "));
  lcd.setCursor(0, 2);
  lcd.print(F("encender/apagar     "));
  lcd.setCursor(0, 3);
  lcd.print(F("ESTADO:             "));
}
*/
void MenuCuatro() // Menú de Visualización de Alarmas
{
  beginLcd();
  lcd.print(F("MONITOR DE ALARMAS"));
  showNavigation();
}

void MenuCuatroCero() // Visualización de Alarmas Activas
{
  beginLcd();
  lcd.print(F("ALARMA ACTIVA:"));
  lcd.setCursor(0, 3);
  lcd.print(F("Resetear Sist=>ENTER"));
}

void MenuCinco() // REGISTRO DE ALARMAS
{
  beginLcd();
  lcd.print(F("REGISTRO DE ALARMAS"));
  showNavigation();
}

void MenuCincoCero() {
  beginLcd();
  lcd.print(F("ULTIMA ALARMA ACTIVA"));
  lcd.setCursor(0, 3);
  lcd.print(F("ENTER para borrar..."));
}
