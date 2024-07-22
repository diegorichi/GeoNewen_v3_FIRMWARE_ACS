#include "functionsLCDMenu.h"
LiquidCrystal lcd(30, 32, 34, 36, 38, 40, 42, 44, 46, 48);  // Se definen los pines RS, EN y D0 a D7 del display (en ese orden)

// DEFINICIÓN DE CARACTERES (símbolos especiales a mostrar)
uint8_t FlechaAbajo[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B01110,
    B00100};

uint8_t FlechaArriba[8] = {
    B00100,
    B01110,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000};

void lcdCreateSpecialChars() {
    lcd.createChar(1, FlechaAbajo);
    lcd.createChar(2, FlechaArriba);
}

void lcdRefreshValues() {
    if (MenuActual == 0)  // Solo en la ventana de bienvenida se muestra el estado actual del sistema y de la señal de marcha
        RefreshMenuCero();

    if (MenuActual == 10)  // Este menú es la primer ventana del monitor de temperaturas y consumo eléctrico
        RefreshMenuUnoCero();

    if (MenuActual == 11)  // La visualización de la segunda ventana del monitor depende del modo de funcionamiento, ya que en función de este se cambian de lugar las variables motradas
        RefreshMenuUnoUno();

    if (MenuActual == 20)  // Modo Frio / Calor
        RefreshMenuDosCero();

    if (MenuActual == 210)  // Editar valor ACS
        RefreshMenuDosUnoCero();

    if (MenuActual == 23)  // Alarmas
        RefreshMenuDosTres();

    if (MenuActual == 24)  // Funcionamiento Auto/Manual
        RefreshMenuDosCuatro();

    if (MenuActual == 28)  // Activacion/desactivacion de ACS
        RefreshMenuDosOcho();

    if (MenuActual == 281)  // Activacion/desactivacion de delta ACS electrico
        RefreshMenuDosOchoUno();

    if (MenuActual == 29)  // Activacion/desactivacion de ACS electrico
        RefreshMenuDosNueve();

    if (MenuActual == 40)  // En función al número de alarma devuelto por la función ConvertFlagToAlarm(), se muestra el mensaje indicando la causa de la misma
        RefreshMenuCuatroCero();

    if (MenuActual == 50)  // En función al número de alarma devuelto por la función ConvertFlagToAlarm(), se muestra el mensaje indicando la causa de la misma
        RefreshMenuCincoCero();
}

// Solo en la ventana de bienvenida se muestra el estado actual del sistema y de la señal de marcha
void RefreshMenuCero() {
    lcd.setCursor(19, 2);
    lcd.print(Estado_Maquina);

    lcd.setCursor(19, 1);
    if (senal_start)
        lcd.print(F("H"));
    else
        lcd.print(F("L"));

    lcd.setCursor(19, 0);
    if (modoFrio) {
        lcd.print(F("F"));
    } else {
        lcd.print(F("C"));
    }

    lcd.setCursor(0, 1);
    lcd.print(F("NW"));

    lcd.setCursor(0, 2);
    lcd.print(F("NE"));
}

// Este menú es la primer ventana del monitor de temperaturas y consumo eléctrico
void RefreshMenuUnoCero() {
    lcd.setCursor(5, 0);
    if (modoFrio) {
        lcd.print(F("FRIO "));
    } else {
        lcd.print(F("CALOR"));
    }
    lcd.setCursor(5, 2);
    lcd.print(Temp_CompressorAcu, 1);
    lcd.setCursor(15, 2);
    lcd.print(Temp_Admision, 1);
    lcd.setCursor(5, 3);
    lcd.print(Temp_Descargaacu, 1);
    lcd.setCursor(15, 3);
    lcd.print(Temp_ACSacu, 1);
}

// La visualización de la segunda ventana del monitor depende del modo de funcionamiento, ya que en función de este se cambian de lugar las variables motradas
void RefreshMenuUnoUno() {
    lcd.setCursor(5, 0);
    if (modoFrio) {
        lcd.print(F("FRIO "));
    } else {
        lcd.print(F("CALOR"));
    }
    lcd.setCursor(15, 3);
    lcd.print(F("    "));
    lcd.setCursor(15, 3);
    lcd.print(Caud_Tacu);
    lcd.setCursor(5, 1);
    lcd.print(Temp_in_Hacu, 1);
    lcd.setCursor(15, 1);
    lcd.print(Temp_out_Hacu, 1);
    lcd.setCursor(5, 2);
    lcd.print(Temp_out_T, 1);
    lcd.setCursor(15, 2);
    lcd.print(Temp_in_T, 1);
    lcd.setCursor(5, 3);
    lcd.print(F("    "));
    lcd.setCursor(5, 3);
    lcd.print(Caud_Hacu);
}

// Modo Frio / Calor
void RefreshMenuDosCero() {
    lcd.setCursor(12, 2);
    if (modoFrio) {
        lcd.print(F("FRIO "));
    } else {
        lcd.print(F("CALOR"));
    }
}

// Editar valor ACS
void RefreshMenuDosUnoCero() {
    lcd.setCursor(14, 1);
    lcd.print(SetP_ACS);
    lcd.setCursor(13, 2);
    lcd.print(SetP_ACS_Edit);
}

// Alarmas
void RefreshMenuDosTres() {
    lcd.setCursor(8, 2);
    if (EnableFlowAlarm) {
        lcd.print(F("ON "));
    } else
        lcd.print(F("OFF"));
}

// Funcionamiento Auto/Manual
void RefreshMenuDosCuatro() {
    lcd.setCursor(10, 2);
    if (!heating_off) {
        lcd.print(F("ENCENDIDO"));
    } else
        lcd.print(F("APAGADO  "));
}

// Activacion/desactivacion de ACS
void RefreshMenuDosOcho() {
    lcd.setCursor(15, 1);
    if (EnableACS) {
        lcd.print(F("ON "));
    } else
        lcd.print(F("OFF"));
}

// Activacion/desactivacion de delta de ACS electrico final
void RefreshMenuDosOchoUno() {
    lcd.setCursor(12, 1);
    if (EnableACS_DeltaElectrico) {
        lcd.print(F("ON "));
    } else
        lcd.print(F("OFF"));

    lcd.setCursor(0, 2);
    if (deltaACSElectricResult) {
        lcd.print(F("ENCENDIDO"));
    } else
        lcd.print(F("APAGADO  "));
}

// Activacion/desactivacion de ACS electrico
void RefreshMenuDosNueve() {
    lcd.setCursor(15, 1);
    if (EnableElectricACS) {
        lcd.print(F("ON "));
    } else
        lcd.print(F("OFF"));

    lcd.setCursor(0, 2);
    if (Valor_DO_Calentador == HIGH) {
        lcd.print(F("ENCENDIDO"));
    } else
        lcd.print(F("APAGADO  "));
}

// En función al número de alarma devuelto por la función ConvertFlagToAlarm(), se muestra el mensaje indicando la causa de la misma
void RefreshAlarma(uint8_t _nro_Alarma) {
    lcd.setCursor(0, 1);
    switch (_nro_Alarma) {
        case 0: {
            lcd.print(F("No hay alarmas act. "));
            lcd.setCursor(0, 2);
            lcd.print(F("                    "));
        } break;
        case 6: {
            lcd.print(F("Temp Compresor es   "));
            lcd.setCursor(0, 2);
            lcd.print(F("ALTA                "));
        } break;
        case 7: {
            lcd.print(F("Caudal de Tierra es "));
            lcd.setCursor(0, 2);
            lcd.print(F("BAJO                "));
        } break;
        case 8: {
            lcd.print(F("Caudal de Hogar es  "));
            lcd.setCursor(0, 2);
            lcd.print(F("BAJO                "));
        } break;
        case 9: {
            lcd.print(F("Presion Pres. HI es "));
            lcd.setCursor(0, 2);
            lcd.print(F("ALTA                "));
        } break;
        case 10: {
            lcd.print(F("Presion Pres. LOW  "));
            lcd.setCursor(0, 2);
            lcd.print(F("BAJA                "));
        } break;
        case 15: {
            lcd.print(F("Temp ADM elevada"));
        } break;
        case 18: {
            lcd.print(F("Temp DES elevada"));
        } break;
    }
}

// En función al número de alarma devuelto por la función ConvertFlagToAlarm(), se muestra el mensaje indicando la causa de la misma
void RefreshMenuCuatroCero() {
    RefreshAlarma(Nro_Alarma);
}
// En función al número de alarma devuelto por la función ConvertFlagToAlarm(), se muestra el mensaje indicando la causa de la misma
void RefreshMenuCincoCero() {
    Alarma_Eeprom = EEPROMreaduint8_t(Alarma_Address);
    RefreshAlarma(Alarma_Eeprom);
}

/**********************************/
/*FUNCIONES DE DIBUJO DE PANTALLAS*/
/**********************************/

void beginLcd() {
    lcd.begin(20, 4);
    lcd.clear();
}

void showNavigation() {
    lcd.setCursor(19, 0);
    lcd.write(2);
    lcd.setCursor(19, 3);
    lcd.write(1);
}

// Pantalla de Inicio/Bienvenida
void MenuCero() {
    beginLcd();
    lcd.print(F("     KUME NEWEN     "));
    lcd.setCursor(0, 1);
    lcd.print(F("   BOMBA DE CALOR   "));
    lcd.setCursor(0, 2);
    lcd.print(F("    GeoNewen v3.0   "));
    lcd.setCursor(0, 3);
    lcd.print(F("   Presione ENTER   "));
}

// Menú de Monitor de Temps y Consumo de Energía
void MenuUno() {
    beginLcd();
    lcd.print(F("MONITOR DE TEMPS"));
    lcd.setCursor(0, 1);
    lcd.print(F("Y CONSUMO DE ENERGIA"));
    showNavigation();
}

// 1º Ventana de Monitor
void MenuUnoCero() {
    beginLcd();
    lcd.print(F("MODO:            "));
    lcd.setCursor(0, 2);
    lcd.print(F("Com:"));
    lcd.setCursor(10, 2);
    lcd.print(F("Adm:"));
    lcd.setCursor(0, 3);
    lcd.print(F("Des:"));
    lcd.setCursor(10, 3);
    lcd.print(F("ACS:"));
    showNavigation();
}

// 2º Ventana de Monitor
void MenuUnoUno() {
    beginLcd();
    lcd.print(F("MODO:            "));
    lcd.setCursor(0, 1);
    lcd.print(F("TiH:"));
    lcd.setCursor(10, 1);
    lcd.print(F("ToH:"));
    lcd.setCursor(0, 2);
    lcd.print(F("TiT:"));
    lcd.setCursor(10, 2);
    lcd.print(F("ToT:"));
    lcd.setCursor(0, 3);
    lcd.print(F("CauT:"));
    lcd.setCursor(10, 3);
    lcd.print(F("CauH:"));
    showNavigation();
}

// Menu de Configuración del Sistema
void MenuDos() {
    beginLcd();
    lcd.print(F("CONFIGURACION DEL   "));
    lcd.setCursor(0, 1);
    lcd.print(F("     SISTEMA        "));
    lcd.setCursor(0, 3);
    showNavigation();
}

// Menú de Cambio de Modo de Funcionamiento
void MenuDosCero() {
    beginLcd();
    lcd.print(F("MODO FRIO/CALOR     "));
    lcd.setCursor(0, 2);
    lcd.print(F("Modo Activo:"));
    lcd.setCursor(0, 3);
    lcd.print(F("Enter modifica"));
    showNavigation();
}

// Configuracion de temp de ACS
void MenuDosUno() {
    beginLcd();
    lcd.print(F("CONFIGURACION DE    "));
    lcd.setCursor(0, 1);
    lcd.print(F("TEMPERATURA DE ACS  "));
    lcd.setCursor(0, 3);
    lcd.print(F("Enter modifica"));
    showNavigation();
}

// Seteo de temp de ACS
void MenuDosUnoCero() {
    beginLcd();
    lcd.print(F("MODIFIQUE VALOR     "));
    lcd.setCursor(0, 1);
    lcd.print(F("Valor Actual="));
    lcd.setCursor(0, 2);
    lcd.print(F("Nuevo Valor="));
    lcd.setCursor(0, 3);
    lcd.print(F("Enter modifica"));
    showNavigation();
}

// Habilitacion alarmas de caudal
void MenuDosTres() {
    beginLcd();
    lcd.print(F("HABILITACION DE     "));
    lcd.setCursor(0, 1);
    lcd.print(F("ALARMAS DE CAUDAL   "));
    lcd.setCursor(0, 2);
    lcd.print(F("ALARMAS:   "));
    lcd.setCursor(0, 3);
    lcd.print(F("Enter modifica"));
    showNavigation();
}

// Apagar la caldera
void MenuDosCuatro() {
    beginLcd();
    lcd.print(F("ENCENDIDO/APAGADO "));
    lcd.setCursor(0, 2);
    lcd.print(F("MODO ACT:"));
    lcd.setCursor(0, 3);
    lcd.print(F("Enter modifica"));
    showNavigation();
}

// Habilitacion de Generacion de ACS
void MenuDosOcho() {
    beginLcd();
    lcd.print(F("HABILITACION DE     "));
    lcd.setCursor(0, 1);
    lcd.print(F("GENERACION ACS:     "));
    lcd.setCursor(0, 3);
    lcd.print(F("Enter modifica"));
    showNavigation();
}

// Habilitacion de Delta electrico
void MenuDosOchoUno() {
    beginLcd();
    lcd.print(F("ACS CON DELTA ELECT "));
    lcd.setCursor(0, 1);
    lcd.print(F("Habilitado:       "));
    lcd.setCursor(0, 3);
    lcd.print(F("Enter modifica"));
    showNavigation();
}

// ACS electrico
void MenuDosNueve() {
    beginLcd();
    lcd.print(F("HABILITACION DE     "));
    lcd.setCursor(0, 1);
    lcd.print(F("ACS ELECTRICO:      "));
    lcd.setCursor(0, 3);
    lcd.print(F("Enter modifica"));
    showNavigation();
}

// Menú de Visualización de Alarmas
void MenuCuatro() {
    beginLcd();
    lcd.print(F("MONITOR DE ALARMAS"));
    showNavigation();
}

// Visualización de Alarmas Activas
void MenuCuatroCero() {
    beginLcd();
    lcd.print(F("ALARMA ACTIVA:"));
    lcd.setCursor(0, 3);
    lcd.print(F("Enter Resetea"));
}

// REGISTRO DE ALARMAS
void MenuCinco() {
    beginLcd();
    lcd.print(F("REGISTRO DE ALARMAS"));
    showNavigation();
}

void MenuCincoCero() {
    beginLcd();
    lcd.print(F("ULTIMA ALARMA ACTIVA"));
    lcd.setCursor(0, 3);
    lcd.print(F("Enter borra"));
}
