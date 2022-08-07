#include "thingspeak.h"

#define API_KEY_KUME = "UML8C4KGN6IVTJE7" //key para enviar datos a Thingspeak, se obtiene de la pagina web
#define API_KEY_DR "230X8WDK4WACGI95" //key para enviar datos a Thingspeak, se obtiene de la pagina web

String S_Temp_in_T; //Variables auxiliares para el envío de datos a Thingspeak
String S_Temp_out_T;
String S_Temp_in_H;
String S_Temp_out_H;
String S_Temp_Comp_01;
String S_A_RMS;
String S_Caud_T;
String S_Caud_H;
String S_Temp_Descarga;
String S_Temp_Admision;
String S_TI;
String S_Temp_intX_T;
String S_Temp_intX_H;
String S_Sensor_Pres_PSI;
String S_Temp_ACS;


void sendToThingSpeak(String api_key) {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial3.println(cmd);
  delay(2000);
  if (Serial3.find("Error")) {
    return;
  }

  cmd = "GET /update?api_key=" + api_key;
  cmd += "&field1=";
  cmd += S_Temp_out_H;
  cmd += "&field2=";
  cmd += S_Temp_in_H;
  cmd += "&field3=";
  cmd += S_Temp_out_T;
  cmd += "&field4=";
  cmd += S_Temp_in_T;
  cmd += "&field5=";
  cmd += String(Caud_H);
  cmd += "&field6=";
  cmd += String(Caud_T);
  cmd += "&field7=";
  cmd += S_Temp_Admision;
  cmd += "&field8=";
  cmd += S_Temp_ACS;
  cmd += "\r\n\r\n\r\n";
  Serial.println(cmd);
  Serial3.print("AT+CIPSEND=");
  Serial3.println(cmd.length());
  if (Serial3.find('>')) {
    Serial3.print(cmd);
  }
  delay(250);
  Serial3.print("AT+CIPCLOSE");

}


void convertValues() {
  char buffer[200]; // Funcion para convertir un dato tipo float en un String
  if (Temp_in_T >= 10.0) {
    S_Temp_in_T = dtostrf(Temp_in_T, -4, 1, buffer); //The format is: dtostrf(floatvar, StringLengthIncDecimalPoint, numVarsAfterDecimal, charbuf);
  }
  else
    S_Temp_in_T = dtostrf(Temp_in_T, -3, 1, buffer);

  if (Temp_out_T >= 10.0) {
    S_Temp_out_T = dtostrf(Temp_out_T, -4, 1, buffer);
  }
  else
    S_Temp_out_T = dtostrf(Temp_out_T, -3, 1, buffer);

  if (Temp_in_H >= 10.0) {
    S_Temp_in_H = dtostrf(Temp_in_H, -4, 1, buffer); //The format is: dtostrf(floatvar, StringLengthIncDecimalPoint, numVarsAfterDecimal, charbuf);
  }
  else
    S_Temp_in_H = dtostrf(Temp_in_H, -3, 1, buffer);

  if (Temp_out_H >= 10.0) {
    S_Temp_out_H = dtostrf(Temp_out_H, -4, 1, buffer); //The format is: dtostrf(floatvar, StringLengthIncDecimalPoint, numVarsAfterDecimal, charbuf);
  }
  else
    S_Temp_out_H = dtostrf(Temp_out_H, -3, 1, buffer);

  if (Temp_Admision >= 10.0) {
    S_Temp_Admision = dtostrf(Temp_Admision, -4, 1, buffer); //The format is: dtostrf(floatvar, StringLengthIncDecimalPoint, numVarsAfterDecimal, charbuf);
  }
  else
    S_Temp_Admision = dtostrf(Temp_Admision, -3, 1, buffer);

  if (Temp_ACS >= 10.0) {
    S_Temp_ACS = dtostrf(Temp_ACS, -4, 1, buffer); //The format is: dtostrf(floatvar, StringLengthIncDecimalPoint, numVarsAfterDecimal, charbuf);
  }
  else
    S_Temp_ACS = dtostrf(Temp_ACS, -3, 1, buffer);
}

void ThingSUpdate() //Funcion que envia la informacion a Thingspeak
{
  convertValues();

  //  sendToThingSpeak(api_key_kume);
  sendToThingSpeak(API_KEY_DR);

} //Fin funcion Thingspeak Update
