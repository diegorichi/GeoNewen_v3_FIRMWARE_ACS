#include "alarm.h"

int Alarmas() // Función de identificación de Alarma Activa
{
  Nro_Alarma = 0;
  if (Flag_TempComp01) {
    Nro_Alarma = 6;
  } else if (Flag_CaudT) {
    Nro_Alarma = 7;
  } else if (Flag_CaudH) {
    Nro_Alarma = 8;
  } else if (Flag_PresHI) {
    Nro_Alarma = 9;
  } else if (Flag_PresLOW) {
    Nro_Alarma = 10;
  } else if (Flag_Temp_Adm) {
    Nro_Alarma = 15;
  } else if (Flag_Temp_Descarga) {
    Nro_Alarma = 18;
  }

  if (Nro_Alarma != 0) {
    EEPROMwrite(Alarma_Address, Nro_Alarma);
  }
  return Nro_Alarma;
}

void ResetFlags() // Luego de ocurrida una alarma y revisada por parte del usuario, esta funcion resetea los flagas y contadores a cero
{

  Flag_TempComp01 = false;
  Flag_CaudT = false;
  Flag_CaudH = false;
  Flag_PresHI = false;
  Flag_PresLOW = false;
  Flag_Temp_Adm = false;
  Flag_Temp_Descarga = false;

  Cont_Temp_Comp_01 = 0;
  Cont_Press_HI = 0;
  Cont_Press_LOW = 0;
  Cont_Temp_Des = 0;
}

void checkFlagsForAlarms() {
  if (Flag_TempComp01 || !PressOK || Flag_CaudT || Flag_CaudH || Flag_Temp_Adm || Flag_Temp_Descarga) {
    Estado_Maquina = 4;
    MenuCuatroCero();
    MenuActual = 40;
  }
}

void resetAlarms() {
  Nro_Alarma = 0;
  digitalWrite(DO_Buzzer, LOW);
  Alarma_Activa = false;
  ResetFlags();
  Flag_Buzzer = false;
  Estado_Maquina = 0;
}