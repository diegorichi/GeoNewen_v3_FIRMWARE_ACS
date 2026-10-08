#include "alarm.h"

#define PressOK ((!flagPresLow) && (!flagPresHi))
/*
PH  PL POK
V   V  F
V   F  F
F   V  F
F   F  V
*/
 
// Función de identificación de Alarma Activa
void ConvertFlagToAlarm() {
    nroAlarma = 0;
    if (flagTempCompressor) {
        nroAlarma = 6;
    } else if (flagCaudT) {
        nroAlarma = 7;
    } else if (flagCaudH) {
        nroAlarma = 8;
    } else if (flagPresHi) {
        nroAlarma = 9;
    } else if (flagPresLow) {
        nroAlarma = 10;
    } else if (flagTempAdm) {
        nroAlarma = 15;
    } else if (flagTempDescarga) {
        nroAlarma = 18;
    }

    if (nroAlarma != 0) {
        eepromWrite(alarmaAddress, nroAlarma);
    }
}

// Luego de ocurrida una alarma y revisada por parte del usuario, esta funcion resetea los flags y contadores a cero
void ResetFlags() {
    flagTempCompressor = false;
    flagCaudT = false;
    flagCaudH = false;
    flagPresHi = false;
    flagPresLow = false;
    flagTempAdm = false;
    flagTempDescarga = false;

    contTempCompressor = 0;
    contPressHi = 0;
    contPressLow = 0;
    contTempDes = 0;
}

void checkFlagsForAlarms() {
    if (flagTempCompressor || !PressOK || flagCaudT || flagCaudH || flagTempAdm || flagTempDescarga) {
        estadoMaquina = 4;
    }
}

void resetAlarms() {
    if (estadoMaquina == 4) {
        nroAlarma = 0;
        buzzerStop();
        alarmaActiva = false;
        ResetFlags();
        estadoMaquina = 0;
    }
}
