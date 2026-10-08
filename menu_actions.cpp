#include "menu_actions.h"

#include "alarm.h"
#include "kume_eeprom.h"
#include "machine_control.h"
#include "vars.h"

void decreaseAcsSetpoint() {
    acsSetpointEdit = acsSetpointEdit - 1;
    normalizeAcsTemp(&acsSetpointEdit);
}

void increaseAcsSetpoint() {
    acsSetpointEdit = acsSetpointEdit + 1;
    normalizeAcsTemp(&acsSetpointEdit);
}

void stopAlarmBuzzer() {
    valorDoBuzzer = LOW;
}

void toggleMode() {
    changeModo(!modoFrio);
}

void saveAcsSetpoint() {
    acsSetpoint = acsSetpointEdit;
    eepromWrite(acsSetpointAddress, acsSetpoint);
}

void toggleFlowAlarm() {
    enableFlowAlarm = !enableFlowAlarm;
    eepromWrite(enableFlowAlarmAddress, enableFlowAlarm);
}

void toggleHeating() {
    heatingOff = !heatingOff;
    eepromWrite(heatingOffAddress, heatingOff);
}

void toggleAcs() {
    enableAcs = !enableAcs;
    eepromWrite(enableAcsAddress, enableAcs);
}

void toggleAcsDelta() {
    enableAcsDeltaElectrico = !enableAcsDeltaElectrico;
    eepromWrite(enableAcsDeltaElectricoAddress, enableAcsDeltaElectrico);
}

void toggleAcsElectric() {
    enableElectricAcs = !enableElectricAcs;
    eepromWrite(enableElectricAcsAddress, enableElectricAcs);
}

void resetActiveAlarms() {
    resetAlarms();
}

void clearAlarmHistory() {
    eepromWrite(alarmaAddress, (uint8_t)0);
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
