#include "measurement_and_calculations.h"

#define ONE_WIRE_BUS 22
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

uint8_t DI_Temp_in_H[8] = {0x28, 0xDA, 0xB6, 0xF8, 0x1A, 0x19, 0x01, 0x8B};       // n1    ok
uint8_t DI_Temp_Compresor[8] = {0x28, 0xE5, 0xAC, 0x26, 0x1B, 0x19, 0x01, 0x3C};  // n2    ok
uint8_t DI_Temp_Admision[8] = {0x28, 0x34, 0x76, 0x57, 0x1A, 0x19, 0x01, 0xA0};   // n10   ok
uint8_t DI_Temp_out_T[8] = {0x28, 0xD6, 0x3C, 0xE0, 0x1B, 0x19, 0x01, 0x0F};      // n7   ok

uint8_t DI_Temp_Descarga[8] = {0x28, 0x49, 0x4B, 0x01, 0x1C, 0x19, 0x01, 0x1A};  // n11
uint8_t DI_Temp_ACS[8] = {0x28, 0xAE, 0x16, 0xFF, 0x1B, 0x19, 0x01, 0xD1};       // n12
uint8_t DI_Temp_out_H[8] = {0x28, 0x87, 0x9F, 0xE9, 0x1B, 0x19, 0x01, 0xF8};     // n13
uint8_t DI_Temp_in_T[8] = {0x28, 0x83, 0x69, 0x3D, 0x1B, 0x19, 0x01, 0x11};      // n14

float FCal = 1.055;  // 1.77;// caudalimetro  sen - hz21wa                    //1.9 caudalimetros  geo v1.0 y 2.0;

unsigned long Ventana_Caudal_H;
unsigned long Ventana_Caudal_T;
volatile uint8_t Pulsos_Caud_T;
volatile uint8_t Pulsos_Caud_H;

const uint8_t DELTA_ACS_ELECTRICO = 7;

void initializeFlowState() {
    attachInterrupt(4, Caudal1, FALLING);  // Pin 19
    attachInterrupt(5, Caudal2, FALLING);  // Pin 18

    Caud_T = 0;
    Caud_H = 0;
    Pulsos_Caud_T = 0;
    Pulsos_Caud_H = 0;
    Ventana_Caudal_H = 0;
    Ventana_Caudal_T = 0;
}

// Función de Cuenta de Pulsos de Caudalímetro
void Caudal1() {
    Pulsos_Caud_T++;
}

// Función de Cuenta de Pulsos de Caudalímetro
void Caudal2() {
    Pulsos_Caud_H++;
}

bool delayedTemperatureMeasurement(void*) {
    float Temp_out_Haux = sensors.getTempC(DI_Temp_out_H);
    if ((Temp_out_Haux > -10.0 && Temp_out_Haux < -1.0) || (Temp_out_Haux > 1.0 && Temp_out_Haux < 80.0))
        Temp_out_H = Temp_out_Haux;

    float Temp_in_Haux = sensors.getTempC(DI_Temp_in_H);
    if ((Temp_in_Haux > -10.0 && Temp_in_Haux < -1.0) || (Temp_in_Haux > 1.0 && Temp_in_Haux < 80.0))
        Temp_in_H = Temp_in_Haux;

    float Temp_out_Taux = sensors.getTempC(DI_Temp_out_T);
    if (Temp_out_Taux > -10.0 && Temp_out_Taux < 80.0)
        Temp_out_T = Temp_out_Taux;

    float Temp_in_Taux = sensors.getTempC(DI_Temp_in_T);
    if (Temp_in_Taux > -10.0 && Temp_in_Taux < 80.0)
        Temp_in_T = Temp_in_Taux;

    float Temp_CompressorAux = sensors.getTempC(DI_Temp_Compresor);
    if ((Temp_CompressorAux > -10.0 && Temp_CompressorAux < -1.0) || (Temp_CompressorAux > 1.0 && Temp_CompressorAux < 80.0))
        Temp_Compressor = Temp_CompressorAux;

    float Temp_Descargaaux = sensors.getTempC(DI_Temp_Descarga);
    if ((Temp_Descargaaux > -10.0 && Temp_Descargaaux < -1.0) || (Temp_Descargaaux > 1.0 && Temp_Descargaaux < 90.0))
        Temp_Descarga = Temp_Descargaaux;

    float Temp_Admisionaux = sensors.getTempC(DI_Temp_Admision);
    if ((Temp_Admisionaux > -10.0 && Temp_Admisionaux < -1.0) || (Temp_Admisionaux > 1.0 && Temp_Admisionaux < 60))
        Temp_Admision = Temp_Admisionaux;

    float Temp_ACSaux = sensors.getTempC(DI_Temp_ACS);
    if (Temp_ACSaux > -10.0 && Temp_ACSaux < 80.0)
        Temp_ACS = Temp_ACSaux;

    return false;
}

void temperatureMeasurement() {
    // Se toma una lectura de los sensores DS18B20 cada 5 segundos, demoran aproximadamente 200ms en entregar un resultado, 750ms máx
    // se les envía un comando para que inicien la toma de datos
    sensors.requestTemperatures();
    timer_things.in(200, delayedTemperatureMeasurement);
}

void flowsCalculation() {
    // Se contabilizan los pulsos de los caudalímetros durante un segundo, y se calcula el caudal
    if ((millis() - Ventana_Caudal_H) > 1000) {
        detachInterrupt(4);
        // Los cálculos resultan de la constante de pulsos/caudal indicados en la hoja de datos de los caudalímetros
        // El cálculo está escalado al tamaño de la ventana de muestreo, que puede no ser exactamente de 1 segundo
        Caud_H = ((60000.0 / (millis() - Ventana_Caudal_H)) * Pulsos_Caud_H) * FCal;
        Ventana_Caudal_H = millis();
        Pulsos_Caud_H = 0;
        // Las interrupciones se deshabilitan al principio del cálculo para no contabilizar pulsos de más, luego se reestablecen
        attachInterrupt(4, Caudal1, FALLING);
    }

    if ((millis() - Ventana_Caudal_T) > 1000) {
        detachInterrupt(5);
        Caud_T = ((60000.0 / (millis() - Ventana_Caudal_T)) * Pulsos_Caud_T) * FCal;
        Ventana_Caudal_T = millis();
        Pulsos_Caud_T = 0;
        attachInterrupt(5, Caudal2, FALLING);
    }
}

void flowControl() {
    if (senal_start && (EnableFlowAlarm) && (Estado_Maquina == 3 || Estado_Maquina == 7)) {
        // Se comprueba que el caudal no sea inferior a un cierto valor, para evitar daños a las bombas
        Flag_CaudT = Caud_Tacu < 100.0;
        Flag_CaudH = Caud_Hacu < 100.0;
    }
}

void temperatureControl() {
    // Si la temperatura de operación del compresor es muy elevada o muy baja, se lo detiene para evitar daños
    if (Temp_CompressorAcu > 80.0) {
        Cont_Temp_Compressor++;
        if (Cont_Temp_Compressor > 3) {
            Flag_TempCompressor = true;
        }
    } else
        Cont_Temp_Compressor = 0;

    if (Temp_Descargaacu > 85.0) {
        Cont_Temp_Descarga++;
        if (Cont_Temp_Descarga > 3) {
            Flag_Temp_Descarga = true;
        }
    } else
        Cont_Temp_Descarga = 0;

    if (Temp_Admision < -7.5) {
        Flag_Temp_Adm = true;
    }
}

void presureControl() {
    // Si la presion de operación del compresor es muy elevada, se lo detiene para evitar daños
    if (digitalRead(DI_Pres_HI) == LOW) {
        Cont_Press_HI++;
        if (Cont_Press_HI > 3) {
            Flag_PresHI = true;
        }
    } else {
        Cont_Press_HI = 0;
        Flag_PresHI = false;
    }

    // Si la presion de operación del compresor es muy elevada, se lo detiene para evitar daños
    if (digitalRead(DI_Pres_LOW) == LOW) {
        Cont_Press_LOW++;
        if (Cont_Press_LOW > 3) {
            Flag_PresLOW = true;
        }
    } else {
        Cont_Press_LOW = 0;
        Flag_PresLOW = false;
    }
}

void auxiliaryACSHeatingControl() {
    // Si la temp ACS alcanza el objetivo, apagamos el calentador
    // Si la temp es menor al seteo, lo apago porque estado = 7 -> generar acs
    // si apago generac ACS no hay delta t final.
    if (((Temp_ACSacu >= (SetP_ACS + DELTA_ACS_ELECTRICO)) && EnableACS) || ((Temp_ACSacu <= (SetP_ACS - GAP_ACS)) && EnableACS) || !EnableACS || !EnableACS_DeltaElectrico) {
        deltaACSElectricResult = false;
    }

    // Si la temperatura baja del gap del objetivo, volvemos a prender el calendador
    //  pero solo si es mayor a la seteada, de manera tal que usamos el cartucho solo en el
    // ultimo tramo de ACS.
    if (Temp_ACSacu < (SetP_ACS + DELTA_ACS_ELECTRICO - GAP_ACS) && (Temp_ACSacu > SetP_ACS) && EnableACS && EnableACS_DeltaElectrico) {
        deltaACSElectricResult = true;
    }

    // si acs elect apagado -> lo apagamos
    // si acs apagado -> lo apagamos
    if (EnableElectricACS || (EnableACS && EnableACS_DeltaElectrico && deltaACSElectricResult)) {
        Valor_DO_Calentador = HIGH;
    } else {
        Valor_DO_Calentador = LOW;
    }
}

void temperatureCalculation() {
    T3_OH = T2_OH;
    T2_OH = T1_OH;
    T1_OH = Temp_out_H;
    Temp_out_Hacu = (T1_OH + T2_OH + T3_OH) / 3;

    T3_IH = T2_IH;
    T2_IH = T1_IH;
    T1_IH = Temp_in_H;
    Temp_in_Hacu = (T1_IH + T2_IH + T3_IH) / 3;

    C3_T = C2_T;
    C2_T = C1_T;
    C1_T = Caud_T;
    Caud_Tacu = (C1_T + C2_T + C3_T) / 3;

    C3_H = C2_H;
    C2_H = C1_H;
    C1_H = Caud_H;
    Caud_Hacu = (C1_H + C2_H + C3_H) / 3;

    T5_Comp = T4_Comp;
    T4_Comp = T3_Comp;
    T3_Comp = T2_Comp;
    T2_Comp = T1_Comp;
    T1_Comp = Temp_Compressor;
    Temp_CompressorAcu = (T1_Comp + T2_Comp + T3_Comp + T4_Comp + T5_Comp) / 5;

    T3_ACS = T2_ACS;
    T2_ACS = T1_ACS;
    T1_ACS = Temp_ACS;
    Temp_ACSacu = (T1_ACS + T2_ACS + T3_ACS) / 3;

    T3_Des = T2_Des;
    T2_Des = T1_Des;
    T1_Des = Temp_Descarga;
    Temp_Descargaacu = (T1_Des + T2_Des + T3_Des) / 3;
}
