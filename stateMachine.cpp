#include "stateMachine.h"

void stateMachine0() { // Estado inicial del sistema, tanto el compresor como las bombas de circulación están apagados
  if (Estado_Maquina == 0) {
    Flag_Marcha_ON = false;
    Valor_DO_Comp_01 = LOW;
    Estado_Comp = 0;
    Activacion_Comp = 0;
    Valor_DO_Bombas = LOW;
    Periodo_Bombas = millis();

    Valor_DO_VACS = LOW; // paso a losa radiante

    if (ModoFrioCalor) {
      Valor_DO_V4V = LOW; // modo frio
    } else {
      Valor_DO_V4V = HIGH; // modo calor
    }

    EsperaValv = millis();
    Estado_Maquina = 1;
    Flag_TempComp01 = false;
  }
}

void stateMachine1() {    // Aquí se espera la señal de Marcha_ON para iniciar la operacion del sistema
  if (Estado_Maquina == 1) // Aquí se espera la señal de Marcha_ON para iniciar la operacion del sistema
  {                        // Espera de Marcha_ON

    if ((millis() - Periodo_Bombas) > 86400000) // rutina para activar las bombas una vez por dia durante 10 segundos, para evitar daños por inactividad (86400000)
    {
      Valor_DO_Bombas = HIGH;
      digitalWrite(DO_Buzzer, HIGH);
      Activacion_Bombas = millis();
    }
    if ((millis() - Activacion_Bombas) > 10000)
    {
      Valor_DO_Bombas = LOW;
      Periodo_Bombas = millis();
      digitalWrite(DO_Buzzer, LOW);
    }

    if ((millis() - EsperaValv) > 15000)
    {
      if (Temp_ACS < (SetP_ACS - GAP_ACS) && Flag_ACS_EN) // Control de ACS, modo calor
      {
        Valor_DO_V4V = HIGH;  // abre la valvula de 3v para calentar el agua
        Valor_DO_VACS = HIGH; // la generacion de ACS requiere que la valv 4v este activa
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
    if ((millis() - Salto_E1 > E1_a_E2) && !Modo_Funcionamiento && senal_start) // Se espera un tiempo (2 min) para que abran las electrovalvulas de la loza radiante
    {
      Estado_Maquina = 2;
    }
  }
}

void stateMachine2() // Arranque Compresor y Bombas
{                          
  if (Estado_Maquina == 2)
  {
    Valor_DO_Bombas = HIGH;

    if (Estado_Comp == 0)
    {
      Valor_DO_Comp_01 = HIGH;
      Estado_Comp = 1;
      Activacion_Comp = millis();
    }

    if (senal_stop)
    {
      Estado_Maquina = 0;
    }

    if (Modo_Funcionamiento)
    {
      Estado_Maquina = 0;
    }

    if (millis() - Activacion_Comp > E2_a_E3) // Transcurrido un cierto tiempo (10 seg), se avanza al siguiente estado
    {
      Estado_Maquina = 3;
      Ingreso_E3 = millis();
    }
  }
}

void stateMachine3() { // Este es el estado final del sistema, donde se controlan las condiciones de alarma
  if (Estado_Maquina == 3)
  {
    if (Temp_ACS < (SetP_ACS - GAP_ACS) && Flag_ACS_EN) // Si estamos en el estado 3 y hay que generar ACS, por razones de seguridad pasamos siempre por es estado 0
    {
      Estado_Maquina = 0;
    }

    if (senal_stop) {
      Estado_Maquina = 0;
    }

    if (Modo_Funcionamiento) {
      Estado_Maquina = 0;
    }

    // Condiciones de Apagado del Compresor
    if ((millis() - Ingreso_E3) > 1000) {
      checkFlagsForAlarms();
    }

    {                             //   ###########    Condiciones de descanso generales      ##############
      if ((Temp_out_H > 50.0)     // Condicion para ir a Descanso
          || (Temp_out_H < 10.0)  // Condicion para ir a Descanso
          || (Temp_out_T > 40.0)  // Condicion para ir a Descanso
          || (Temp_out_T < -6)    // Condicion para ir a Descanso
          || (Temp_Admision < -7) // Condicion para ir a Descanso
      )
      {
        Estado_Maquina = 6;
        Ingreso_Descanso = millis();
      }
      //   #################    FIN Condiciones de descanso generales      #####################################
    }
  }
}

void stateMachine4() // Estado de Alarma
{ 

  if (Estado_Maquina == 4)
  {

    Valor_DO_Comp_01 = LOW;
    Estado_Comp = 0;
    Activacion_Comp = 0;
    Valor_DO_Bombas = LOW;
    digitalWrite(DO_Triac_01, LOW);

    if (!Alarma_Activa) {
      Alarmas();
      Alarma_Activa = true;
    }
    if (Nro_Alarma != 0) {
      Flag_Alarma_General = true;
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
  if (Estado_Maquina == 6) 
  {
    Valor_DO_Comp_01 = LOW;
    Valor_DO_Bombas = LOW;
    if ((millis() - Ingreso_Descanso > 400000)) // 6 min //una vez en el descanso, se espera una hora antes de enviar el sistema al estado inicial
    {
      Estado_Maquina = 0;
    }
  } // Fin Estado 6
}

void stateMachine7() // Generacion ACS
{                          
  if (Estado_Maquina == 7)
  {
    Valor_DO_Bombas = HIGH; // se mantiene HIGH hasta que se va a estado 0 (puede pasar por 71 u 8)

    if (millis() - Ingreso_E7 > 5000)
    {
      checkFlagsForAlarms();
      Flag_retardo_e7 = true;

      if (Temp_ACSacu > SetP_ACS)
      { // si se alcanzan la temperatura deseada se deja de calentar
        Estado_Maquina = 0;
        Flag_retardo_e7 = false;
      }

      if ((millis() - Ingreso_E7) > 30000)
      {
        Valor_DO_Comp_01 = HIGH;
        Estado_Comp = 1;
      }
    }
    if (Temp_out_H > 50.0 || Temp_Descarga > 80.0) // se le da energia al ACS de a saltos para evitar pasar de presion y temperatura el circuito de gas
    {
      Estado_Maquina = 71;
      Ingreso_E71 = millis();
      Flag_retardo_e7 = false;
    }

    if ((Temp_ACS >= SetP_ACS) && !ModoFrioCalor || !Flag_ACS_EN) //
    {
      Estado_Maquina = 0;
      Flag_retardo_e7 = false; // termino el ciclo en modo calor y vuelve a 0
    }
    else if ((Temp_ACS >= SetP_ACS) && ModoFrioCalor && (Estado_Comp == 1)) // termino el ciclo en modo frio y vuelve a 0, previa espera de 5 minutos en el estado 8
    {
      Periodo_Fin_ACS = millis();
      Estado_Maquina = 8;
      Flag_retardo_e7 = false;
    }
  }
}

void stateMachine71() // Generacion ACS: Estado con bombas andando y compresor apagado
{
  if (Estado_Maquina == 71)
  {
    Valor_DO_Comp_01 = LOW;
    Estado_Comp = 0;
    if ((millis() - Ingreso_E71) > 90000 || !Flag_ACS_EN) // espero 1.5 minuto antes de volver al estado 7
    {
      Estado_Maquina = 7;
      Ingreso_E7 = millis();
    }
  } // FIn Estado 71
}

void stateMachine8()// Estado de transicion al finalizar la generacion de ACS y el sistema esta en modo frio, evita circular agua muy caliente al circuito frio
{
  if (Estado_Maquina == 8) {
    Valor_DO_Comp_01 = LOW;
    Estado_Comp = 0;
    if ((millis() - Periodo_Fin_ACS) > 30000) {
      Estado_Maquina = 0;
    }
  }
}
