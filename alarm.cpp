#include "alarm.h"


int Alarmas() // Función de identificación de Alarma Activa
{
  Flag_Alarma_General = false;
  Nro_Alarma = 0;
  if (Flag_TempIntXT_Baja) {
    Nro_Alarma = 1;
  }
  else if (Flag_TempIntXT_Alta) {
    Nro_Alarma = 2;
  }
  else if (Flag_TempIntXH_Baja) {
    Nro_Alarma = 3;
  }
  else if (Flag_TempIntXH_Alta) {
    Nro_Alarma = 4;
  }
  else if (Flag_TempTriacs) {
    Nro_Alarma = 5;
  }
  else if (Flag_TempComp01) {
    Nro_Alarma = 6;
  }
  else if (Flag_CaudT) {
    Nro_Alarma = 7;
  }
  else if (Flag_CaudH) {
    Nro_Alarma = 8;
  }
  else if (Flag_PresHI) {
    Nro_Alarma = 9;
  }
  else if (Flag_PresLOW) {
    Nro_Alarma = 10;
  }
  else if (Flag_Corriente) {
    Nro_Alarma = 12;
  }
  else if (Flag_Temp_Caldera) {
    Nro_Alarma = 13;
  }
  else if (Flag_Alarma_Trif) {
    // Nro_Alarma = 14;
  }
  else if (Flag_Temp_Adm) {
    Nro_Alarma = 15;
  }
  else if (Flag_Aporte_E) {
    Nro_Alarma = 16;
  }
  else if (Flag_RetornoLiq) {
    Nro_Alarma = 17;
  }
  else if (Flag_Temp_Descarga) {
    Nro_Alarma = 18;
  }

  if (Nro_Alarma != 0) {
    Flag_Alarma_General = true;
    EEPROM.write(Alarma_Address, Nro_Alarma);
  }
  return Nro_Alarma;
}

void ResetFlags() // Luego de ocurrida una alarma y revisada por parte del usuario, esta funcion resetea los flagas y contadores a cero
{
  Flag_TempIntXT_Baja = false;
  Flag_TempIntXT_Alta = false;
  Flag_TempIntXH_Baja = false;
  Flag_TempIntXH_Alta = false;
  Flag_TempTriacs = false;
  Flag_TempComp01 = false;
  Flag_CaudT = false;
  Flag_CaudH = false;
  Flag_PresHI = false;
  Flag_PresLOW = false;
  Flag_Corriente = false;
  Flag_Temp_Caldera = false;
  Flag_Alarma_Trif = false;
  Flag_Temp_Adm = false;
  Flag_Aporte_E = false;
  Flag_RetornoLiq = false;
  Flag_Temp_Descarga = false;

  Cont_Temp_intX_T = 0;
  Cont_Temp_intX_T2 = 0;
  Cont_Temp_intX_H = 0;
  Cont_Temp_intX_H2 = 0;
  Cont_Temp_Comp_01 = 0;
  Cont_Caud_T = 0;
  Cont_Caud_H = 0;
  Cont_Press_HI = 0;
  Cont_Press_LOW = 0;
  Cont_Temp_Des = 0;
}


void checkFlagsForAlarms() {
  if (Flag_TempComp01 || !PressOK || Flag_CaudT || Flag_CaudH || Flag_Temp_Adm || Flag_Temp_Descarga)
  {
    Estado_Maquina = 4;
    MenuCuatroCero();
    MenuActual = 40;
  }
}
