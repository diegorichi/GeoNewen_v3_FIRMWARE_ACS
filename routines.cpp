#include "routines.h"

const char MSG_WAIT[] PROGMEM = "POR FAVOR, ESPERE";
const char MSG_CHANGING_MODE[] PROGMEM = "CAMBIANDO MODO...";

void FrioCalor() // Función de cambio de Modo de Funcionamiento  (Bromberg: modo frio = valvula de 4 vias APAGADA)
{

  Valor_DO_Comp_01 = LOW; // Primer paso es apagar el compresor
  Estado_Comp = 0;
  Activacion_Comp = 0;
  // Modo_Funcionamiento = true;
  lcd.clear();
  lcd.write(MSG_WAIT);
  lcd.setCursor(0, 2);
  lcd.write(MSG_CHANGING_MODE);
  for (unsigned long i = 0; i < 3000000; i++) // Se espera un cierto tiempo antes de activar la válvula de 4 vías
  {
    wdt_reset();
  }
  if (ModoFrioCalor) // Se activa la válvula
  {
    Valor_DO_V4V = HIGH;
    ModoFrioCalor = false;
  }
  else {
    Valor_DO_V4V = LOW;
    ModoFrioCalor = true;
  }

  for (unsigned long i = 0; i < 200000; i++) {
    wdt_reset();
  }
  MenuActual = 20;
  MenuDosCero();
  // Estado_Maquina = 0;  //Al cambiar el modo de funcionamiento se resetea el sistema
}

int Alarmas() // Función de identificación de Alarma Activa
{
  Flag_Alarma_General = false;
  Nro_Alarma = 0;
  if (Flag_TempIntXT_Baja) {
    Nro_Alarma = 1;
  }
  else if (Flag_TempIntXT_Alta) {
    Nro_Alarma = 2;
  }
  else if (Flag_TempIntXH_Baja) {
    Nro_Alarma = 3;
  }
  else if (Flag_TempIntXH_Alta) {
    Nro_Alarma = 4;
  }
  else if (Flag_TempTriacs) {
    Nro_Alarma = 5;
  }
  else if (Flag_TempComp01) {
    Nro_Alarma = 6;
  }
  else if (Flag_CaudT) {
    Nro_Alarma = 7;
  }
  else if (Flag_CaudH) {
    Nro_Alarma = 8;
  }
  else if (Flag_PresHI) {
    Nro_Alarma = 9;
  }
  else if (Flag_PresLOW) {
    Nro_Alarma = 10;
  }
  else if (Flag_Corriente) {
    Nro_Alarma = 12;
  }
  else if (Flag_Temp_Caldera) {
    Nro_Alarma = 13;
  }
  else if (Flag_Alarma_Trif) {
    // Nro_Alarma = 14;
  }
  else if (Flag_Temp_Adm) {
    Nro_Alarma = 15;
  }
  else if (Flag_Aporte_E) {
    Nro_Alarma = 16;
  }
  else if (Flag_RetornoLiq) {
    Nro_Alarma = 17;
  }
  else if (Flag_Temp_Descarga) {
    Nro_Alarma = 18;
  }

  if (Nro_Alarma != 0) {
    Flag_Alarma_General = true;
    EEPROM.write(Alarma_Address, Nro_Alarma);
  }
  return Nro_Alarma;
}

void ResetFlags() // Luego de ocurrida una alarma y revisada por parte del usuario, esta funcion resetea los flagas y contadores a cero
{
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
  Flag_Corriente = false;
  Flag_Temp_Caldera = false;
  Flag_Alarma_Trif = false;
  Flag_Temp_Adm = false;
  Flag_Aporte_E = false;
  Flag_RetornoLiq = false;
  Flag_Temp_Des = false;
  Flag_TempComp01 = false;
  Flag_Temp_Descarga = false;

  Cont_Temp_intX_T = 0;
  Cont_Temp_intX_T2 = 0;
  Cont_Temp_intX_H = 0;
  Cont_Temp_intX_H2 = 0;
  Cont_Temp_Comp_01 = 0;
  Cont_Caud_T = 0;
  Cont_Caud_H = 0;
  Cont_Press_HI = 0;
  Cont_Press_LOW = 0;
  Cont_Temp_Des = 0;
}

void checkWifi() {
  Flag_Wifi = false;
  if (Flag_ESP) {
    Serial3.println("AT+CWJAP?"); // Consulta el estado del ESP8266, si está conectado a una red o no
    delay(100);
    if (Serial3.find(':')) // Si lo está, detiene el modo Smart y queda listo para funcionar
    {
      Serial.println(F("Conectado a red"));
      Serial3.println("AT+CIFSR");  // Consulta la IP del ESP8266
  
      if (Serial3.available()){
        if (Serial3.find("STAIP,\"")) {
          local_ip = Serial3.readString();
          local_ip = local_ip.substring(0,local_ip.indexOf("\""));
         }
      }

      Serial.println("Ip is:" + local_ip);

      Flag_Wifi = true;
    }
    else {
      Serial.print(F("No Conectado a red"));
    }
  }
}

void checkESP() {
  Serial3.println("AT");
  delay(1000);
  Flag_ESP = Serial3.find("OK");
}

void setupDigitalInputs() {
  pinMode(DI_Marcha_on, INPUT);
  pinMode(DI_Pres_HI, INPUT);
  pinMode(DI_Pres_LOW, INPUT);
  pinMode(DI_Caud_T, INPUT);
  pinMode(DI_Caud_H, INPUT);
}

void setupDigitalOuputs() {

  pinMode(DO_Comp_01, OUTPUT);
  pinMode(DO_Val1, OUTPUT);
  pinMode(DO_Bombas, OUTPUT);
  pinMode(DO_Calentador, OUTPUT);
  pinMode(DO_Val2, OUTPUT);

  pinMode(DO_Triac_01, OUTPUT);
  pinMode(DO_Buzzer, OUTPUT);
  // pinMode(DO_Contraste, OUTPUT);

  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(ED_ENABLE, OUTPUT);
}


void initializeDigitalOuputs() {
  digitalWrite(ED_ENABLE, LOW); // Inicialización de salidas
  digitalWrite(DO_Comp_01, LOW);
  digitalWrite(DO_Val1, LOW); // Inicia con la valvula de loza encendida
  digitalWrite(DO_Val2, LOW);
  digitalWrite(DO_Bombas, LOW);
  digitalWrite(DO_Calentador, LOW);
  // digitalWrite(DO_Aux, LOW);
  digitalWrite(DO_Triac_01, LOW);
  digitalWrite(DO_Buzzer, LOW);
}
