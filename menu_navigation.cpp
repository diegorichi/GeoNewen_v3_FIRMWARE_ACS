#include "menu_navigation.h"

#include "alarm.h"
#include "functionsLCDMenu.h"
#include "machine_control.h"

using MenuAction = void (*)();

struct MenuEntry {
    MenuId id;
    MenuId up;
    MenuId down;
    MenuId enter;
    MenuId back;
    MenuAction draw;
    MenuAction refresh;
};

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

const MenuEntry menuEntries[] = {
    // id, up, down, enter, back, draw, refresh
    {MENU_HOME, MENU_NONE, MENU_NONE, MENU_MONITOR, MENU_NONE, drawHomeScreen, refreshHomeScreen},
    {MENU_MONITOR, MENU_ALARM_LOG, MENU_CONFIGURATION, MENU_MONITOR_VALUES_1, MENU_HOME, drawMonitorMenu, nullptr},
    {MENU_MONITOR_VALUES_1, MENU_MONITOR_VALUES_2, MENU_MONITOR_VALUES_2, MENU_NONE, MENU_MONITOR, drawMonitorScreen1, refreshMonitorScreen1},
    {MENU_MONITOR_VALUES_2, MENU_MONITOR_VALUES_1, MENU_MONITOR_VALUES_1, MENU_NONE, MENU_MONITOR, drawMonitorScreen2, refreshMonitorScreen2},
    {MENU_CONFIGURATION, MENU_MONITOR, MENU_ALARM_MONITOR, MENU_MODE, MENU_HOME, drawConfigurationMenu, nullptr},
    {MENU_MODE, MENU_ACS_ELECTRIC, MENU_ACS_CONFIG, MENU_NONE, MENU_CONFIGURATION, drawModeScreen, refreshModeScreen},
    {MENU_ACS_CONFIG, MENU_MODE, MENU_FLOW_ALARMS, MENU_ACS_EDIT, MENU_CONFIGURATION, drawAcsConfigurationScreen, nullptr},
    {MENU_ACS_EDIT, MENU_NONE, MENU_NONE, MENU_NONE, MENU_ACS_CONFIG, drawAcsEditScreen, refreshAcsEditScreen},
    {MENU_FLOW_ALARMS, MENU_ACS_CONFIG, MENU_HEATING, MENU_NONE, MENU_CONFIGURATION, drawFlowAlarmScreen, refreshFlowAlarmScreen},
    {MENU_HEATING, MENU_FLOW_ALARMS, MENU_ACS_ENABLE, MENU_NONE, MENU_CONFIGURATION, drawHeatingScreen, refreshHeatingScreen},
    {MENU_ACS_ENABLE, MENU_HEATING, MENU_ACS_DELTA, MENU_NONE, MENU_CONFIGURATION, drawAcsEnableScreen, refreshAcsEnableScreen},
    {MENU_ACS_DELTA, MENU_ACS_ENABLE, MENU_ACS_ELECTRIC, MENU_NONE, MENU_CONFIGURATION, drawAcsDeltaScreen, refreshAcsDeltaScreen},
    {MENU_ACS_ELECTRIC, MENU_ACS_DELTA, MENU_MODE, MENU_NONE, MENU_CONFIGURATION, drawAcsElectricScreen, refreshAcsElectricScreen},
    {MENU_ALARM_MONITOR, MENU_CONFIGURATION, MENU_ALARM_LOG, MENU_ALARM_ACTIVE, MENU_HOME, drawAlarmMenu, nullptr},
    {MENU_ALARM_ACTIVE, MENU_NONE, MENU_NONE, MENU_NONE, MENU_ALARM_MONITOR, drawActiveAlarmScreen, refreshActiveAlarmScreen},
    {MENU_ALARM_LOG, MENU_ALARM_MONITOR, MENU_MONITOR, MENU_ALARM_LOG_VIEW, MENU_HOME, drawAlarmHistoryMenu, nullptr},
    {MENU_ALARM_LOG_VIEW, MENU_NONE, MENU_NONE, MENU_NONE, MENU_ALARM_LOG, drawAlarmHistoryScreen, refreshAlarmHistoryScreen},
};

const MenuEntry* findMenuEntry(MenuId id) {
    for (const MenuEntry& entry : menuEntries) {
        if (entry.id == id) {
            return &entry;
        }
    }
    return nullptr;
}

void navigateTo(MenuId id) {
    const MenuEntry* entry = findMenuEntry(id);
    if (entry == nullptr) {
        return;
    }

    MenuActual = id;
    entry->draw();
}

void refreshCurrentMenu() {
    const MenuEntry* entry = findMenuEntry(MenuActual);
    if (entry != nullptr && entry->refresh != nullptr) {
        entry->refresh();
    }
}

MenuAction findMenuAction(MenuId id, MenuButton button) {
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

void processMenuButton(MenuButton button) {
    const MenuEntry* entry = findMenuEntry(MenuActual);
    if (entry == nullptr) {
        return;
    }

    MenuId target = MENU_NONE;
    if (button == BUTTON_UP) target = entry->up;
    if (button == BUTTON_DOWN) target = entry->down;
    if (button == BUTTON_ENTER) target = entry->enter;
    if (button == BUTTON_BACK) target = entry->back;

    if (target != MENU_NONE) {
        navigateTo(target);
        return;
    }

    MenuAction action = findMenuAction(entry->id, button);
    if (action != nullptr) {
        action();
    }
}
