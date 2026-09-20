#include "menu_actions.h"

#include "alarm.h"
#include "kume_eeprom.h"
#include "machine_control.h"
#include "vars.h"

void decreaseAcsSetpoint() {
    SetP_ACS_Edit = SetP_ACS_Edit - 1;
    normalizeAcsTemp(&SetP_ACS_Edit);
}

void increaseAcsSetpoint() {
    SetP_ACS_Edit = SetP_ACS_Edit + 1;
    normalizeAcsTemp(&SetP_ACS_Edit);
}

void stopAlarmBuzzer() {
    Valor_DO_Buzzer = LOW;
}

void toggleMode() {
    changeModo(!modoFrio);
}

void saveAcsSetpoint() {
    SetP_ACS = SetP_ACS_Edit;
    EEPROMwrite(SetP_ACS_Address, SetP_ACS);
}

void toggleFlowAlarm() {
    EnableFlowAlarm = !EnableFlowAlarm;
    EEPROMwrite(EnableFlowAlarm_Address, EnableFlowAlarm);
}

void toggleHeating() {
    heating_off = !heating_off;
    EEPROMwrite(heating_off_address, heating_off);
}

void toggleAcs() {
    EnableACS = !EnableACS;
    EEPROMwrite(EnableACS_Address, EnableACS);
}

void toggleAcsDelta() {
    EnableACS_DeltaElectrico = !EnableACS_DeltaElectrico;
    EEPROMwrite(EnableACS_DeltaElectrico_Address, EnableACS_DeltaElectrico);
}

void toggleAcsElectric() {
    EnableElectricACS = !EnableElectricACS;
    EEPROMwrite(EnableElectricACS_Address, EnableElectricACS);
}

void resetActiveAlarms() {
    resetAlarms();
}

void clearAlarmHistory() {
    EEPROMwrite(Alarma_Address, (uint8_t)0);
}

MenuAction menuActionFor(MenuId id, MenuButton button) {
    if (button == BUTTON_UP && id == MENU_ACS_EDIT) return increaseAcsSetpoint;
    if (button == BUTTON_DOWN && id == MENU_ACS_EDIT) return decreaseAcsSetpoint;
    if (button == BUTTON_ENTER && id == MENU_MODE) return toggleMode;
    if (button == BUTTON_ENTER && id == MENU_ACS_EDIT) return saveAcsSetpoint;
    if (button == BUTTON_ENTER && id == MENU_FLOW_ALARMS) return toggleFlowAlarm;
    if (button == BUTTON_ENTER && id == MENU_HEATING) return toggleHeating;
    if (button == BUTTON_ENTER && id == MENU_ACS_ENABLE) return toggleAcs;
    if (button == BUTTON_ENTER && id == MENU_ACS_DELTA) return toggleAcsDelta;
    if (button == BUTTON_ENTER && id == MENU_ACS_ELECTRIC) return toggleAcsElectric;
    if (button == BUTTON_UP && id == MENU_ALARM_ACTIVE) return stopAlarmBuzzer;
    if (button == BUTTON_DOWN && id == MENU_ALARM_ACTIVE) return stopAlarmBuzzer;
    if (button == BUTTON_ENTER && id == MENU_ALARM_ACTIVE) return resetActiveAlarms;
    if (button == BUTTON_ENTER && id == MENU_ALARM_LOG_VIEW) return clearAlarmHistory;
    return nullptr;
}
