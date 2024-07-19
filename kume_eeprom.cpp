#include "kume_eeprom.h"

void EEPROMwrite(int address, bool flag) {
    EEPROM.write(address, flag);
}

void EEPROMwrite(int address, uint8_t number) {
    EEPROM.write(address, number);
}

uint8_t EEPROMreaduint8_t(int address) {
    return EEPROM.read(address);
}
bool EEPROMbool(int address) {
    return EEPROM.read(address);
}

void EEPROMLectura()  // Función de lectura de valores almacenados en memoria EEPROM
{
    uint8_t lowByte = EEPROM.read(SetP_ACS_Address + 1);  // Se leen los 2 bytes que componen la variable int por separado
    uint8_t highByte = EEPROM.read(SetP_ACS_Address);
    SetP_ACS = ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
    SetP_ACS = EEPROM.read(SetP_ACS_Address);
    SetP_ACS_Edit = SetP_ACS;

    EnableFlowAlarm = EEPROM.read(EnableFlowAlarm_Address);
    EnableACS = EEPROM.read(EnableACS_Address);
    EnableACS_DeltaElectrico = EEPROM.read(EnableACS_DeltaElectrico_Address);
    EnableElectricACS = EEPROM.read(EnableElectricACS_Address);
    modoFrio = EEPROM.read(modoFrio_address);
}
