#include "kume_eeprom.h"

namespace {
bool readBoolOrDefault(int address, bool defaultValue) {
    uint8_t value = EEPROM.read(address);
    if (value == 0) {
        return false;
    }
    if (value == 1) {
        return true;
    }
    return defaultValue;
}
}

void eepromWrite(int address, bool flag) {
    EEPROM.update(address, flag);
}

void eepromWrite(int address, uint8_t number) {
    EEPROM.update(address, number);
}

uint8_t eepromReadUint8(int address) {
    return EEPROM.read(address);
}
void EEPROMLectura()  // Función de lectura de valores almacenados en memoria EEPROM
{
    acsSetpoint = EEPROM.read(acsSetpointAddress);
    acsSetpointEdit = acsSetpoint;

    enableFlowAlarm = readBoolOrDefault(enableFlowAlarmAddress, false);
    enableAcs = readBoolOrDefault(enableAcsAddress, true);
    enableAcsDeltaElectrico = readBoolOrDefault(enableAcsDeltaElectricoAddress, false);
    enableElectricAcs = readBoolOrDefault(enableElectricAcsAddress, false);
    modoFrio = readBoolOrDefault(modoFrioAddress, false);
    heatingOff = readBoolOrDefault(heatingOffAddress, false);
}
