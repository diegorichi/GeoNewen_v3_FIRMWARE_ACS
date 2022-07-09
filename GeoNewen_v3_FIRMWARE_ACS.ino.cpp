# 1 "C:\\Users\\Usuario\\AppData\\Local\\Temp\\tmpamxmyk8_"
#include <Arduino.h>
# 1 "E:/My Documents/PlatformIO/Projects/220705-143609-megaatmega2560/src/GeoNewen_v3_FIRMWARE_ACS.ino"
#include "vars.h"
#include "keyboard.h"
#include "functionsLCDMenu.h"
#include "routines.h"
#include "kume_eeprom.h"
#include "thingspeak.h"

#define ONE_WIRE_BUS 22
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
void setup();
void loop();
void Caudal1();
void Caudal2();
float getVPP();
#line 16 "E:/My Documents/PlatformIO/Projects/220705-143609-megaatmega2560/src/GeoNewen_v3_FIRMWARE_ACS.ino"
void setup() {

  wdt_disable();

  keyboardSetup();

  setupDigitalInputs();

  setupDigitalOuputs();

  initializeDigitalOuputs();

  attachInterrupt(1, AtencionTeclado, FALLING);
  attachInterrupt(4, Caudal1, FALLING);
  attachInterrupt(5, Caudal2, FALLING);

  lcdCreateSpecialChars();

  Caud_T = 0;
  Caud_H = 0;
  Pulsos_Caud_T = 0;
  Pulsos_Caud_H = 0;
  Ventana_Caudal1 = 0;
  Ventana_Caudal2 = 0;
  Estado_Maquina = 0;
  Estado_Bombas = 0;
  Estado_Comp = 0;
  Activacion_Comp = 0;
  Periodo_Refresco_Wifi = 0;
  Estado_ConfigWIFI = 0;
  Activacion_Bombas = 0;
  Periodo_Bombas = 0;
  A_RMS = 0;
  Valor_DO_VACS = HIGH;
  Valor_DO_V4V = LOW;




  Flag_TempIntXT_Baja = false;
  Flag_TempIntXT_Alta = false;
  Flag_TempIntXH_Baja = false;
  Flag_TempIntXH_Alta = false;
  Flag_TempTriacs = false;
  Flag_TempComp01 = false;
  Flag_CaudT = false;
  Flag_CaudH = false;
  Flag_PresHI = false;
  Flag_PresLOW = false;
  Flag_Alarma_General = false;
  Flag_Caldera = false;
  SenalComp = LOW;
  ModoFrioCalor = false;
  Modo_Funcionamiento = false;

  Cont_Temp_intX_T = 0;
  Cont_Temp_intX_T2 = 0;
  Cont_Temp_intX_H = 0;
  Cont_Temp_intX_H2 = 0;
  Cont_Temp_Comp_01 = 0;
  Cont_Caud_T = 0;
  Cont_Caud_H = 0;
  Cont_Press_HI = 0;
  Cont_Press_LOW = 0;
  W = "";
  w = ' ';

  Timer1.initialize(100000);

  Serial.begin(115200);
  Serial3.begin(115200);

  checkESP();

  if (Flag_ESP) {
    Serial3.println("AT+CWMODE=1");
    delay(100);
    Serial3.println("AT+CWSTOPSMART");
    delay(100);
    Serial3.println("AT+CIPMUX=0");
    delay(100);
    Serial.println("ESP Inicializado");
    delay(100);


    checkWifi();
  }
  else {
    Serial.println("Fallo en inicializacion de ESP");
  }

  EEPROMLectura();

  if (SetP_ACS < 30) {
    SetP_ACS = 30;
  }
  if (SetP_ACS > 48) {
    SetP_ACS = 48;
  }
  SetP_ACS_Edit = SetP_ACS;

  MenuActual = 0;
  MenuCero();

  wdt_enable(WDTO_8S);
}





void loop() {

  if (Estado_ConfigWIFI == 0) {


    if ((millis() - LecturaDSB) > 5000)
    {
      sensors.requestTemperatures();
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


    {
      Pulsos_Caud_H_Bis = Pulsos_Caud_H;
      if ((millis() - Ventana_Caudal1) > 1000)
      {
        detachInterrupt(4);
        Caud_H = ((60000.0 / (millis() - Ventana_Caudal1)) * Pulsos_Caud_H) * FCal;

        Ventana_Caudal1 = millis();
        Pulsos_Caud_H = 0;
        attachInterrupt(4, Caudal1, FALLING);
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
# 206 "E:/My Documents/PlatformIO/Projects/220705-143609-megaatmega2560/src/GeoNewen_v3_FIRMWARE_ACS.ino"
    Potencia = 220 * TI * CosFI;

    if ((millis() - RefrescoWifi) > 30000)
    {
      checkESP();
      checkWifi();
      RefrescoWifi = millis();
    }

    if (((millis() - Periodo_Refresco_Wifi) > 60000)
      && Flag_Wifi)
    {
      wdt_reset();
      ThingSUpdate();
      delay(1000);

      Periodo_Refresco_Wifi = millis();
    }

    wdt_reset();


    if (millis() - Periodo_Refresco > 1000)
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

      lcdRefreshValues();



      Periodo_Refresco = millis();
    }






    {
      if ((Caud_Tacu < 100.0) && (digitalRead(DI_Marcha_on) == HIGH) && (Flag_Alarma_EN == true) && (Estado_Maquina == 3 || (Estado_Maquina == 7 && Flag_retardo_e7 == true)))
      {
        Flag_CaudT = true;
      }

      if (Caud_Tacu > 100.0)
        Flag_CaudT = false;

      if (Caud_Hacu < 100.0 && (digitalRead(DI_Marcha_on) == HIGH) && (Flag_Alarma_EN == true) && (Estado_Maquina == 3 || (Estado_Maquina == 7 && Flag_retardo_e7 == true)))
      {
        Flag_CaudH = true;
      }

      if (Caud_Hacu > 100.0)
        Flag_CaudH = false;
    }


    {
      if (Temp_comp_acu > 80.0)
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

      if (Temp_Admision < -4) {
        Flag_Temp_Adm = true;
      }
    }


    {
      if (digitalRead(DI_Pres_HI) == LOW)
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

      if (digitalRead(DI_Pres_LOW) == LOW)
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


    {
      if (((Temp_ACSacu >= (SetP_ACS + 7)) && Flag_ACS_EN)
        || ((Temp_ACSacu < SetP_ACS) && Flag_ACS_EN)
        || !Flag_ACS_EN)
      {
        flag_dtElectrico_final = false;
      }




      if (Temp_ACSacu < (SetP_ACS + 5) && (Temp_ACSacu > SetP_ACS) && Flag_ACS_EN) {
        flag_dtElectrico_final = true;
      }



      if (Flag_ACS_EN_ELECT || (Flag_ACS_EN && flag_dtElectrico_final)) {
        Valor_DO_Calentador = HIGH;
      }
      else {
        Valor_DO_Calentador = LOW;
      }
    }


    if (Estado_Maquina == 0)
    {
      descanso = 0;


      Flag_Marcha_ON = false;
      Valor_DO_Comp_01 = LOW;
      Estado_Comp = 0;
      Activacion_Comp = 0;
      Valor_DO_Bombas = LOW;
      Valor_DO_Aux = LOW;
      Cal = true;
      Cal_ACS = false;
      Valor_DO_VACS = LOW;
      Valor_DO_V4V = LOW;
      EsperaValv = millis();
      Estado_Maquina = 1;
      Flag_TempComp01 = false;



    }

    if (Estado_Maquina == 1)
    {

      if (ModoFrioCalor == true)
        Valor_DO_V4V = LOW;
      if (ModoFrioCalor == false)
        Valor_DO_V4V = HIGH;

      if ((millis() - Periodo_Bombas) > 86400000)
      {
        Valor_DO_Bombas = HIGH;

        digitalWrite(DO_Buzzer, HIGH);
        Periodo_Bombas = millis();
        Activacion_Bombas = millis();
      }
      if ((millis() - Activacion_Bombas) > 10000) {
        Valor_DO_Bombas = LOW;

        digitalWrite(DO_Buzzer, LOW);
      }

      if (Estado_Comp == 0 && Flag_ACS_EN == false)
      {
        if (ModoFrioCalor == true)
        {
          Valor_DO_VACS = LOW;
        }
        else
          Valor_DO_VACS = HIGH;
      }

      if ((millis() - EsperaValv) > 15000) {
        if (Temp_ACS < (SetP_ACS - 2) && Flag_ACS_EN == true)
        {
          Valor_DO_V4V = HIGH;


          Valor_DO_Bombas = HIGH;
          Valor_DO_VACS = HIGH;
          Nro_Alarma = 0;
          Estado_Maquina = 7;
          Ingreso_E7 = millis();
        }
      }

      if (Flag_ACS_EN == false)
      {
        Valor_DO_VACS = LOW;
      }

      if (digitalRead(DI_Marcha_on) == HIGH && Flag_Marcha_ON == false)
      {
        Salto_E1 = millis();
        Flag_Marcha_ON = true;
      }

      if ((digitalRead(DI_Marcha_on) == LOW && Flag_Marcha_ON == true))
      {
        Flag_Marcha_ON = false;
      }

      if ((millis() - Salto_E1 > E1_a_E2) && Modo_Funcionamiento == false && digitalRead(DI_Marcha_on) == HIGH)
      {
        Estado_Maquina = 2;
      }
    }

    if (Estado_Maquina == 2)
    {
# 487 "E:/My Documents/PlatformIO/Projects/220705-143609-megaatmega2560/src/GeoNewen_v3_FIRMWARE_ACS.ino"
      Valor_DO_Bombas = HIGH;

      Nro_Alarma = 0;
      if (Estado_Comp == 0) {
        Valor_DO_Comp_01 = HIGH;
        Estado_Comp = 1;

        Activacion_Comp = millis();
      }

      if (digitalRead(DI_Marcha_on) == LOW) {
        Estado_Maquina = 0;
        Flag_Marcha_ON = false;
      }

      if (millis() - Activacion_Comp > 10000)
      {
        Estado_Maquina = 3;
        Ingreso_E3 = millis();
      }
# 532 "E:/My Documents/PlatformIO/Projects/220705-143609-megaatmega2560/src/GeoNewen_v3_FIRMWARE_ACS.ino"
      if (Modo_Funcionamiento == true) {
        Estado_Maquina = 0;
      }
    }

    if (Estado_Maquina == 3)
    {

      if (Temp_ACS < (SetP_ACS - 2) && Flag_ACS_EN)
      {
        Estado_Maquina = 0;
      }




      Valor_DO_VACS = LOW;

      if (digitalRead(DI_Marcha_on) == LOW || Flag_Caldera) {
        Estado_Maquina = 0;
        Flag_Marcha_ON = false;
      }


      if ((millis() - Ingreso_E3) > 1000) {
        if (Flag_TempComp01 || PressOK || Flag_CaudT || Flag_CaudH || Flag_Temp_Adm || Flag_Temp_Descarga)
        {
          Estado_Maquina = 4;
          MenuCuatroCero();
          MenuActual = 40;
        }
      }



      if (Temp_out_H > 45.0)
      {
        Estado_Maquina = 6;
        Ingreso_Descanso = millis();
      }
      if (Temp_out_H < 10.0)
      {
        Estado_Maquina = 6;
        Ingreso_Descanso = millis();
      }
      if (Temp_out_T > 40.0)
      {
        Estado_Maquina = 6;
        Ingreso_Descanso = millis();
      }
      if (Temp_out_T < 0)
      {
        Estado_Maquina = 6;
        Ingreso_Descanso = millis();
      }
      if (Temp_Admision < -4)
      {
        Estado_Maquina = 6;
        Ingreso_Descanso = millis();
      }



      if (Temp_out_Hacu > 50.0) {
        Estado_Maquina = 8;
        Ingreso_Descanso = millis();
      }

      if (Modo_Funcionamiento == true) {
        Estado_Maquina = 0;
      }
    }

    if (Estado_Maquina == 4)
    {
      if (Estado_Comp == 1) {
        Valor_DO_Comp_01 = LOW;
        Apagado_Comp = millis();
        Estado_Comp = 0;
        Activacion_Comp = 0;
      }

      Valor_DO_Comp_01 = LOW;
      Estado_Comp = 0;
      Activacion_Comp = 0;
      Valor_DO_Bombas = LOW;
      Valor_DO_Aux = LOW;
      Valor_DO_Marcha_Ext = LOW;
      digitalWrite(DO_Triac_01, LOW);

      if (Alarma_Activa == false) {
        Alarmas();
        Alarma_Activa = true;
      }
      if (Nro_Alarma != 0) {
        Flag_Alarma_General = true;
        if (Flag_Buzzer == false) {
          Timer1.pwm(DO_Buzzer, 100, 1000000);
          Flag_Buzzer = true;
        }
      }



      wdt_reset();
    }

    if (Estado_Maquina == 6)
    {
      Valor_DO_Comp_01 = LOW;
      Valor_DO_Bombas = LOW;
      Estado_Comp = 0;
      if ((millis() - Ingreso_Descanso > 400000))
      {
        Estado_Maquina = 0;
      }
    }

    if (Estado_Maquina == 7)
    {
      if (millis() - Ingreso_E7 > 5000) {
        Flag_retardo_e7 = true;
        if (Flag_TempComp01 == true || PressOK == false || Flag_CaudT == true || Flag_CaudH == true || Flag_Temp_Adm == true || Flag_Temp_Descarga == true)
        {
          Estado_Maquina = 4;
          MenuCuatroCero();
          MenuActual = 40;
        }

        if (Temp_ACSacu > SetP_ACS) {
          flag_dtElectrico_final = true;
          Valor_DO_V4V = LOW;
          Valor_DO_VACS = HIGH;
          Cal_ACS = false;
          Cal = true;
          Estado_Maquina = 0;
          Flag_retardo_e7 = false;
        }







        if ((millis() - Ingreso_E7) > 30000) {
          Valor_DO_Comp_01 = HIGH;
        }


      }
      if (Temp_out_H > 50.0 || Temp_Descarga > 80.0)
      {
        Estado_Maquina = 71;
        Ingreso_E71 = millis();
        Flag_retardo_e7 = false;
      }

      if (Temp_ACS >= SetP_ACS && ModoFrioCalor == false)
      {
        Estado_Maquina = 0;
        Flag_retardo_e7 = false;
      }
      else if (Temp_ACS >= SetP_ACS && ModoFrioCalor == true && Estado_Comp == 1)
      {
        Valor_DO_Comp_01 = LOW;
        Estado_Comp = 0;
        Valor_DO_Bombas = HIGH;
        Periodo_Fin_ACS = millis();
        Estado_Maquina = 8;
        Flag_retardo_e7 = false;
      }
    }

    if (Estado_Maquina == 71)
    {
      Valor_DO_Comp_01 = LOW;
      Estado_Comp = 0;
      Valor_DO_Bombas = HIGH;
      if ((millis() - Ingreso_E71) > 90000)
      {
        Valor_DO_Bombas = HIGH;
        Estado_Maquina = 7;
        Ingreso_E7 = millis();
      }
    }

    if (Estado_Maquina == 8)
    {
      if ((millis() - Periodo_Fin_ACS) > 30000) {
        Estado_Maquina = 0;
      }
    }


    digitalWrite(DO_Bombas, Valor_DO_Bombas);
    digitalWrite(DO_Calentador, Valor_DO_Calentador);
    digitalWrite(DO_Val2, Valor_DO_V4V);
    digitalWrite(DO_Comp_01, Valor_DO_Comp_01);
    digitalWrite(DO_Val1, Valor_DO_VACS);

    wdt_reset();
  }
  else if (Estado_ConfigWIFI == 1)
  {

    wdt_disable();
    if (Serial3.available())
    {
      w = Serial3.read();
      Serial.print(w);
      W = W + w;
    }

    if (Serial.available())
    {
      char s = Serial.read();
      Serial3.print(s);
    }

    if (w == '\n')
    {
      if (W.indexOf("connected") > 0)
      {
        Serial3.println("AT+CWSTOPSMART");
        delay(500);

        checkWifi();

        Estado_ConfigWIFI = Flag_Wifi ? 0 : 1;
      }

      W = "";
      w = ' ';
    }

    wdt_enable(WDTO_8S);
  }

}

void Caudal1()
{
  Pulsos_Caud_T++;
}

void Caudal2()
{
  Pulsos_Caud_H++;
}

float getVPP()
{
  float VPP;
  int readValue;
  int maxValue = 0;
  int minValue = 1024;
  uint32_t start_time = millis();
  while ((millis() - start_time) < 1000)
  {
    readValue = analogRead(AI_TI);

    if (readValue > maxValue) {

      maxValue = readValue;
    }
    if (readValue < minValue) {

      minValue = readValue;
    }
  }

  VPP = ((maxValue - minValue) * 5.0) / 1024.0;

  return VPP;
}