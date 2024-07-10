#include "stateMachine.h"

const float MAX_TEMP_OUT_H_ACS = 50.0;

unsigned long E1_a_E2 = 60000; // 1 minuto para pasar de E1 a E2
unsigned long E2_a_E3 = 15000;  // 15 segundos para pasar de E2 a E3

void initializeStateMachine() {
  Estado_Maquina = 0;
  Activacion_Comp = 0;
  dont_stuck_pumps_activation = 0;
  dont_stuck_pumps = 0;
  Valor_DO_VACS = LOW;
  Valor_DO_V4V = LOW;
  modoFrio = false;
  heating_off = false;

}


void stateMachine0() { // Estado inicial del sistema, tanto el compresor como las bombas de circulación están apagados
  if (Estado_Maquina == 0) {
    Flag_Marcha_ON = false;
    Valor_DO_Comp_01 = LOW;
    Activacion_Comp = 0;
    Valor_DO_Bombas = LOW;
    PumpStart = 0;
    dont_stuck_pumps = millis();

    Valor_DO_VACS = LOW; // paso a losa radiante
    EsperaValv = millis();

    Valor_DO_V4V = modoFrio ? LOW /* modo frio*/ : HIGH /* modo calor*/;

    if (!heating_off)
      Estado_Maquina = 1;

  }
}

void stateMachine1() {                          // Aquí se espera la señal de Marcha_ON para iniciar la operacion del sistema
  if (Estado_Maquina == 1) // Aquí se espera la señal de Marcha_ON para iniciar la operacion del sistema
  {                        // Espera de Marcha_ON
    if (heating_off) {
      Estado_Maquina = 0;
      return;
    }

    if ((millis() - dont_stuck_pumps) > 86400000) // rutina para activar las bombas una vez por dia durante 10 segundos, para evitar daños por inactividad (86400000)
    {
      Valor_DO_Bombas = HIGH;
      digitalWrite(DO_Buzzer, HIGH);
      dont_stuck_pumps_activation = millis();
    }
    if ((millis() - dont_stuck_pumps_activation) > 10000) {
      Valor_DO_Bombas = LOW;
      dont_stuck_pumps = millis();
      digitalWrite(DO_Buzzer, LOW);
    }

    if ((millis() - EsperaValv) > 15000) {
      if GENERATE_ACS // Control de ACS, modo calor
      {
        Valor_DO_VACS = HIGH; // la generacion de ACS requiere que la valv 4v este activa
        Valor_DO_V4V = HIGH;  // abre la valvula de 3v para calentar el agua
        EsperaValv = millis();
        Estado_Maquina = 7;   // Generacion ACS
        Ingreso_E7 = millis();
      }
    }

    if (senal_start && !Flag_Marcha_ON) //
    {
      Salto_E1 = millis();
      Flag_Marcha_ON = true;
    }

    if (senal_stop && Flag_Marcha_ON) //
    {
      Flag_Marcha_ON = false;
    }

    // E1_E2 no puede ser menor a 15000 ya que si no viola la condicion
    //     if ((millis() - EsperaValv) > 15000)
    if ((millis() - Salto_E1 > E1_a_E2) && senal_start) // Se espera un tiempo (2 min) para que abran las electrovalvulas de la loza radiante
    {
      Estado_Maquina = 2;
    }
  }
}

void stateMachine2() // Arranque Compresor y Bombas
{
  if (Estado_Maquina == 2) {

    if (heating_off || senal_stop) {
      Estado_Maquina = 0;
      return;
    }

    if ((millis() - EsperaValv) > 15000){
      Valor_DO_Bombas = HIGH;
      PumpStart = millis();   
      if (Valor_DO_Comp_01 == LOW) {
        if ((millis() - PumpStart) > 15000){
          Valor_DO_Comp_01 = HIGH;
          Activacion_Comp = millis();
        }
      }
    }

    if (millis() - Activacion_Comp > E2_a_E3) // Transcurrido un cierto tiempo, se avanza al siguiente estado
    {
      Estado_Maquina = 3;
      Ingreso_E3 = millis();
    }
  }
}

void stateMachine3() { // Este es el estado final del sistema, donde se controlan las condiciones de alarma
  if (Estado_Maquina == 3) {
    /* Si hay que generar ACS, pasamos por es estado 0 */
    if ((heating_off || senal_stop)
      || GENERATE_ACS) {
      Estado_Maquina = 0;
      return;
    }

    // Condiciones de Apagado del Compresor
    if ((millis() - Ingreso_E3) > 1000) {
      checkFlagsForAlarms();
    }

    takeRestControl();
  }
}

void stateMachine4() // Estado de Alarma
{

  if (Estado_Maquina == 4) {

    Valor_DO_Comp_01 = LOW;
    Activacion_Comp = 0;
    Valor_DO_Bombas = LOW;
    PumpStart = 0;
    digitalWrite(DO_Triac_01, LOW);

    if (!Alarma_Activa) {
      Alarmas();
      Alarma_Activa = true;
    }
    if (Nro_Alarma != 0) {
      if (!Flag_Buzzer) {
        Timer1.pwm(DO_Buzzer, 100, 1000000);
        Flag_Buzzer = true;
      }
    }
    wdt_reset();
  }
}

void stateMachine6() // Estado de descanso
{
  if (Estado_Maquina == 6) {
    Valor_DO_Comp_01 = LOW;
    Valor_DO_Bombas = LOW;
    PumpStart = 0;
    if ((millis() - Ingreso_Descanso > 400000)) // 6 min //una vez en el descanso, se espera antes de enviar el sistema al estado inicial
    {
      Estado_Maquina = 0;
    }
  } // Fin Estado 6
}

void stateMachine7() // Generacion ACS
{
  if (Estado_Maquina == 7) {
    if (heating_off) {
      Estado_Maquina = 0;
      return;
    }

    if (millis() - EsperaValv > 15000) {
      // solo prendo las bombas si paso el tiempo para abrir las valuvlas
      Valor_DO_Bombas = HIGH; // se mantiene HIGH hasta que se va a estado 0 (puede pasar por 71 u 8)
      PumpStart = millis();
    }

    if (millis() - Ingreso_E7 > 20000) {
      checkFlagsForAlarms();
      Flag_retardo_e7 = true;

      if ((millis() - Ingreso_E7) > 30000) {
        Valor_DO_Comp_01 = HIGH;
      }
    }

    // se le da energia al ACS de a saltos para evitar pasar de presion y temperatura el circuito de gas
    if (Temp_out_H > MAX_TEMP_OUT_H_ACS || Temp_Descarga > 80.0) {
      Estado_Maquina = 71;
      Ingreso_E71 = millis();
      Flag_retardo_e7 = false;
    }

    // termino el ciclo en modo calor y vuelve a 0
    if (((Temp_ACS >= SetP_ACS) && !modoFrio) || !Flag_ACS_EN) //
    {
      Estado_Maquina = 0;
      Flag_retardo_e7 = false;
    }
    // termino el ciclo en modo frio y vuelve a 0, previa espera de 5 minutos en el estado 8
    else if ((Temp_ACS >= SetP_ACS) && modoFrio ) {
      Periodo_Fin_ACS = millis();
      Estado_Maquina = 8;
      Flag_retardo_e7 = false;
    }
  }
}

void stateMachine71() // Generacion ACS: Estado con bombas andando y compresor apagado
{
  if (Estado_Maquina == 71) {
    if (heating_off) {
      Estado_Maquina = 0;
      return;
    }

    Valor_DO_Comp_01 = LOW;

    /*
    Ahora lo hace por tiempo, pero:
    Debe volver cuando se haya transferido la temperatura
    y esten casi igualadas, o con un gap minimo
    en ese momento si no se alcanzo la temperatura deseada,
    se debe volver a 7 para prender el compresor
    */
    if ((Temp_ACS > SetP_ACS) || (Temp_ACS > (Temp_out_H - GAP_ACS))
      || ((millis() - Ingreso_E71) > 90000) || !Flag_ACS_EN) {
      Estado_Maquina = 7;
      Ingreso_E7 = millis();
    }
  } // FIn Estado 71
}

void stateMachine8() // Estado de transicion al finalizar la generacion de ACS y el sistema esta en modo frio, evita circular agua muy caliente al circuito frio
{
  if (Estado_Maquina == 8) {
    Valor_DO_Comp_01 = LOW;
    if ((millis() - Periodo_Fin_ACS) > 30000) {
      Estado_Maquina = 0;
    }
  }
}
