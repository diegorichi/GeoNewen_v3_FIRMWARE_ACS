#include "connectivity.h"

void checkWifi() {
  Flag_Wifi = false;
  if (Flag_ESP) {
    Serial3.println("AT+CWJAP?"); // Consulta el estado del ESP8266, si está conectado a una red o no
    delay(100);
    if (Serial3.find(':')) // Si lo está, detiene el modo Smart y queda listo para funcionar
    {
      Serial.println(F("Conectado a red"));
      Serial3.println("AT+CIFSR");  // Consulta la IP del ESP8266
      if (Serial3.available()) {
        if (Serial3.find("STAIP,\"")) {
          local_ip = Serial3.readString();
          local_ip = local_ip.substring(0, local_ip.indexOf("\""));
        }
      }

      Serial.println("Ip:" + local_ip);

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

void configWifi() {
  wdt_disable();
  if (Serial3.available()) // Lo que entra por WIFI -> Serial
  {
    w = Serial3.read();
    Serial.print(w); // Vamos montando un String con lo que entra
    W = W + w;
  }

  if (Serial.available()) // Lo que entra por Serial -> WIFI
  {
    char s = Serial.read();
    Serial3.print(s);
  }

  if (w == '\n') // Sin han pulsado intro
  {

    RefreshMenuDosSieteUno();

    if (W.indexOf("connected") > 0) // Si P13 esta incluido en el string
    {
      Serial3.println("AT+CWSTOPSMART");
      delay(500);
      checkWifi(); // cambia el Flag_Wifi
  
      Estado_ConfigWIFI = Flag_Wifi ? 0 : 1; // Si el WIFI esta conectado, se sale del modo de configuracion WIFI
    }

    W = "";
    w = ' ';
  } // reseteamos la variable para permitir que el resto del codigo se ejecute, y reseteamos las variables auxiliares

  wdt_enable(WDTO_8S);
}

void checkEspWifiConnected() { // Verificacion de conexion a Wifi
  if ((millis() - RefrescoWifi) > 30000) 
  {
    checkESP();
    checkWifi();
    RefrescoWifi = millis();
  }
}

void sendAndReceiveDataCloud(){ // Envio de datos a ThingSpeak
  if (((millis() - Periodo_Refresco_Wifi) > 60000) && Flag_Wifi) 
  {
    wdt_reset();
    ThingSUpdate();
    Periodo_Refresco_Wifi = millis();
  }
}

void setupConnectivity(){
  checkESP();

  if (Flag_ESP) {
    Serial3.println("AT+CWMODE=1"); // 1:Mode station 2:Mode AP 3:Mode BOTH
    delay(100);
    Serial3.println("AT+CWSTOPSMART");
    delay(100);
    Serial3.println("AT+CIPMUX=0"); // 1:multiple connection 0:single connection
    delay(100);
    Serial.println(F("ESP Inicializado"));
    delay(100);
    checkWifi();
  }
  else {
    Serial.println(F("Fallo en inicializacion de ESP"));
  }
}