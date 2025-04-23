#include "keyboard.h"

// BOTONES DE TECLADO (no se pueden modificar)
const int DI_Teclado_Arriba = 7;
const int DI_Teclado_Abajo = 5;
const int DI_Teclado_Enter = 6;
const int DI_Teclado_Atras = 4;

// Pin de interrupcion para funcionamiento del teclado
const int InterruptPin = 3;  

void keyboardSetup() {
    pinMode(DI_Teclado_Arriba, INPUT);
    pinMode(DI_Teclado_Abajo, INPUT);
    pinMode(DI_Teclado_Enter, INPUT);
    pinMode(DI_Teclado_Atras, INPUT);
    pinMode(InterruptPin, INPUT);
    // Asignación de Interrupciones (se define el número de la interrupción, no del pin; la rutina de interrupción y el modo de activación)
    attachInterrupt(1, AtencionTeclado, FALLING);
}

// Función de Navegación entre Menús y Modificación de Parámetros
// void AtencionTecladoNew() {
//     bool BotonArriba = digitalRead(DI_Teclado_Arriba) == LOW ? true : false;
//     bool BotonAbajo = digitalRead(DI_Teclado_Abajo) == LOW ? true : false;
//     bool BotonEnter = digitalRead(DI_Teclado_Enter) == LOW ? true : false;
//     bool BotonAtras = digitalRead(DI_Teclado_Atras) == LOW ? true : false;

//     if (BotonEnter || BotonAbajo || BotonArriba || BotonAtras) {
//         buzzerStart();
//     }
//     if (BotonEnter) {
//         menuActivo = &menuActivo->enter();
//     }
//     if (BotonAbajo) {
//         menuActivo = &(menuActivo->down());
//     }
//     if (BotonArriba) {
//         menuActivo = &(menuActivo->up());
//     }
//     if (BotonAtras) {
//         menuActivo = &(menuActivo->escape());
//     }
//     menuActivo->refresh();
// }

// Función de Navegación entre Menús y Modificación de Parámetros
void AtencionTecladoOld() {
    bool BotonArriba = digitalRead(DI_Teclado_Arriba) == LOW ? true : false;
    bool BotonAbajo = digitalRead(DI_Teclado_Abajo) == LOW ? true : false;
    bool BotonEnter = digitalRead(DI_Teclado_Enter) == LOW ? true : false;
    bool BotonAtras = digitalRead(DI_Teclado_Atras) == LOW ? true : false;

    if (BotonEnter || BotonAbajo || BotonArriba || BotonAtras) {
        buzzerStart();
    }

    if (BotonEnter) {
        switch (MenuActual) {
            case 0: {
                MenuActual = 1;
                MenuUno();
            } break;
            case 1: {
                MenuActual = 10;
                MenuUnoCero();
            } break;
            case 2: {
                MenuActual = 20;
                MenuDosCero();
            } break;
            case 4: {
                MenuActual = 40;
                MenuCuatroCero();
            } break;
            case 5: {
                MenuActual = 50;
                MenuCincoCero();
            } break;
            case 20: {
                changeModo(!modoFrio);
            } break;
            case 21: {
                MenuActual = 210;
                MenuDosUnoCero();
            } break;
            case 210: {
                SetP_ACS = SetP_ACS_Edit;
                EEPROMwrite(SetP_ACS_Address, SetP_ACS);
            } break;
            case 23: {
                EnableFlowAlarm = !EnableFlowAlarm;
                EEPROMwrite(EnableACS_Address, EnableFlowAlarm);
            } break;
            case 24: {
                heating_off = !heating_off;
                EEPROMwrite(heating_off_address, heating_off);
            } break;
            case 28: {
                EnableACS = !EnableACS;
                EEPROMwrite(EnableACS_Address, EnableACS);
            } break;
            case 281: {
                EnableACS_DeltaElectrico = !EnableACS_DeltaElectrico;
                EEPROMwrite(EnableACS_DeltaElectrico_Address, EnableACS_DeltaElectrico);
            } break;
            case 29: {
                EnableElectricACS = !EnableElectricACS;
                EEPROMwrite(EnableElectricACS_Address, EnableElectricACS);
            } break;
            case 40: {
                resetAlarms();
            } break;
            case 50: {
                EEPROMwrite(Alarma_Address, (uint8_t)0);
            } break;
        }
    }

    if (BotonAbajo) {
        switch (MenuActual) {
            case 1: {
                MenuActual = 2;
                MenuDos();
            } break;
            case 10: {
                MenuActual = 11;
                MenuUnoUno();
            } break;
            case 2: {
                MenuActual = 4;
                MenuCuatro();
            } break;
            case 4: {
                MenuActual = 5;
                MenuCinco();
            } break;
            case 20: {
                MenuActual = 21;
                MenuDosUno();
            } break;
            case 21: {
                MenuActual = 23;
                MenuDosTres();
            } break;
            case 210: {
                SetP_ACS_Edit = SetP_ACS_Edit - 1;
                normalizeAcsTemp(&SetP_ACS_Edit);
            } break;
            case 23: {
                MenuActual = 24;
                MenuDosCuatro();
            } break;
            case 24: {
                MenuActual = 28;
                MenuDosOcho();
            } break;
            case 28: {
                MenuActual = 281;
                MenuDosOchoUno();
            } break;
            case 281: {
                MenuActual = 29;
                MenuDosNueve();
            } break;
            case 40: {
                buzzerStop(true);
            } break;
        }
    }

    if (BotonArriba) {
        switch (MenuActual) {
            case 5: {
                MenuActual = 4;
                MenuCuatro();
            } break;
            case 4: {
                MenuActual = 2;
                MenuDos();
            } break;
            case 2: {
                MenuActual = 1;
                MenuUno();
            } break;
            case 11: {
                MenuActual = 10;
                MenuUnoCero();
            } break;
            case 12: {
                MenuActual = 11;
                MenuUnoUno();
            } break;
            case 29: {
                MenuActual = 281;
                MenuDosOchoUno();
            } break;
            case 281: {
                MenuActual = 28;
                MenuDosOcho();
            } break;
            case 28: {
                MenuActual = 24;
                MenuDosCuatro();
            } break;
            case 24: {
                MenuActual = 23;
                MenuDosTres();
            } break;
            case 23: {
                MenuActual = 21;
                MenuDosUno();
            } break;
            case 21: {
                MenuActual = 20;
                MenuDosCero();
            } break;
            case 210: {
                SetP_ACS_Edit = SetP_ACS_Edit + 1;
                normalizeAcsTemp(&SetP_ACS_Edit);
            } break;
            case 40: {
                buzzerStop(true);
            } break;
        }
    }

    if (BotonAtras) {
        switch (MenuActual) {
            case 0: {
            } break;
            case 10: {
                MenuActual = 1;
                MenuUno();
            } break;
            case 11: {
                MenuActual = 1;
                MenuUno();
            } break;
            case 12: {
                MenuActual = 1;
                MenuUno();
            } break;
            case 13: {
                MenuActual = 1;
                MenuUno();
            } break;
            case 20: {
                MenuActual = 2;
                MenuDos();
            } break;
            case 21: {
                MenuActual = 2;
                MenuDos();
            } break;
            case 23: {
                MenuActual = 2;
                MenuDos();
            } break;
            case 24: {
                MenuActual = 2;
                MenuDos();
            } break;
            case 25: {
                MenuActual = 2;
                MenuDos();
            } break;
            case 28: {
                MenuActual = 2;
                MenuDos();
            } break;
            case 281: {
                MenuActual = 2;
                MenuDos();
            } break;
            case 29: {
                MenuActual = 2;
                MenuDos();
            } break;
            case 40: {
                MenuActual = 4;
                MenuCuatro();
            } break;
            case 50: {
                MenuActual = 5;
                MenuCinco();
            } break;
            case 210: {
                MenuActual = 21;
                MenuDosUno();
            } break;
            case 5: {
                MenuActual = 0;
                MenuCero();
            } break;
            case 4: {
                MenuActual = 0;
                MenuCero();
            } break;
            case 2: {
                MenuActual = 0;
                MenuCero();
            } break;
            case 1: {
                MenuActual = 0;
                MenuCero();
            } break;
        }
    }
}

void AtencionTeclado() {
    AtencionTecladoOld();
}
