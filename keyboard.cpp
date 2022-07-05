#include "keyboard.h"

const int DI_Teclado_Arriba = 7; //BOTONES DE TECLADO (no se pueden modificar)
const int DI_Teclado_Abajo = 5;
const int DI_Teclado_Enter = 6;
const int DI_Teclado_Atras = 4;
volatile bool BotonArriba = false; //Variables auxiliares para el funcionamiento del teclado, usadas en la rutina de atencion de teclado
volatile bool BotonAtras = false;
volatile bool BotonAbajo = false;
volatile bool BotonEnter = false;
volatile int BotonArribaT = false;
volatile int BotonAtrasT = false;
volatile int BotonAbajoT = false;
volatile int BotonEnterT = false;



void AtencionTeclado() //Función de Navegación entre Menús y Modificación de Parámetros
{
  BotonArribaT = digitalRead(DI_Teclado_Arriba);
  BotonAbajoT = digitalRead(DI_Teclado_Abajo);
  BotonEnterT = digitalRead(DI_Teclado_Enter);
  BotonAtrasT = digitalRead(DI_Teclado_Atras);

  if (BotonArribaT == LOW)
  {
    BotonArriba = true;
  }
  else
    BotonArriba = false;

  if (BotonAbajoT == LOW)
  {
    BotonAbajo = true;
  }
  else
    BotonAbajo = false;

  if (BotonEnterT == LOW)
  {
    BotonEnter = true;
  }
  else
    BotonEnter = false;

  if (BotonAtrasT == LOW)
  {
    BotonAtras = true;
  }
  else
    BotonAtras = false;

  for (int j = 0; j < 100; j++) //Debuoncing
  {
  }

  /*if(BotonArriba || BotonAbajo || BotonEnter || BotonAtras)
    {
     while( digitalRead(DI_Teclado_Arriba) || digitalRead(DI_Teclado_Abajo) || digitalRead(DI_Teclado_Enter) || digitalRead(DI_Teclado_Atras));
     delay(200);
    }*/

  if (BotonEnter == true)
  {
    digitalWrite(DO_Buzzer, HIGH);
    delay(300);
    digitalWrite(DO_Buzzer, LOW);
    switch (MenuActual)
    {
    case 0:
    {
      MenuActual = 1;
      MenuUno();
    }
    break;
    case 1:
    {
      MenuActual = 10;
      MenuUnoCero();
    }
    break;
    case 2:
    {
      MenuActual = 20;
      MenuDosCero();
    }
    break;
    case 3:
    {
      MenuActual = 30;
      MenuTresCero();
    }
    break;
    case 4:
    {
      MenuActual = 40;
      MenuCuatroCero();
    }
    break;
    case 5:
    {
      MenuActual = 50;
      MenuCincoCero();
    }
    break;
    case 20:
    {
      if (Estado_Maquina == 1) //|| Estado_Maquina == 3
      {
        FrioCalor();
        EEPROMUpdate();
      }      
      //MenuActual = 200;
      //MenuDosCeroCero();
    }
    break;
    /*case 200:
    {
      if (Estado_Maquina == 1) //|| Estado_Maquina == 3
      {
        FrioCalor();
        EEPROMUpdate();
      }
    }
    break;
    */
    case 21:
    {
      MenuActual = 210;
      MenuDosUnoCero();
    }
    break;
    case 210:
    {
      SetP_ACS = SetP_ACS_Edit;
      EEPROMUpdate();
    }
    break;
    /*case 22:
    {
      MenuActual = 220;
      MenuDosDosCero();
    }
    break;
    */
    /*case 220:
    {
      SetP_ACS = SetP_ACS_Edit;
      EEPROMUpdate();
    }
    break;
    */
    case 23:
    {
      Flag_Alarma_EN = !Flag_Alarma_EN;
      EEPROMUpdate();
      //MenuActual = 230;
      //MenuDosTresCero();
    }
    break;
    /*case 230:
    {
      Flag_Alarma_EN = !Flag_Alarma_EN;
      EEPROMUpdate();
    }
    break;
    */
    case 24:
    {
      Modo_Funcionamiento = !Modo_Funcionamiento;
      EEPROMUpdate();
    }
    break;
    case 25:
    {
      if (Modo_Funcionamiento == true && Estado_Maquina == 0)
      {
        SenalComp = !SenalComp;
      }
    }
    break;
    case 26:
    {
      MenuActual = 260;
      MenuDosSeisCero();
      digitalWrite(ED_ENABLE, LOW);
      EEPROMLectura();
    }
    break;
    case 260:
    {
      //          Referenciar = true;
    }
    break;
    case 27:
    {
      Estado_ConfigWIFI = 1;
      EntradaConfigWifi = millis();
      Serial3.println("AT+CWSTARTSMART");
      //MenuActual = 270;
      //MenuDosSieteCero();
    }
    break;
    /*case 270:
    {
      Estado_ConfigWIFI = 1;
      EntradaConfigWifi = millis();
      Serial3.println("AT+CWSTARTSMART");
    }
    break;
    */
    case 28:
    {
      Flag_ACS_EN = !Flag_ACS_EN;
      EEPROMUpdate();
      //MenuActual = 280;
      //MenuDosOchoCero();
    }
    break;
    /*case 280:
    {
      Flag_ACS_EN = !Flag_ACS_EN;
      EEPROMUpdate();
    }
    break;
    */
    case 29:
    {
      Flag_ACS_EN_ELECT = !Flag_ACS_EN_ELECT;
      EEPROMUpdate();
      //MenuActual = 290;
      //MenuDosNueveCero();
    }
    break;
    /*case 290:
    {
      Flag_ACS_EN_ELECT = !Flag_ACS_EN_ELECT;
      EEPROMUpdate();
    }
    break;
    */
    case 30:
    {
      Estado_Maquina = Flag_Caldera ? 0 : 5;
      Flag_Caldera = !Flag_Caldera;
    }
    break; //Ciclo_Trabajo = Ciclo_Trabajoedit; EEPROMUpdate();
    case 40:
    {
      Flag_Alarma_General = false;
      Nro_Alarma = 0;
      digitalWrite(DO_Buzzer, LOW);
      Alarma_Activa = false;
      ResetFlags();
      Flag_Buzzer = false;
      Estado_Maquina = 0;
    }
    break;
    case 50:
    {
      EEPROM.write(Alarma_Address, 0);
    }
    break;
    }
  }

  if (BotonAbajo == true)
  {
    digitalWrite(DO_Buzzer, HIGH);
    delay(300);
    digitalWrite(DO_Buzzer, LOW);
    switch (MenuActual)
    {
    case 1:
    {
      MenuActual = 2;
      MenuDos();
    }
    break;
    case 10:
    {
      MenuActual = 11;
      MenuUnoUno();
    }
    break;
    case 11:
    {
      MenuActual = 12;
      MenuUnoDos();
    }
    break;
    case 2:
    {
      MenuActual = 3;
      MenuTres();
    }
    break;
    case 3:
    {
      MenuActual = 4;
      MenuCuatro();
    }
    break;
    case 4:
    {
      MenuActual = 5;
      MenuCinco();
    }
    break;
    case 20:
    {
      MenuActual = 21;
      MenuDosUno();
    }
    break;
    case 21:
    {
      MenuActual = 23;
      MenuDosTres();
    }
    break;
    case 210:
    {
      SetP_ACS_Edit = SetP_ACS_Edit - 1;
      if (SetP_ACS_Edit < 30)
      {
        SetP_ACS_Edit = 30;
      }
    }
    break;
    /*case 22:
    {
      MenuActual = 23;
      MenuDosTres();
    }
    break;
    */
    /*case 220:
    {
      SetP_ACS_Edit = SetP_ACS_Edit - 1;
      if (SetP_ACS_Edit < 30)
      {
        SetP_ACS_Edit = 30;
      }
    }
    break;
    */
    case 23:
    {
      MenuActual = 24;
      MenuDosCuatro();
    }
    break;
    case 24:
    {
      /*MenuActual = 25;
      MenuDosCinco();
    }
    break;
    case 25:
    {*/
      MenuActual = 26;
      MenuDosSeis();
    }
    break;
    case 26:
    {
      MenuActual = 27;
      MenuDosSiete();
    }
    break;
    case 27:
    {
      MenuActual = 28;
      MenuDosOcho();
    }
    break;
    case 28:
    {
      MenuActual = 29;
      MenuDosNueve();
    }
    break;
    case 260:
    {
      //    Paso_Abrir = true;
    }
    break;
    case 40:
    {
      digitalWrite(DO_Buzzer, LOW);
    }
    break;
    }
  }

  if (BotonArriba == true)
  {
    digitalWrite(DO_Buzzer, HIGH);
    delay(300);
    digitalWrite(DO_Buzzer, LOW);
    switch (MenuActual)
    {
    case 5:
    {
      MenuActual = 4;
      MenuCuatro();
    }
    break;
    case 4:
    {
      MenuActual = 3;
      MenuTres();
    }
    break;
    case 3:
    {
      MenuActual = 2;
      MenuDos();
    }
    break;
    case 2:
    {
      MenuActual = 1;
      MenuUno();
    }
    break;
    case 11:
    {
      MenuActual = 10;
      MenuUnoCero();
    }
    break;
    case 12:
    {
      MenuActual = 11;
      MenuUnoUno();
    }
    break;
    case 260:
    {
      //          Paso_Cerrar = true;
    }
    break;
    case 29:
    {
      MenuActual = 28;
      MenuDosOcho();
    }
    break;
    case 28:
    {
      MenuActual = 27;
      MenuDosSiete();
    }
    break;
    case 27:
    {
      MenuActual = 26;
      MenuDosSeis();
    }
    break;
    case 26:
    {
      /*MenuActual = 25;
      MenuDosCinco();
    }
    break;
    case 25:
    {*/
      MenuActual = 24;
      MenuDosCuatro();
    }
    break;
    case 24:
    {
      MenuActual = 23;
      MenuDosTres();
    }
    break;
    case 23:
    {
      MenuActual = 21;
      MenuDosUno();
    }
    break;
    //case 22: {MenuActual=21; MenuDosUno();}break;
    case 21:
    {
      MenuActual = 20;
      MenuDosCero();
    }
    break;
    case 210:
    {
      SetP_ACS_Edit = SetP_ACS_Edit + 1;
      if (SetP_ACS_Edit > 48)
      {
        SetP_ACS_Edit = 48;
      }
    }
    break;
    /*case 220:
    {
      SetP_ACS_Edit = SetP_ACS_Edit + 1;
      if (SetP_ACS_Edit > 48)
      {
        SetP_ACS_Edit = 48;
      }
    }
    break;
    */
    case 40:
    {
      digitalWrite(DO_Buzzer, LOW);
    }
    break;
    }
  }

  if (BotonAtras == true)
  {
    digitalWrite(DO_Buzzer, HIGH);
    delay(300);
    digitalWrite(DO_Buzzer, LOW);
    switch (MenuActual)
    {
    case 0:
    {
    }
    break;
    case 10:
    {
      MenuActual = 1;
      MenuUno();
    }
    break;
    case 11:
    {
      MenuActual = 1;
      MenuUno();
    }
    break;
    case 12:
    {
      MenuActual = 1;
      MenuUno();
    }
    break;
    case 13:
    {
      MenuActual = 1;
      MenuUno();
    }
    break;
    case 20:
    {
      MenuActual = 2;
      MenuDos();
    }
    break;
    /*case 200:
    {
      MenuActual = 20;
      MenuDosCero();
    }
    break;
    */
    case 21:
    {
      MenuActual = 2;
      MenuDos();
    }
    break;
    /*case 22:
    {
      MenuActual = 2;
      MenuDos();
    }
    break;
    */
    case 23:
    {
      MenuActual = 2;
      MenuDos();
    }
    break;
    case 24:
    {
      MenuActual = 2;
      MenuDos();
    }
    break;
    case 25:
    {
      MenuActual = 2;
      MenuDos();
    }
    break;
    case 26:
    {
      MenuActual = 2;
      MenuDos();
    }
    break;
    case 27:
    {
      MenuActual = 2;
      MenuDos();
    }
    break;
    case 28:
    {
      MenuActual = 2;
      MenuDos();
    }
    break;
    case 29:
    {
      MenuActual = 2;
      MenuDos();
    }
    break;
    /*case 220:
    {
      MenuActual = 22;
      MenuDosDos();
    }
    break;
    */
    /*case 230:
    {
      MenuActual = 23;
      MenuDosTres();
    }
    break;
    */
    case 260:
    {
      MenuActual = 26;
      MenuDosSeis();
      digitalWrite(ED_ENABLE, HIGH);
      EEPROMUpdate();
    }
    break;
    case 270:
    {
      Estado_ConfigWIFI = 0;
      MenuActual = 27;
      MenuDosSiete();
    }
    break;
    /*case 280:
    {
      MenuActual = 28;
      MenuDosOcho();
    }
    break;
    case 290:
    {
      MenuActual = 29;
      MenuDosNueve();
    }
    break;
    */
    case 30:
    {
      MenuActual = 3;
      MenuTres();
    }
    break;
    case 40:
    {
      MenuActual = 4;
      MenuCuatro();
    }
    break;
    case 50:
    {
      MenuActual = 5;
      MenuCinco();
    }
    break;
    case 210:
    {
      MenuActual = 21;
      MenuDosUno();
    }
    break;
    case 5:
    {
      MenuActual = 0;
      MenuCero();
      digitalWrite(DO_Buzzer, HIGH);
      delay(500);
      digitalWrite(DO_Buzzer, LOW);
    }
    break;
    case 4:
    {
      MenuActual = 0;
      MenuCero();
      digitalWrite(DO_Buzzer, HIGH);
      delay(500);
      digitalWrite(DO_Buzzer, LOW);
    }
    break;
    case 3:
    {
      MenuActual = 0;
      MenuCero();
      digitalWrite(DO_Buzzer, HIGH);
      delay(500);
      digitalWrite(DO_Buzzer, LOW);
    }
    break;
    case 2:
    {
      MenuActual = 0;
      MenuCero();
      digitalWrite(DO_Buzzer, HIGH);
      delay(500);
      digitalWrite(DO_Buzzer, LOW);
    }
    break;
    case 1:
    {
      MenuActual = 0;
      MenuCero();
      digitalWrite(DO_Buzzer, HIGH);
      delay(500);
      digitalWrite(DO_Buzzer, LOW);
    }
    break;
    }
  }
}
