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

  if (MenuActual == 12) // La tercera ventana muestra la eficiencia térmica
    RefreshMenuUnoDos();

  /*if (MenuActual == 13) //La cuarta ventana muestra los parámetros eléctricos del sistema
    {
    lcd.setCursor(3, 1);
    //lcd.print(AC_V,0);
    //lcd.setCursor(12,1);
    lcd.print(A_RMS, 2);
    lcd.setCursor(3, 2);
    lcd.print("    ");
    lcd.setCursor(3, 2);
    lcd.print(Potencia, 0);
    }*/

  if (MenuActual == 20) // Modo Frio / Calor
    RefreshMenuDosCero();

  if (MenuActual == 210) // Editar valor ACS
    RefreshMenuDosUnoCero();

  if (MenuActual == 23) // Alarmas
    RefreshMenuDosTres();

  if (MenuActual == 24) // Funcionamiento Auto/Manual
    RefreshMenuDosCuatro();

  //if (MenuActual == 26) // Pos_Valv
  //  RefreshMenuDosSeis();

  if (MenuActual == 27) // Conexion de WIFI (Conectado/No Conectado)
    RefreshMenuDosSiete();

  if (MenuActual == 28) // Activacion/desactivacion de ACS
    RefreshMenuDosOcho();

  if (MenuActual == 29) // Activacion/desactivacion de ACS electrico
    RefreshMenuDosNueve();

  if (MenuActual == 30) // Estado caldera (ENCENDIDA/APAGADA)
    RefreshMenuTresCero();

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
    lcd.print("H");
  else
    lcd.print("L");

  lcd.setCursor(0, 1);
  if (Flag_Wifi)
    lcd.print("W ");
  else
    lcd.print("NW");

  lcd.setCursor(0, 2);
  if (Flag_ESP)
    lcd.print("E ");
  else
    lcd.print("NE");
}

// Este menú es la primer ventana del monitor de temperaturas y consumo eléctrico
void RefreshMenuUnoCero() {
  lcd.setCursor(5, 0);
  if (ModoFrioCalor) {
    lcd.print("FRIO ");
  }
  else {
    lcd.print("CALOR");
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
    lcd.print("FRIO ");
    lcd.setCursor(15, 3);
    lcd.print("    ");
    lcd.setCursor(15, 3);
    lcd.print(Caud_Tacu);
  }
  else {
    lcd.print("CALOR");
    lcd.setCursor(15, 3);
    lcd.print("    ");
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
  lcd.print("    ");
  lcd.setCursor(5, 3);
  lcd.print(Caud_H);
}

// La tercera ventana muestra la eficiencia térmica
void RefreshMenuUnoDos() {
  lcd.setCursor(3, 1);
  lcd.print("    ");
  lcd.setCursor(3, 1);
  lcd.print(TI, 1);
  lcd.setCursor(3, 2);
  lcd.print("    ");
  lcd.setCursor(3, 2);
  lcd.print(Potencia, 0);
}

// Modo Frio / Calor
void RefreshMenuDosCero() {
  lcd.setCursor(12, 2);
  if (ModoFrioCalor) {
    lcd.print("FRIO ");
  }
  else {
    lcd.print("CALOR");
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
    lcd.print("ON ");
  }
  else
    lcd.print("OFF");
}

// Funcionamiento Auto/Manual
void RefreshMenuDosCuatro() {
  lcd.setCursor(10, 2);
  if (Modo_Funcionamiento == false) {
    lcd.print("AUTO  ");
  }
  else
    lcd.print("MANUAL");
}

// Pos_Valv
//void RefreshMenuDosSeis() {
//  lcd.setCursor(12, 2);
//  lcd.print(Pos_Valv);
//}

// Conexion de WIFI (Conectado/No Conectado)
void RefreshMenuDosSiete() {
  lcd.setCursor(8, 2);
  if (Flag_Wifi) {
    lcd.print("Conectado   ");
  }
  else {
    lcd.print("No Conectado");
  }
  lcd.setCursor(4, 1);
  lcd.print(local_ip);
}

// Activacion/desactivacion de ACS
void RefreshMenuDosOcho() {
  lcd.setCursor(15, 1);
  if (Flag_ACS_EN) {
    lcd.print("ON ");
  }
  else
    lcd.print("OFF");
}

// Activacion/desactivacion de ACS electrico
void RefreshMenuDosNueve() {
  lcd.setCursor(15, 1);
  if (Flag_ACS_EN_ELECT) {
    lcd.print("ON ");
  }
  else
    lcd.print("OFF");
}

// Estado caldera (ENCENDIDA/APAGADA)
void RefreshMenuTresCero() {
  lcd.setCursor(8, 3);
  if (Flag_Caldera) {
    lcd.print("ENCENDIDA");
  }
  else
    lcd.print("APAGADA  ");
}

// En función al número de alarma devuelto por la función Alarmas(), se muestra el mensaje indicando la causa de la misma
void RefreshAlarma(volatile byte _nro_Alarma) {
  lcd.setCursor(0, 1);
  switch (_nro_Alarma) {
  case 0:
    {
      lcd.print("No hay alarmas act. ");
      lcd.setCursor(0, 2);
      lcd.print("                    ");
    }
    break;
  case 1:
    {
      lcd.print("Temp Intercambiador ");
      lcd.setCursor(0, 2);
      lcd.print("de Tierra es BAJA   ");
    }
    break;
  case 2:
    {
      lcd.print("Temp Intercambiador ");
      lcd.setCursor(0, 2);
      lcd.print("de Tierra es ALTA   ");
    }
    break;
  case 3:
    {
      lcd.print("Temp Intercambiador ");
      lcd.setCursor(0, 2);
      lcd.print("de Hogar es BAJA    ");
    }
    break;
  case 4:
    {
      lcd.print("Temp Intercambiador ");
      lcd.setCursor(0, 2);
      lcd.print("de Hogar es ALTA    ");
    }
    break;
  case 5:
    {
      lcd.print("Temp Admision Gas   ");
      lcd.setCursor(0, 2);
      lcd.print("es ALTA             ");
    }
    break;
  case 6:
    {
      lcd.print("Temp Compresor es   ");
      lcd.setCursor(0, 2);
      lcd.print("ALTA                ");
    }
    break;
  case 7:
    {
      lcd.print("Caudal de Tierra es ");
      lcd.setCursor(0, 2);
      lcd.print("BAJO                ");
    }
    break;
  case 8:
    {
      lcd.print("Caudal de Hogar es  ");
      lcd.setCursor(0, 2);
      lcd.print("BAJO                ");
    }
    break;
  case 9:
    {
      lcd.print("Presion Pres. HI es ");
      lcd.setCursor(0, 2);
      lcd.print("ALTA                ");
    }
    break;
  case 10:
    {
      lcd.print("Presion Pres. LOW  ");
      lcd.setCursor(0, 2);
      lcd.print("BAJA                ");
    }
    break;
  case 11:
    {
      lcd.print("Delta T INTX BAJO  ");
    }
    break;
  case 12:
    {
      lcd.print("Corriente compresor");
      lcd.setCursor(0, 2);
      lcd.print("fuera de rango     ");
    }
    break;
  case 13:
    {
      lcd.print("Temp Caldera Aux   ");
      lcd.setCursor(0, 2);
      lcd.print("es ALTA             ");
    }
    break;
  case 14:
    {
      lcd.print("Alarma Modulo Trif.");
    }
    break;
  case 15:
    {
      lcd.print("Temp ADM elevada");
    }
    break;
  case 16:
    {
      lcd.print("Bajo aporte de kcal");
    }
    break;
  case 17:
    {
      lcd.print("Retorno liquido comp");
    }
    break;
  case 18:
    {
      lcd.print("Temp DES elevada");
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
  lcd.print("     KUME NEWEN     ");
  lcd.setCursor(0, 1);
  lcd.print("   BOMBA DE CALOR   ");
  lcd.setCursor(0, 2);
  lcd.print("    GeoNewen v3.0   ");
  lcd.setCursor(0, 3);
  lcd.print("   Presione ENTER   ");
}

void MenuUno() // Menú de Monitor de Temps y Consumo de Energía
{
  beginLcd();
  lcd.print("MONITOR DE TEMPS");
  lcd.setCursor(0, 1);
  lcd.print("Y CONSUMO DE ENERGIA");
  showNavigation();
}

void MenuUnoCero() // 1º Ventana de Monitor
{
  beginLcd();
  lcd.print("MODO:            ");
  lcd.setCursor(0, 1);
  lcd.print("IxT: n/a");
  lcd.setCursor(10, 1);
  lcd.print("IxH: n/a");
  lcd.setCursor(0, 2);
  lcd.print("Com:");
  lcd.setCursor(10, 2);
  lcd.print("Adm:");
  lcd.setCursor(0, 3);
  lcd.print("Des:");
  lcd.setCursor(10, 3);
  lcd.print("ACS:");
  // lcd.setCursor(10, 3);
  // lcd.print("Evap:");
  showNavigation();
}

void MenuUnoUno() // 2º Ventana de Monitor
{
  beginLcd();
  lcd.print("MODO:            ");
  lcd.setCursor(0, 1);
  lcd.print("TiH:");
  lcd.setCursor(10, 1);
  lcd.print("ToH:");
  lcd.setCursor(0, 2);
  lcd.print("TiT:");
  lcd.setCursor(10, 2);
  lcd.print("ToT:");
  lcd.setCursor(0, 3);
  lcd.print("CauT:");
  lcd.setCursor(10, 3);
  lcd.print("CauH:");
  showNavigation();
}

void MenuUnoDos() // 3º Ventana de Monitor
{
  beginLcd();
  lcd.print(" MONITOR COMPRESOR ");
  lcd.setCursor(0, 1);
  lcd.print("I=");
  lcd.setCursor(8, 1);
  lcd.print("A");
  lcd.setCursor(0, 2);
  lcd.print("P=");
  lcd.setCursor(8, 2);
  lcd.print("W");
  lcd.setCursor(0, 3);
  lcd.print("Pres Evap(PSI):     ");
  showNavigation();
}

void MenuDos() // Menu de Configuración del Sistema
{
  beginLcd();
  lcd.print("CONFIGURACION DEL   ");
  lcd.setCursor(0, 1);
  lcd.print("     SISTEMA        ");
  lcd.setCursor(0, 3);
  // lcd.print("ENTER para ver...");
  showNavigation();
}

void MenuDosCero() // Menú de Cambio de Modo de Funcionamiento
{
  beginLcd();
  lcd.print("MODO FRIO/CALOR     ");
  lcd.setCursor(0, 2);
  lcd.print("Modo Activo:");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  showNavigation();

}

void MenuDosUno() // Configuracion de temp de ACS
{
  beginLcd();
  lcd.print("CONFIGURACION DE    ");
  lcd.setCursor(0, 1);
  lcd.print("TEMPERATURA DE ACS  ");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  showNavigation();
}

void MenuDosUnoCero() // Seteo de temp de ACS
{
  beginLcd();
  lcd.print("MODIFIQUE VALOR     ");
  lcd.setCursor(0, 1);
  lcd.print("Valor Actual=");
  lcd.setCursor(0, 2);
  lcd.print("Nuevo Valor=");
  lcd.setCursor(0, 3);
  lcd.print("CONFIRME CON ENTER  ");
  showNavigation();
}

void MenuDosTres() // Habilitacion alarmas de caudal
{
  beginLcd();
  lcd.print("HABILITACION DE     ");
  lcd.setCursor(0, 1);
  lcd.print("ALARMAS DE CAUDAL   ");
  lcd.setCursor(0, 2);
  lcd.print("ALARMAS:   ");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  showNavigation();
}

void MenuDosCuatro() // Cambio a modo manual o automatico
{
  beginLcd();
  lcd.print("CAMBIO MODO MANUAL ");
  lcd.setCursor(0, 1);
  lcd.print("O AUTOMATICO        ");
  lcd.setCursor(0, 2);
  lcd.print("MODO ACT:");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  showNavigation();
}

void MenuDosSeis() // Control manual de valvula
{
  beginLcd();
  lcd.print("CONTROL MANUAL");
  lcd.setCursor(0, 1);
  lcd.print("DE VALVULA          ");
  lcd.setCursor(0, 2);
  lcd.print("Pos Actual=");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  showNavigation();
}

void MenuDosSiete() // configuracion de WIFI
{
  beginLcd();
  lcd.print("CONFIGURACION WIFI");
  lcd.setCursor(0, 1);
  lcd.print("IP:               ");
  lcd.setCursor(0, 2);
  lcd.print("ESTADO: ");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  showNavigation();
}

void MenuDosOcho() // Habilitacion de Generacion de ACS
{
  beginLcd();
  lcd.print("HABILITACION DE     ");
  lcd.setCursor(0, 1);
  lcd.print("GENERACION ACS:     ");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  showNavigation();
}

void MenuDosNueve() // ACS electrico
{
  beginLcd();
  lcd.print("HABILITACION DE     ");
  lcd.setCursor(0, 1);
  lcd.print("ACS ELECTRICO:      ");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  showNavigation();
}

void MenuTres() // Menú de Configuración de Caldera Eléctrica
{
  beginLcd();
  lcd.print("CALDERA AUXILIAR");
  showNavigation();
}

void MenuTresCero() // Ventana de Variación del Ciclo de Trabajo de la Caldera Eléctrica
{
  beginLcd();
  lcd.print("Presione ENTER para ");
  lcd.setCursor(0, 2);
  lcd.print("encender/apagar     ");
  lcd.setCursor(0, 3);
  lcd.print("ESTADO:             ");
}

void MenuCuatro() // Menú de Visualización de Alarmas
{
  beginLcd();
  lcd.print("MONITOR DE ALARMAS");
  showNavigation();
}

void MenuCuatroCero() // Visualización de Alarmas Activas
{
  beginLcd();
  lcd.print("ALARMA ACTIVA:");
  lcd.setCursor(0, 3);
  lcd.print("Resetear Sist=>ENTER");
}

void MenuCinco() // REGISTRO DE ALARMAS
{
  beginLcd();
  lcd.print("REGISTRO DE ALARMAS");
  showNavigation();
}

void MenuCincoCero() {
  beginLcd();
  lcd.print("ULTIMA ALARMA ACTIVA");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para borrar...");
}
