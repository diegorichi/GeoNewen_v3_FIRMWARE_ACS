#include "measurement_and_calculations.h"

#define ONE_WIRE_BUS 22
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

uint8_t diTempInH[8] = {0x28, 0xDA, 0xB6, 0xF8, 0x1A, 0x19, 0x01, 0x8B};       // n1
uint8_t diTempCompresor[8] = {0x28, 0xE5, 0xAC, 0x26, 0x1B, 0x19, 0x01, 0x3C};  // n2
uint8_t diTempAdmision[8] = {0x28, 0x34, 0x76, 0x57, 0x1A, 0x19, 0x01, 0xA0};   // n10
uint8_t diTempInT[8] = {0x28, 0xD6, 0x3C, 0xE0, 0x1B, 0x19, 0x01, 0x0F};       // n7

uint8_t diTempDescarga[8] = {0x28, 0x49, 0x4B, 0x01, 0x1C, 0x19, 0x01, 0x1A};  // n11
uint8_t diTempAcs[8] = {0x28, 0xAE, 0x16, 0xFF, 0x1B, 0x19, 0x01, 0xD1};       // n12
uint8_t diTempOutH[8] = {0x28, 0x87, 0x9F, 0xE9, 0x1B, 0x19, 0x01, 0xF8};     // n13
uint8_t diTempOutT[8] = {0x28, 0x83, 0x69, 0x3D, 0x1B, 0x19, 0x01, 0x11};     // n14

float fCal = 1.055;  // 1.77;// caudalimetro  sen - hz21wa                    //1.9 caudalimetros  geo v1.0 y 2.0;

unsigned long ventanaCaudalH;
unsigned long ventanaCaudalT;
// Los caudalímetros pueden superar 255 pulsos dentro de una ventana de un
// segundo. uint8_t se desborda y hace perder pulsos en ciertos casos
volatile uint16_t pulsosCaudT;
volatile uint16_t pulsosCaudH;

unsigned long ultimoPedidoTemperatura = 0;
bool conversionTemperaturaPendiente = false;
const unsigned long TEMPERATURE_REQUEST_INTERVAL_MS = 1000;

const uint8_t DELTA_ACS_ELECTRICO = 7;

void initializeFlowState() {
    attachInterrupt(4, caudalHogar, FALLING);   // Pin 19
    attachInterrupt(5, caudalTierra, FALLING);  // Pin 18

    caudT = 0;
    caudH = 0;
    pulsosCaudT = 0;
    pulsosCaudH = 0;
    ventanaCaudalH = 0;
    ventanaCaudalT = 0;
}

// Función de Cuenta de Pulsos de Caudalímetro
void caudalTierra() {
    pulsosCaudT++;
}

// Función de Cuenta de Pulsos de Caudalímetro
void caudalHogar() {
    pulsosCaudH++;
}

void initializeTemperatureMeasurement() {
    sensors.begin();
    sensors.setWaitForConversion(false);
    ultimoPedidoTemperatura = millis() - TEMPERATURE_REQUEST_INTERVAL_MS;
}

bool delayedTemperatureMeasurement(void*) {
    float Temp_out_Haux = sensors.getTempC(diTempOutH);
    if ((Temp_out_Haux > -10.0 && Temp_out_Haux < -1.0) || (Temp_out_Haux > 1.0 && Temp_out_Haux < 80.0))
        tempOutH = Temp_out_Haux;

    float Temp_in_Haux = sensors.getTempC(diTempInH);
    if ((Temp_in_Haux > -10.0 && Temp_in_Haux < -1.0) || (Temp_in_Haux > 1.0 && Temp_in_Haux < 80.0))
        tempInH = Temp_in_Haux;

    float Temp_out_Taux = sensors.getTempC(diTempOutT);
    if (Temp_out_Taux > -10.0 && Temp_out_Taux < 80.0)
        tempOutT = Temp_out_Taux;

    float Temp_in_Taux = sensors.getTempC(diTempInT);
    if (Temp_in_Taux > -10.0 && Temp_in_Taux < 80.0)
        tempInT = Temp_in_Taux;

    float Temp_CompressorAux = sensors.getTempC(diTempCompresor);
    if ((Temp_CompressorAux > -10.0 && Temp_CompressorAux < -1.0) || (Temp_CompressorAux > 1.0 && Temp_CompressorAux < 80.0))
        tempCompressor = Temp_CompressorAux;

    float Temp_Descargaaux = sensors.getTempC(diTempDescarga);
    if ((Temp_Descargaaux > -10.0 && Temp_Descargaaux < -1.0) || (Temp_Descargaaux > 1.0 && Temp_Descargaaux < 90.0))
        tempDescarga = Temp_Descargaaux;

    float Temp_Admisionaux = sensors.getTempC(diTempAdmision);
    if ((Temp_Admisionaux > -10.0 && Temp_Admisionaux < -1.0) || (Temp_Admisionaux > 1.0 && Temp_Admisionaux < 60))
        tempAdmision = Temp_Admisionaux;

    float Temp_ACSaux = sensors.getTempC(diTempAcs);
    if (Temp_ACSaux > -10.0 && Temp_ACSaux < 80.0)
        tempAcs = Temp_ACSaux;

    return false;
}

void temperatureMeasurement() {
    unsigned long ahora = millis();

    if (conversionTemperaturaPendiente) {
        if (sensors.isConversionComplete()) {
            delayedTemperatureMeasurement(nullptr);
            conversionTemperaturaPendiente = false;
        }
        return;
    }

    if (ahora - ultimoPedidoTemperatura >= TEMPERATURE_REQUEST_INTERVAL_MS) {
        sensors.requestTemperatures();
        ultimoPedidoTemperatura = ahora;
        conversionTemperaturaPendiente = true;
    }
}

void flowsCalculation() {
    // Se contabilizan los pulsos de los caudalímetros durante un segundo, y se calcula el caudal
    if ((millis() - ventanaCaudalH) > 1000) {
        detachInterrupt(4);
        // Los cálculos resultan de la constante de pulsos/caudal indicados en la hoja de datos de los caudalímetros
        // El cálculo está escalado al tamaño de la ventana de muestreo, que puede no ser exactamente de 1 segundo
        caudH = ((60000.0 / (millis() - ventanaCaudalH)) * pulsosCaudH) * fCal;
        ventanaCaudalH = millis();
        pulsosCaudH = 0;
        // Las interrupciones se deshabilitan al principio del cálculo para no contabilizar pulsos de más, luego se reestablecen
        attachInterrupt(4, caudalHogar, FALLING);
    }

    if ((millis() - ventanaCaudalT) > 1000) {
        detachInterrupt(5);
        caudT = ((60000.0 / (millis() - ventanaCaudalT)) * pulsosCaudT) * fCal;
        ventanaCaudalT = millis();
        pulsosCaudT = 0;
        attachInterrupt(5, caudalTierra, FALLING);
    }
}

void flowControl() {
    if (senalStart && (enableFlowAlarm) && (estadoMaquina == 3 || estadoMaquina == 7)) {
        // Se comprueba que el caudal no sea inferior a un cierto valor, para evitar daños a las bombas
        flagCaudT = caudTacu < 100.0;
        flagCaudH = caudHacu < 100.0;
    }
}

void temperatureControl() {
    // Si la temperatura de operación del compresor es muy elevada o muy baja, se lo detiene para evitar daños
    if (tempCompressorAcu > 80.0) {
        contTempCompressor++;
        if (contTempCompressor > 3) {
            flagTempCompressor = true;
        }
    } else
        contTempCompressor = 0;

    if (tempDescargaAcu > 85.0) {
        contTempDescarga++;
        if (contTempDescarga > 3) {
            flagTempDescarga = true;
        }
    } else
        contTempDescarga = 0;

    if (tempAdmision < -7.5) {
        flagTempAdm = true;
    }
}

void presureControl() {
    // Si la presion de operación del compresor es muy elevada, se lo detiene para evitar daños
    if (digitalRead(diPresHi) == LOW) {
        contPressHi++;
        if (contPressHi > 3) {
            flagPresHi = true;
        }
    } else {
        contPressHi = 0;
        flagPresHi = false;
    }

    // Si la presion de operación del compresor es muy elevada, se lo detiene para evitar daños
    if (digitalRead(diPresLow) == LOW) {
        contPressLow++;
        if (contPressLow > 3) {
            flagPresLow = true;
        }
    } else {
        contPressLow = 0;
        flagPresLow = false;
    }
}

void auxiliaryACSHeatingControl() {
    // Si la temp ACS alcanza el objetivo, apagamos el calentador
    // Si la temp es menor al seteo, lo apago porque estado = 7 -> generar acs
    // si apago generac ACS no hay delta t final.
    if (((tempAcsAcu >= (acsSetpoint + DELTA_ACS_ELECTRICO)) && enableAcs) || ((tempAcsAcu <= (acsSetpoint - GAP_ACS)) && enableAcs) || !enableAcs || !enableAcsDeltaElectrico) {
        deltaAcsElectricResult = false;
    }

    // Si la temperatura baja del gap del objetivo, volvemos a prender el calendador
    //  pero solo si es mayor a la seteada, de manera tal que usamos el cartucho solo en el
    // ultimo tramo de ACS.
    if (tempAcsAcu < (acsSetpoint + DELTA_ACS_ELECTRICO - GAP_ACS) && (tempAcsAcu > acsSetpoint) && enableAcs && enableAcsDeltaElectrico) {
        deltaAcsElectricResult = true;
    }

    // si acs elect apagado -> lo apagamos
    // si acs apagado -> lo apagamos
    if (enableElectricAcs || (enableAcs && enableAcsDeltaElectrico && deltaAcsElectricResult)) {
        valorDoCalentador = HIGH;
    } else {
        valorDoCalentador = LOW;
    }
}

void temperatureCalculation() {
    t3Oh = t2Oh;
    t2Oh = t1Oh;
    t1Oh = tempOutH;
    tempOutHacu = (t1Oh + t2Oh + t3Oh) / 3;

    t3Ih = t2Ih;
    t2Ih = t1Ih;
    t1Ih = tempInH;
    tempInHacu = (t1Ih + t2Ih + t3Ih) / 3;

    c3T = c2T;
    c2T = c1T;
    c1T = caudT;
    caudTacu = (c1T + c2T + c3T) / 3;

    c3H = c2H;
    c2H = c1H;
    c1H = caudH;
    caudHacu = (c1H + c2H + c3H) / 3;

    t5Comp = t4Comp;
    t4Comp = t3Comp;
    t3Comp = t2Comp;
    t2Comp = t1Comp;
    t1Comp = tempCompressor;
    tempCompressorAcu = (t1Comp + t2Comp + t3Comp + t4Comp + t5Comp) / 5;

    t3Acs = t2Acs;
    t2Acs = t1Acs;
    t1Acs = tempAcs;
    tempAcsAcu = (t1Acs + t2Acs + t3Acs) / 3;

    t3Des = t2Des;
    t2Des = t1Des;
    t1Des = tempDescarga;
    tempDescargaAcu = (t1Des + t2Des + t3Des) / 3;
}
