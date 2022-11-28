#include "keyboard.h"

const int DI_Teclado_Arriba = 7; // BOTONES DE TECLADO (no se pueden modificar)
const int DI_Teclado_Abajo = 5;
const int DI_Teclado_Enter = 6;
const int DI_Teclado_Atras = 4;

const int InterruptPin = 3; // Pin de interrupcion para funcionamiento del teclado

void keyboardSetup() {
  pinMode(DI_Teclado_Arriba, INPUT); // Definición de entradas y salidas
  pinMode(DI_Teclado_Abajo, INPUT);
  pinMode(DI_Teclado_Enter, INPUT);
  pinMode(DI_Teclado_Atras, INPUT);
  pinMode(InterruptPin, INPUT);
}

bool buzzerStop(void*) {
  digitalWrite(DO_Buzzer, LOW);
  return false;
}

void buzzerBeep() {
  digitalWrite(DO_Buzzer, HIGH);
  timer_things.in(300, buzzerStop);
}

void AtencionTecladoOld() // Función de Navegación entre Menús y Modificación de Parámetros
{
  bool BotonArriba = digitalRead(DI_Teclado_Arriba) == LOW ? true : false;
  bool BotonAbajo = digitalRead(DI_Teclado_Abajo) == LOW ? true : false;
  bool BotonEnter = digitalRead(DI_Teclado_Enter) == LOW ? true : false;
  bool BotonAtras = digitalRead(DI_Teclado_Atras) == LOW ? true : false;

  for (int j = 0; j < 100; j++) // Debuoncing
  {
  }

  if (BotonEnter || BotonAbajo || BotonArriba || BotonAtras) {
    buzzerBeep();
  }

  if (BotonEnter) {
    switch (MenuActual) {
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
          changeModo();
        }
        break;
      case 21:
        {
          MenuActual = 210;
          MenuDosUnoCero();
        }
        break;
      case 210:
        {
          SetP_ACS = SetP_ACS_Edit;
          EEPROMwrite(SetP_ACS_Address, SetP_ACS);
        }
        break;
      case 23:
        {
          Flag_Alarma_EN = !Flag_Alarma_EN;
          EEPROMwrite(ACS_EN_Address, Flag_Alarma_EN);
        }
        break;
      case 24:
        {
          heating_off = !heating_off;
          EEPROMwrite(heating_off_address, heating_off);
        }
        break;
      case 28:
        {
          Flag_ACS_EN = !Flag_ACS_EN;
          EEPROMwrite(ACS_EN_Address, Flag_ACS_EN);
        }
        break;
      case 281:
        {
          Flag_ACS_DT_EN = !Flag_ACS_DT_EN;
          EEPROMwrite(ACS_DT_EN_Address, Flag_ACS_DT_EN);
        }
        break;
      case 29:
        {
          Flag_ACS_EN_ELECT = !Flag_ACS_EN_ELECT;
          EEPROMwrite(ACS_EN_ELECT_Address, Flag_ACS_EN_ELECT);
        }
        break;
      case 40:
        {
          resetAlarms();
        }
        break;
      case 50:
        {
          EEPROMwrite(Alarma_Address, (uint8_t)0);
        }
        break;
    }
  }

  if (BotonAbajo) {
    switch (MenuActual) {
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
      case 2:
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
          normalizeAcsTemp(&SetP_ACS_Edit);
        }
        break;
      case 23:
        {
          MenuActual = 24;
          MenuDosCuatro();
        }
        break;
      case 24:
        {
          MenuActual = 28;
          MenuDosOcho();
        }
        break;
      case 28:
        {
          MenuActual = 281;
          MenuDosOchoUno();
        }
        break;
      case 281:
        {
          MenuActual = 29;
          MenuDosNueve();
        }
        break;
      case 40:
        {
          digitalWrite(DO_Buzzer, LOW);
        }
        break;
    }
  }

  if (BotonArriba) {
    switch (MenuActual) {
      case 5:
        {
          MenuActual = 4;
          MenuCuatro();
        }
        break;
      case 4:
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
      case 29:
        {
          MenuActual = 281;
          MenuDosOchoUno();
        }
        break;
      case 281:
        {
          MenuActual = 28;
          MenuDosOcho();
        }
        break;
      case 28:
        {
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
      case 21:
        {
          MenuActual = 20;
          MenuDosCero();
        }
        break;
      case 210:
        {
          SetP_ACS_Edit = SetP_ACS_Edit + 1;
          normalizeAcsTemp(&SetP_ACS_Edit);
        }
        break;
      case 40:
        {
          digitalWrite(DO_Buzzer, LOW);
        }
        break;
    }
  }

  if (BotonAtras) {
    switch (MenuActual) {
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
      case 21:
        {
          MenuActual = 2;
          MenuDos();
        }
        break;
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
      case 28:
        {
          MenuActual = 2;
          MenuDos();
        }
        break;
      case 281:
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
        }
        break;
      case 4:
        {
          MenuActual = 0;
          MenuCero();
        }
        break;
      case 2:
        {
          MenuActual = 0;
          MenuCero();
        }
        break;
      case 1:
        {
          MenuActual = 0;
          MenuCero();
        }
        break;
    }
  }
}

void AtencionTeclado() {
  AtencionTecladoOld();
}
