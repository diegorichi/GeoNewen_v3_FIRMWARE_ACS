#include "machine_control.h"

void FrioCalor() // Función de cambio de Modo de Funcionamiento  (Bromberg: modo frio = valvula de 4 vias APAGADA)
{

  Valor_DO_Comp_01 = LOW; // Primer paso es apagar el compresor
  Estado_Comp = 0;
  Activacion_Comp = 0;
  lcd.clear();
  lcd.write(F("POR FAVOR, ESPERE"));
  lcd.setCursor(0, 2);
  lcd.write(F("CAMBIANDO MODO..."));
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

void refreshDataToShow() {
  if (millis() - Periodo_Refresco > 1000) // Refresco de valores
  {
    calculateValuesToShow();

    lcdRefreshValues();

    //menuActivo->refresh();

    Periodo_Refresco = millis(); // El período de refresco es a los fines de que la información mostrada no esté constanmente cambiando y la visualización sea más adecuada
  }
}

void processStartStopSignal() {
  /*
Si modo calor:
  di marcha on == HIGH -> arrancar
  di marcha on == LOW -> parar
Si modo frio:
  di marcha on == LOW -> arrancar
  di marcha on == HIGH -> parar

digitalRead(DI_Marcha_on) == HIGH
modoFrioCalor

*/

  senal_start = ((digitalRead(DI_Marcha_on) == HIGH) && !ModoFrioCalor) || ((digitalRead(DI_Marcha_on) == LOW) && ModoFrioCalor);

  senal_stop = ((digitalRead(DI_Marcha_on) == LOW) && !ModoFrioCalor) || ((digitalRead(DI_Marcha_on) == HIGH) && ModoFrioCalor);
}