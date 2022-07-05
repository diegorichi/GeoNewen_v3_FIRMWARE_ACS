#include "functionsLCDMenu.h"
LiquidCrystal lcd(30, 32, 34, 36, 38, 40, 42, 44, 46, 48); //Se definen los pines RS, EN y D0 a D7 del display (en ese orden)

#define byte uint8_t


//DEFINICIÓN DE CARACTERES (símbolos especiales a mostrar)
byte FlechaAbajo[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B01110,
    B00100};

byte FlechaArriba[8] = {
    B00100,
    B01110,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000};

/*byte Enter[8] = {
        B00000,
        B00101,
        B01101,
        B11111,
        B01100,
        B00100,
        B00000,
        B00000
  };

  byte GradoC[8] = {
        B00110,
        B01001,
        B01001,
        B00110,
        B00000,
        B00000,
        B00000,
        B00000
  };*/

void lcdCreateSpecialChars()
{
  lcd.createChar(1, FlechaAbajo); //Se crean los caracteres definidos anteriormente
  lcd.createChar(2, FlechaArriba);
  //lcd.createChar(3, Enter);
  //lcd.createChar(4, GradoC);
}

void lcdRefreshValues()
{

  if (MenuActual == 0) //Solo en la ventana de bienvenida se muestra el estado actual del sistema y de la señal de marcha
    {
      lcd.setCursor(19, 2);
      lcd.print(Estado_Maquina);
      lcd.setCursor(19, 1);
      if (digitalRead(DI_Marcha_on) == HIGH)
      {
        lcd.print("H");
      }
      else if (digitalRead(DI_Marcha_on) == LOW)
      {
        lcd.print("L");
      }
      lcd.setCursor(0, 1);
      if (Flag_Wifi == true)
      {
        lcd.print("W");
      }
      else if (Flag_Wifi == false)
      {
        lcd.print("NW");
      }
      lcd.setCursor(0, 2);
      if (Flag_ESP == true)
      {
        lcd.print("E");
      }
      else if (Flag_ESP == false)
      {
        lcd.print("NE");
      }
    }

  if (MenuActual == 10) //Este menú es la primer ventana del monitor de temperaturas y consumo eléctrico
  {
    lcd.setCursor(5, 0);
    if (ModoFrioCalor == true)
    {
      lcd.print("FRIO ");
    }
    else
    {
      lcd.print("CALOR");
    }
    lcd.setCursor(5, 1);
    //lcd.print(Temp_intX_T, 1);
    lcd.setCursor(15, 1);
    //lcd.print(Temp_intX_H, 1);
    lcd.setCursor(5, 2);
    lcd.print(Temp_comp_acu, 1);
    lcd.setCursor(15, 2);
    lcd.print(Temp_Admision, 1);
    lcd.setCursor(5, 3);
    lcd.print(Temp_Descargaacu, 1);
    lcd.setCursor(15, 3);
    lcd.print(Temp_ACSacu, 1);
    //lcd.setCursor(16,3);
    //lcd.print(Temp_PCB, 1);
  }

  if (MenuActual == 11) //La visualización de la segunda ventana del monitor depende del modo de funcionamiento, ya que en función de este se cambian de lugar las variables motradas
  {
    if (ModoFrioCalor == true)
    {
      lcd.setCursor(5, 0);
      lcd.print("FRIO ");
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
      lcd.setCursor(15, 3);
      lcd.print("    ");
      lcd.setCursor(15, 3);
      lcd.print(Caud_Tacu);
    }
    else
    {
      lcd.setCursor(5, 0);
      lcd.print("CALOR");
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
      lcd.setCursor(15, 3);
      lcd.print("    ");
      lcd.setCursor(15, 3);
      lcd.print(Caud_T);
    }
  }

  if (MenuActual == 12) //La tercera ventana muestra la eficiencia térmica
  {
    lcd.setCursor(3, 1);
    lcd.print("    ");
    lcd.setCursor(3, 1);
    lcd.print(TI, 1);
    lcd.setCursor(3, 2);
    lcd.print("    ");
    lcd.setCursor(3, 2);
    lcd.print(Potencia, 0);
  }

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

  if (MenuActual == 200)  // Modo Frio / Calor
  {
    lcd.setCursor(10, 3);
    if (ModoFrioCalor == true)
    {
      lcd.print("FRIO ");
    }
    else
    {
      lcd.print("CALOR");
    }
  }

  if (MenuActual == 210)  // Editar valor ACS
  {
    lcd.setCursor(14, 1);
    lcd.print(SetP_ACS);
    lcd.setCursor(13, 2);
    lcd.print(SetP_ACS_Edit);
  }

  if (MenuActual == 230)  // Alarmas
  {
    lcd.setCursor(0, 1);
    if (Flag_Alarma_EN == true)
    {
      lcd.print("Alarmas habilitadas ");
    }
    else
      lcd.print("Alarmas desactivadas");
  }

  if (MenuActual == 24) // Funcionamiento Auto/Manual
  {
    lcd.setCursor(10, 2);
    if (Modo_Funcionamiento == false)
    {
      lcd.print("AUTO  ");
    }
    else
      lcd.print("MANUAL");
  }

  if (MenuActual == 260)  // Pos_Valv 
  {
    lcd.setCursor(11, 2);
    lcd.print(Pos_Valv);
  }

  if (MenuActual == 270)  // Conexion de WIFI (Conectado/No Conectado)
  {
    lcd.setCursor(8, 3);
    if (Flag_Wifi == true)
    {
      lcd.print("Conectado:  ");
    }
    else{
      lcd.print("No Conectado");
    }
    lcd.setCursor(8, 4);
    lcd.print(local_ip);
  }

  if (MenuActual == 280) //Activacion/desactivacion de ACS
  {
    lcd.setCursor(0, 1);
    if (Flag_ACS_EN == true)
    {
      lcd.print("ACS habilitado ");
    }
    else
      lcd.print("ACS desactivado");
  }

  if (MenuActual == 290) //Activacion/desactivacion de ACS electrico
  {
    lcd.setCursor(0, 1);
    if (Flag_ACS_EN_ELECT == true)
    {
      lcd.print("ACS ELECT ON ");
    }
    else
      lcd.print("ACS ELECT OFF");
  }

  if (MenuActual == 30) //Estado caldera (ENCENDIDA/APAGADA)
  {
    lcd.setCursor(8, 3);
    if (Flag_Caldera == true)
    {
      lcd.print("ENCENDIDA");
    }
    else
      lcd.print("APAGADA  ");
  }

  if (MenuActual == 40) //En función al número de alarma devuelto por la función Alarmas(), se muestra el mensaje indicando la causa de la misma
  {
    lcd.setCursor(0, 1);
    switch (Nro_Alarma)
    {
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
      lcd.print("Temp Triacs es ALTA ");
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

  if (MenuActual == 50) //En función al número de alarma devuelto por la función Alarmas(), se muestra el mensaje indicando la causa de la misma
  {
    lcd.setCursor(0, 1);
    Alarma_Eeprom = EEPROM.read(Alarma_Address);
    Nro_Alarma2 = Alarma_Eeprom;
    switch (Nro_Alarma2)
    {
    case 0:
    {
      lcd.print("No hay alarmas ");
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
      lcd.print("Corriente Compresor");
      lcd.setCursor(0, 2);
      lcd.print("fuera de rango     ");
    }
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
}


/****************/
/*FUNCIONES*/
/****************/

void MenuCero() //Pantalla de Inicio/Bienvenida
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     KUME NEWEN     ");
  lcd.setCursor(0, 1);
  lcd.print("   BOMBA DE CALOR   ");
  lcd.setCursor(0, 2);
  lcd.print("    GeoNewen v3.0   ");
  lcd.setCursor(0, 3);
  lcd.print("   Presione ENTER   ");
}

void MenuUno() //Menú de Monitor de Temps y Consumo de Energía
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("MONITOR DE TEMPS");
  lcd.setCursor(0, 1);
  lcd.print("Y CONSUMO DE ENERGIA");
  lcd.setCursor(0, 3);
  //lcd.print("ENTER para ver...");
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuUnoCero() //1º Ventana de Monitor
{
  lcd.begin(20, 4);
  lcd.clear();
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
  //lcd.setCursor(10, 3);
  //lcd.print("Evap:");
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuUnoUno() //2º Ventana de Monitor
{
  lcd.begin(20, 4);
  lcd.clear();
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
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuUnoDos() //3º Ventana de Monitor
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print(" MONITOR COMPRESOR "); //editar parametros a mostrar
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
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuDos() //Menu de Configuración del Sistema
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("CONFIGURACION DEL   ");
  lcd.setCursor(0, 1);
  lcd.print("     SISTEMA        ");
  lcd.setCursor(0, 3);
  //lcd.print("ENTER para ver...");
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuDosCero() //Menú de Cambio de Modo de Funcionamiento
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("MODO FRIO/CALOR     ");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para ver...");
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuDosCeroCero() //Ventana de Cambio de Modo de Funcionamiento
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("Presione ENTER para  ");
  lcd.setCursor(0, 1);
  lcd.print("cambiar modo de func.");
  lcd.setCursor(0, 3);
  lcd.print("Modo Act:");
}

void MenuDosUno()  // Configuracion de temp de ACS
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("CONFIGURACION DE    ");
  lcd.setCursor(0, 1);
  lcd.print("PARAMETRO: Temp ACS ");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuDosUnoCero()  //Seteo de temp de ACS
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("MODIFIQUE VALOR     ");
  lcd.setCursor(0, 1);
  lcd.print("Valor Actual=");
  lcd.setCursor(0, 2);
  lcd.print("Nuevo Valor=");
  lcd.setCursor(0, 3);
  lcd.print("CONFIRME CON ENTER  ");
}

void MenuDosDos()  // var_mod_2
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("CONFIGURACION DE    ");
  lcd.setCursor(0, 1);
  lcd.print("PARAMETRO: Var_Mod_2");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuDosDosCero()
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("MODIFIQUE VALOR     ");
  lcd.setCursor(0, 1);
  lcd.print("Valor Actual=");
  lcd.setCursor(0, 2);
  lcd.print("Nuevo Valor=");
  lcd.setCursor(0, 3);
  lcd.print("CONFIRME CON ENTER  ");
}

void MenuDosTres()   // Habilitacion alarmas de caudal
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("HABILITACION DE     ");
  lcd.setCursor(0, 1);
  lcd.print("ALARMAS DE CAUDAL   ");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuDosTresCero()
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("OPCION ACTUAL:");
  lcd.setCursor(0, 3);
  lcd.print("MODIFIQUE CON ENTER ");
}

void MenuDosCuatro() // Cambio a modo manual o automatico
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("CAMBIO MODO MANUAL ");
  lcd.setCursor(0, 1);
  lcd.print("O AUTOMATICO        ");
  lcd.setCursor(0, 2);
  lcd.print("MODO ACT:");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuDosCinco()  // Control manual de compresor
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("CONTROL MANUAL");
  lcd.setCursor(0, 1);
  lcd.print("DE COMPRESOR          ");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuDosSeis()  // Control manual de valvula
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("CONTROL MANUAL");
  lcd.setCursor(0, 1);
  lcd.print("DE VALVULA          ");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuDosSeisCero()
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("TECLAS ARRIBA Y");
  lcd.setCursor(0, 1);
  lcd.print("ABAJO P/MODIFICAR");
  lcd.setCursor(0, 2);
  lcd.print("Pos Actual=");
  lcd.setCursor(0, 3);
  lcd.print("ATRAS para salir");
}

void MenuDosSiete() //confiuracion de WIFI
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("CONFIGURACION WIFI");
  lcd.setCursor(0, 1);
  lcd.print("ip:"+local_ip);
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  lcd.setCursor(19, 0);
  lcd.write(2);
}

void MenuDosSieteCero()
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("Presione ENTER para");
  lcd.setCursor(0, 1);
  lcd.print("iniciar config.          ");
  lcd.setCursor(0, 3);
  lcd.print("ESTADO: ");
}

void MenuDosOcho() // Habilitacion de Generacion de ACS 
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("HABILITACION DE     ");
  lcd.setCursor(0, 1);
  lcd.print("GENERACION ACS   ");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuDosOchoCero()
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("OPCION ACTUAL:");
  lcd.setCursor(0, 3);
  lcd.print("MODIFIQUE CON ENTER ");
}

void MenuDosNueve()  // ACS electrico
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("HABILITACION DE     ");
  lcd.setCursor(0, 1);
  lcd.print("ACS ELECTRICO");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para modificar");
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuDosNueveCero()
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("OPCION ACTUAL:");
  lcd.setCursor(0, 3);
  lcd.print("MODIFIQUE CON ENTER ");
}

void MenuTres() //Menú de Configuración de Caldera Eléctrica
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("CALDERA AUXILIAR");
  lcd.setCursor(0, 3);
  //lcd.print("ENTER para ver...");
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuTresCero() //Ventana de Variación del Ciclo de Trabajo de la Caldera Eléctrica
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("Presione ENTER para ");
  lcd.setCursor(0, 2);
  lcd.print("encender/apagar     ");
  lcd.setCursor(0, 3);
  lcd.print("ESTADO:             ");
}

void MenuCuatro() //Menú de Visualización de Alarmas
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("MONITOR DE ALARMAS");
  lcd.setCursor(0, 3);
  //lcd.print("ENTER para ver...");
  lcd.setCursor(19, 0);
  lcd.write(2);
  lcd.setCursor(19, 3);
  lcd.write(1);
}

void MenuCuatroCero() //Visualización de Alarmas Activas
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("ALARMA ACTIVA:");
  lcd.setCursor(0, 3);
  lcd.print("Resetear Sist=>ENTER");
}

void MenuCinco() // REGISTRO DE ALARMAS
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("REGISTRO DE ALARMAS");
  lcd.setCursor(0, 3);
  //lcd.print("ENTER para ver...");
  lcd.setCursor(19, 0);
  lcd.write(2);
}

void MenuCincoCero()
{
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("ULTIMA ALARMA ACTIVA");
  lcd.setCursor(0, 3);
  lcd.print("ENTER para borrar...");
}
