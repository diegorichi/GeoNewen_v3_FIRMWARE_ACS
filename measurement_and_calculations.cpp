#include "measurement_and_calculations.h"

#define ONE_WIRE_BUS 22
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void Caudal1() // Función de Cuenta de Pulsos de Caudalímetro
{
  Pulsos_Caud_T++;
}

void Caudal2() // Función de Cuenta de Pulsos de Caudalímetro
{
  Pulsos_Caud_H++;
}

void temperatureMeasurement() {
  if ((millis() - LecturaDSB) > 5000) // Se toma una lectura de los sensores DS18B20 cada 5 segundos, demoran aproximadamente 200ms en entregar un resultado, 750ms máx
  {
    sensors.requestTemperatures(); // se les envía un comando para que inicien la toma de datos
    delay(100);

    Temp_Admisionaux = sensors.getTempC(DI_Temp_Admision);
    if ((Temp_Admisionaux > -10.0 && Temp_Admisionaux < -1.0) || (Temp_Admisionaux > 1.0 && Temp_Admisionaux < 60))
      Temp_Admision = Temp_Admisionaux;

    Temp_out_Haux = sensors.getTempC(DI_Temp_out_H);
    if ((Temp_out_Haux > -10.0 && Temp_out_Haux < -1.0) || (Temp_out_Haux > 1.0 && Temp_out_Haux < 80.0))
      Temp_out_H = Temp_out_Haux;

    Temp_in_Haux = sensors.getTempC(DI_Temp_in_H);
    if ((Temp_in_Haux > -10.0 && Temp_in_Haux < -1.0) || (Temp_in_Haux > 1.0 && Temp_in_Haux < 80.0))
      Temp_in_H = Temp_in_Haux;

    Temp_out_Taux = sensors.getTempC(DI_Temp_out_T);
    if (Temp_out_Taux > -10.0 && Temp_out_Taux < 80.0)
      Temp_out_T = Temp_out_Taux;

    Temp_in_Taux = sensors.getTempC(DI_Temp_in_T);
    if (Temp_in_Taux > -10.0 && Temp_in_Taux < 80.0)
      Temp_in_T = Temp_in_Taux;

    Temp_Comp_01aux = sensors.getTempC(DI_Temp_Compresor);
    if ((Temp_Comp_01aux > -10.0 && Temp_Comp_01aux < -1.0) || (Temp_Comp_01aux > 1.0 && Temp_Comp_01aux < 80.0))
      Temp_Comp_01 = Temp_Comp_01aux;

    Temp_Descargaaux = sensors.getTempC(DI_Temp_Descarga);
    if ((Temp_Descargaaux > -10.0 && Temp_Descargaaux < -1.0) || (Temp_Descargaaux > 1.0 && Temp_Descargaaux < 90.0))
      Temp_Descarga = Temp_Descargaaux;

    Temp_ACSaux = sensors.getTempC(DI_Temp_ACS);
    if (Temp_ACSaux > -10.0 && Temp_ACSaux < 80.0)
      Temp_ACS = Temp_ACSaux;
  }
}

void flowsCalculation() {
  Pulsos_Caud_H_Bis = Pulsos_Caud_H;       // Variable auxiliar para visualizar los pulsos contados directamente
  if ((millis() - Ventana_Caudal1) > 1000) // Se contabilizan los pulsos de los caudalímetros durante un segundo, y se calcula el caudal
  {
    detachInterrupt(4);
    Caud_H = ((60000.0 / (millis() - Ventana_Caudal1)) * Pulsos_Caud_H) * FCal; // Los cálculos resultan de la constante de pulsos/caudal indicados en la hoja de datos de los caudalímetros
    Ventana_Caudal1 = millis();                                                 // El cálculo está escalado al tamaño de la ventana de muestreo, que puede no ser exactamente de 1 segundo
    Pulsos_Caud_H = 0;
    attachInterrupt(4, Caudal1, FALLING); // Las interrupciones se deshabilitan al principio del cálculo para no contabilizar pulsos de más, luego se reestablecen
  }
  Pulsos_Caud_T_Bis = Pulsos_Caud_T;
  if ((millis() - Ventana_Caudal2) > 1000) {
    detachInterrupt(5);
    Caud_T = ((60000.0 / (millis() - Ventana_Caudal2)) * Pulsos_Caud_T) * FCal;
    Ventana_Caudal2 = millis();
    Pulsos_Caud_T = 0;
    attachInterrupt(5, Caudal2, FALLING);
  }
}

void thermalEfficiencyCalculation() {
  // Ef_Termica_1 = (Temp_in_T - Temp_out_T) * Caud_H;  //recordar caudales estan cruzados
  // Ef_Termica_2 = (Temp_in_T - Temp_out_T) * Caud_T;

  // Cálculo de la Corriente AC

  // Voltage = getVPP();                       //La llamada a la función getVPP devuelve el valor pico a pico de la tensión muestreada
  // V_RMS = (Voltage / 2.0) * 0.707;          //Ese valor pico a pico se divide por 2 y se multiplica por 0.707 para obtener el valor eficaz (aproximado)
  // A_RMS = (V_RMS * 1000) / mVperAmp;        //En función del valor eficaz y del módulo empleado, se usa la constante de equivalencia correspondiente y se obtiene el valor de la corriente
}

void powerCalculation() {
  Potencia = 220 * TI * CosFI; // Valor estimativo
}

void flowControl() {
  if ((Caud_Tacu < 100.0) && senal_start && (Flag_Alarma_EN == true) && (Estado_Maquina == 3 || (Estado_Maquina == 7 && Flag_retardo_e7 == true))) // Se comprueba que el caudal no sea inferior a un cierto valor, para evitar daños a las bombas
  {
    Flag_CaudT = true;
  }

  if (Caud_Tacu > 100.0)
    Flag_CaudT = false;

  if (Caud_Hacu < 100.0 && senal_start && (Flag_Alarma_EN == true) && (Estado_Maquina == 3 || (Estado_Maquina == 7 && Flag_retardo_e7 == true))) // Se comprueba que el caudal no sea inferior a un cierto valor, para evitar daños a las bombas
  {
    Flag_CaudH = true;
  }

  if (Caud_Hacu > 100.0)
    Flag_CaudH = false;
}

void temperatureControl() {
  if (Temp_comp_acu > 80.0) // Si la temperatura de operación del compresor es muy elevada o muy baja, se lo detiene para evitar daños
  {
    Cont_Temp_Comp_01++;
    if (Cont_Temp_Comp_01 > 3) {
      Flag_TempComp01 = true;
    }
  }
  else
    Cont_Temp_Comp_01 = 0;

  if (Temp_Descargaacu > 85.0) {
    Cont_Temp_Descarga++;
    if (Cont_Temp_Descarga > 3) {
      Flag_Temp_Descarga = true;
    }
  }
  else
    Cont_Temp_Descarga = 0;

  if (Temp_Admision < -7.5) {
    Flag_Temp_Adm = true;
  }
}

void presureControl() {
  if (digitalRead(DI_Pres_HI) == LOW) // Si la presion de operación del compresor es muy elevada, se lo detiene para evitar daños
  {
    Cont_Press_HI++;
    if (Cont_Press_HI > 3) {
      Flag_PresHI = true;
    }
  }
  else {
    Cont_Press_HI = 0;
    Flag_PresHI = false;
  }

  if (digitalRead(DI_Pres_LOW) == LOW) // Si la presion de operación del compresor es muy elevada, se lo detiene para evitar daños
  {
    Cont_Press_LOW++;
    if (Cont_Press_LOW > 3) {
      Flag_PresLOW = true;
    }
  }
  else {
    Cont_Press_LOW = 0;
    Flag_PresLOW = false;
  }

  if (Flag_PresHI == true || Flag_PresLOW == true) {
    PressOK = false;
  }
  else
    PressOK = true;
}

void auxiliaryACSHeatingControl() {
  if (((Temp_ACSacu >= (SetP_ACS + DELTA_ACS_ELECTRICO)) && Flag_ACS_EN)    // Si la temp ACS alcanza el objetivo, apagamos el calentador
      || ((Temp_ACSacu <= (SetP_ACS - GAP_ACS)) && Flag_ACS_EN) // Si la temp es menor al seteo, lo apago porque estado = 7 -> generar acs
      || !Flag_ACS_EN || !Flag_ACS_DT_EN)                 // si apago generac ACS no hay delta t final.
  {
    flag_dtElectrico_final = false;
  }

  // Si la temperatura baja del gap del objetivo, volvemos a prender el calendador
  //  pero solo si es mayor a la seteada, de manera tal que usamos el cartucho solo en el
  // ultimo tramo de ACS.
  if (Temp_ACSacu < (SetP_ACS + DELTA_ACS_ELECTRICO - GAP_ACS) 
    && (Temp_ACSacu > SetP_ACS) && Flag_ACS_EN && Flag_ACS_DT_EN) {
    flag_dtElectrico_final = true;
  }

  // si acs elect apagado -> lo apagamos
  // si acs apagado -> lo apagamos
  if (Flag_ACS_EN_ELECT || (Flag_ACS_EN && Flag_ACS_DT_EN && flag_dtElectrico_final)) {
    Valor_DO_Calentador = HIGH;
  }
  else {
    Valor_DO_Calentador = LOW;
  }
}

void temperatureCalculation()
{
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
  T1_Comp = Temp_Comp_01;
  Temp_comp_acu = (T1_Comp + T2_Comp + T3_Comp + T4_Comp + T5_Comp) / 5;

  T3_ACS = T2_ACS;
  T2_ACS = T1_ACS;
  T1_ACS = Temp_ACS;
  Temp_ACSacu = (T1_ACS + T2_ACS + T3_ACS) / 3;

  T3_Des = T2_Des;
  T2_Des = T1_Des;
  T1_Des = Temp_Descarga;
  Temp_Descargaacu = (T1_Des + T2_Des + T3_Des) / 3;
}

float getVPP() // Función Auxiliar para determinación del valor de Tensión
{
  float VPP;
  int readValue;       // value read from the sensor
  int maxValue = 0;    // store max value here
  int minValue = 1024; // store min value here
  uint32_t start_time = millis();
  while ((millis() - start_time) < 1000) // sample for 1 Sec
  {
    readValue = analogRead(AI_TI);
    // see if you have a new maxValue
    if (readValue > maxValue) {
      /*record the maximum sensor value*/
      maxValue = readValue;
    }
    if (readValue < minValue) {
      /*record the maximum sensor value*/
      minValue = readValue;
    }
  }
  // Subtract min from max
  VPP = ((maxValue - minValue) * 5.0) / 1024.0;

  return VPP;
}
