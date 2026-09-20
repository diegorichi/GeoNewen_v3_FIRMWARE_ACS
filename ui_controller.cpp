#include "ui_controller.h"

#include "menu_navigation.h"
#include "machine_control.h"
#include "vars.h"

void processUiEvents() {
    static bool alarmWasActive = false;
    static bool modeChangeWasLocked = false;
    const bool alarmIsActive = Estado_Maquina == 4;
    const bool modeChangeIsLocked = isModeChangeLocked();

    if (alarmIsActive && !alarmWasActive) {
        navigateTo(MENU_ALARM_ACTIVE);
    }

    if (!alarmIsActive && alarmWasActive && MenuActual == MENU_ALARM_ACTIVE) {
        navigateTo(MENU_ALARM_MONITOR);
    }

    if (!alarmIsActive) {
        if (modeChangeIsLocked && !modeChangeWasLocked) {
            navigateTo(MENU_MODE_CHANGING);
        } else if (!modeChangeIsLocked && modeChangeWasLocked) {
            navigateTo(MENU_MODE);
        }
    }

    alarmWasActive = alarmIsActive;
    modeChangeWasLocked = modeChangeIsLocked;
}
