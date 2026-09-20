#ifndef MENU_NAVIGATION_H
#define MENU_NAVIGATION_H

#include <Arduino.h>

enum MenuId : uint8_t {
    MENU_HOME,
    MENU_MONITOR,
    MENU_MONITOR_VALUES_1,
    MENU_MONITOR_VALUES_2,
    MENU_CONFIGURATION,
    MENU_MODE,
    MENU_ACS_CONFIG,
    MENU_ACS_EDIT,
    MENU_FLOW_ALARMS,
    MENU_HEATING,
    MENU_ACS_ENABLE,
    MENU_ACS_DELTA,
    MENU_ACS_ELECTRIC,
    MENU_ALARM_MONITOR,
    MENU_ALARM_ACTIVE,
    MENU_ALARM_LOG,
    MENU_ALARM_LOG_VIEW,
    MENU_NONE
};

enum MenuButton : uint8_t {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_ENTER,
    BUTTON_BACK
};

void processMenuButton(MenuButton button);
void refreshCurrentMenu();

#endif
