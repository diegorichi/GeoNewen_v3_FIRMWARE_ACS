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
    SetP_ACS = EEPROM.read(SetP_ACS_Address);
    SetP_ACS_Edit = SetP_ACS;

    EnableFlowAlarm = EEPROM.read(EnableFlowAlarm_Address);
    EnableACS = EEPROM.read(EnableACS_Address);
    EnableACS_DeltaElectrico = EEPROM.read(EnableACS_DeltaElectrico_Address);
    EnableElectricACS = EEPROM.read(EnableElectricACS_Address);
    modoFrio = EEPROM.read(modoFrio_address);
    heating_off = EEPROMbool(heating_off_address);
}
