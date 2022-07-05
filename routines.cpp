#include "routines.h"

void FrioCalor() // Función de cambio de Modo de Funcionamiento  (Bromberg: modo frio = valvula de 4 vias APAGADA)
{

  Valor_DO_Comp_01 = LOW; // Primer paso es apagar el compresor
  Estado_Comp = 0;
  Activacion_Comp = 0;
  // Modo_Funcionamiento = true;
  lcd.clear();
  lcd.write("POR FAVOR, ESPERE   ");
  lcd.setCursor(0, 2);
  lcd.write("CAMBIANDO MODO...   ");
  for (unsigned long i = 0; i < 3000000; i++) // Se espera un cierto tiempo antes de activar la válvula de 4 vías
  {
    wdt_reset();
  }
  if (ModoFrioCalor) // Se activa la válvula
  {
    Valor_DO_V4V = HIGH;
    ModoFrioCalor = false;
  }
  else
  {
    Valor_DO_V4V = LOW;
    ModoFrioCalor = true;
  }

  lcd.clear(); // Se espera un cierto tiempo antes de reactivar el compresor
  lcd.write("POR FAVOR, ESPERE   ");
  lcd.setCursor(0, 2);
  lcd.write("CAMBIANDO MODO...   ");
  for (unsigned long i = 0; i < 200000; i++)
  {
    wdt_reset();
  }
  MenuActual = 20;
  MenuDosCero();
  // Estado_Maquina = 0;  //Al cambiar el modo de funcionamiento se resetea el sistema
}

int Alarmas() // Función de identificación de Alarma Activa
{
  if (Flag_TempIntXT_Baja == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 1;
  }
  else if (Flag_TempIntXT_Alta == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 2;
  }
  else if (Flag_TempIntXH_Baja == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 3;
  }
  else if (Flag_TempIntXH_Alta == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 4;
  }
  else if (Flag_TempTriacs == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 5;
  }
  else if (Flag_TempComp01 == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 6;
  }
  else if (Flag_CaudT == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 7;
  }
  else if (Flag_CaudH == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 8;
  }
  else if (Flag_PresHI == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 9;
  }
  else if (Flag_PresLOW == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 10;
  }
  else if (Flag_Corriente == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 12;
  }
  else if (Flag_Temp_Caldera == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 13;
  }
  else if (Flag_Alarma_Trif == true)
  {
    Flag_Alarma_General = true;
    // Nro_Alarma = 14;
  }
  else if (Flag_Temp_Adm == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 15;
  }
  else if (Flag_Aporte_E == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 16;
  }
  else if (Flag_RetornoLiq == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 17;
  }
  else if (Flag_Temp_Descarga == true)
  {
    Flag_Alarma_General = true;
    Nro_Alarma = 18;
  }
  else
    Nro_Alarma = 0;

  if (Nro_Alarma != 0)
  {
    Alarma_Eeprom = Nro_Alarma;
    EEPROM.write(Alarma_Address, Alarma_Eeprom);
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

void checkWifi()
{
  Flag_Wifi = false;
  if (Flag_ESP){
    Serial3.println("AT+CWJAP?"); // Consulta el estado del ESP8266, si está conectado a una red o no
    delay(100);
    if (Serial3.find(":")) // Si lo está, detiene el modo Smart y queda listo para funcionar
    {
      Serial.print("Conectado a red");
      Serial3.println("AT+CIFSR");  // Consulta la IP del ESP8266

      local_ip = "";
      // Loop through all the data returned
      while (Serial3.available())
      {
        delay(200); // wait for all characters arrive

        char c = Serial3.read(); // read the next character.
        local_ip.concat(c);
      }
      Serial.print("ip is:" + local_ip);
      Flag_Wifi = true;
    }
    else
    {
      Serial.print("No Conectado a red");
    }
  }
}


void checkESP(){
  Serial3.println("AT");
  delay(100);
  Flag_ESP = Serial3.find("OK");
}