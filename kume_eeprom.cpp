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

void EEPROMwrite(int address, bool flag) {
    EEPROM.update(address, flag);
}

void EEPROMwrite(int address, uint8_t number) {
    EEPROM.update(address, number);
}

uint8_t EEPROMreaduint8_t(int address) {
    return EEPROM.read(address);
}
void EEPROMLectura()  // Función de lectura de valores almacenados en memoria EEPROM
{
    SetP_ACS = EEPROM.read(SetP_ACS_Address);
    SetP_ACS_Edit = SetP_ACS;

    EnableFlowAlarm = readBoolOrDefault(EnableFlowAlarm_Address, false);
    EnableACS = readBoolOrDefault(EnableACS_Address, true);
    EnableACS_DeltaElectrico = readBoolOrDefault(EnableACS_DeltaElectrico_Address, false);
    EnableElectricACS = readBoolOrDefault(EnableElectricACS_Address, false);
    modoFrio = readBoolOrDefault(modoFrio_address, false);
    heating_off = readBoolOrDefault(heating_off_address, false);
}
