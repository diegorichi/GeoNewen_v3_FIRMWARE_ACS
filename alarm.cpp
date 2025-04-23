#include "alarm.h"

#define PressOK ((!Flag_PresLOW) && (!Flag_PresHI))
/*
PH  PL POK
V   V  F
V   F  F
F   V  F
F   F  V
*/
 
// Función de identificación de Alarma Activa
void ConvertFlagToAlarm() {
    Nro_Alarma = 0;
    if (Flag_TempCompressor) {
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
}

// Luego de ocurrida una alarma y revisada por parte del usuario, esta funcion resetea los flags y contadores a cero
void ResetFlags() {
    Flag_TempCompressor = false;
    Flag_CaudT = false;
    Flag_CaudH = false;
    Flag_PresHI = false;
    Flag_PresLOW = false;
    Flag_Temp_Adm = false;
    Flag_Temp_Descarga = false;

    Cont_Temp_Compressor = 0;
    Cont_Press_HI = 0;
    Cont_Press_LOW = 0;
    Cont_Temp_Des = 0;
}

void checkFlagsForAlarms() {
    if (Flag_TempCompressor || !PressOK || Flag_CaudT || Flag_CaudH || Flag_Temp_Adm || Flag_Temp_Descarga) {
        Estado_Maquina = 4;
        MenuCuatroCero();
        MenuActual = 40;
    }
}

void resetAlarms() {
    if (Estado_Maquina == 4) {
        Nro_Alarma = 0;
        Valor_DO_Buzzer = LOW;
        Flag_Buzzer = false;
        Alarma_Activa = false;
        ResetFlags();
        Estado_Maquina = 0;
    }
}