#include "vars.h"
#include "keyboard.h"
#include "functionsLCDMenu.h"
#include "routines.h"
#include "kume_eeprom.h"
#include "thingspeak.h"

#define ONE_WIRE_BUS 22
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

/*******/
/*SETUP*/
/*******/

void setup()
{ // Inicializacion de I/O y variables generales

  wdt_disable();

  pinMode(DI_Teclado_Arriba, INPUT); // Definición de entradas y salidas
  pinMode(DI_Teclado_Abajo, INPUT);
  pinMode(DI_Teclado_Enter, INPUT);
  pinMode(DI_Teclado_Atras, INPUT);
  pinMode(InterruptPin, INPUT);

  pinMode(DI_Marcha_on, INPUT);
  pinMode(DI_Pres_HI, INPUT);
  pinMode(DI_Pres_LOW, INPUT);
  pinMode(DI_Caud_T, INPUT);
  pinMode(DI_Caud_H, INPUT);

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

  digitalWrite(ED_ENABLE, LOW); // Inicialización de salidas
  digitalWrite(DO_Comp_01, LOW);
  digitalWrite(DO_Val1, LOW); // Inicia con la valvula de loza encendida
  digitalWrite(DO_Val2, LOW);
  digitalWrite(DO_Bombas, LOW);
  digitalWrite(DO_Calentador, LOW);
  // digitalWrite(DO_Aux, LOW);
  digitalWrite(DO_Triac_01, LOW);
  digitalWrite(DO_Buzzer, LOW);

  attachInterrupt(1, AtencionTeclado, FALLING); // Asignación de Interrupciones (se define el número de la interrupción, no del pin; la rutina de interrupción y el modo de activación)
  attachInterrupt(4, Caudal1, FALLING);         // Pin 19
  attachInterrupt(5, Caudal2, FALLING);         // Pin 18

  lcdCreateSpecialChars();

  Caud_T = 0; // Inicialización de variables
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

  // Valor_Per_Esp_C = 20000;
  // Per_Esp_C = (Valor_Per_Esp_C/1000);

  Flag_TempIntXT_Baja = false; // inicializacion  de banderas
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

  delay(50); // Delay para que se establezca el display
  lcd.begin(20, 4);

  Serial.begin(115200);
  Serial3.begin(115200);

  checkESP();

  if (Flag_ESP)
  {
    Serial3.println("AT+CWMODE=1"); // 1:Mode station 2:Mode AP 3:Mode BOTH
    delay(100);
    Serial3.println("AT+CWSTOPSMART");
    delay(100);
    Serial3.println("AT+CIPMUX=0"); // 1:multiple connection 0:single connection
    delay(100);
    Serial.println("ESP Inicializado");
    delay(100);
    // Serial3.println("AT+CWJAP=\"Velazquez-UBNET\",\"hv45av25\"");
    // delay(10000);
    checkWifi();
  }
  else
  {
    Flag_Wifi = false;
    Serial.println("Fallo en inicializacion de ESP");
  }

  EEPROMLectura(); // Carga parametros guardados en la memoria EEPROM

  if (SetP_ACS < 30)
  {
    SetP_ACS = 30;
  }
  if (SetP_ACS > 48)
  {
    SetP_ACS = 48;
  }
  SetP_ACS_Edit = SetP_ACS;
  MenuActual = 0;
  MenuCero();

  wdt_enable(WDTO_8S);
}

/***************/
/*LAZO INFINITO*/
/***************/

void loop()
{

  if (Estado_ConfigWIFI == 0)
  {

    // CÁLCULO DE TEMPERATURAS, CAUDALES, EFICIENCIA TÉRMICA Y CONSUMO DE ENERGÍA
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

    {                                          // Determinación de Caudal
      Pulsos_Caud_H_Bis = Pulsos_Caud_H;       // Variable auxiliar para visualizar los pulsos contados directamente
      if ((millis() - Ventana_Caudal1) > 1000) // Se contabilizan los pulsos de los caudalímetros durante un segundo, y se calcula el caudal
      {
        detachInterrupt(4);
        Caud_H = ((60000.0 / (millis() - Ventana_Caudal1)) * Pulsos_Caud_H) * FCal; // Los cálculos resultan de la constante de pulsos/caudal indicados en la hoja de datos de los caudalímetros
        // Caud_H = Caud_H;                                                           //El cálculo está escalado al tamaño de la ventana de muestreo, que puede no ser exactamente de 1 segundo
        Ventana_Caudal1 = millis();
        Pulsos_Caud_H = 0;
        attachInterrupt(4, Caudal1, FALLING); // Las interrupciones se deshabilitan al principio del cálculo para no contabilizar pulsos de más, luego se reestablecen
      }
      Pulsos_Caud_T_Bis = Pulsos_Caud_T;
      if ((millis() - Ventana_Caudal2) > 1000)
      {
        detachInterrupt(5);
        Caud_T = ((60000.0 / (millis() - Ventana_Caudal2)) * Pulsos_Caud_T) * FCal;
        // Caud_T = Caud_T;
        Ventana_Caudal2 = millis();
        Pulsos_Caud_T = 0;
        attachInterrupt(5, Caudal2, FALLING);
      }
    }

    // Cálculo de Eficiencia Térmica

    // Ef_Termica_1 = (Temp_in_T - Temp_out_T) * Caud_H;  //recordar caudales estan cruzados
    // Ef_Termica_2 = (Temp_in_T - Temp_out_T) * Caud_T;

    // Cálculo de la Corriente AC

    // Voltage = getVPP();                       //La llamada a la función getVPP devuelve el valor pico a pico de la tensión muestreada
    // V_RMS = (Voltage / 2.0) * 0.707;          //Ese valor pico a pico se divide por 2 y se multiplica por 0.707 para obtener el valor eficaz (aproximado)
    // A_RMS = (V_RMS * 1000) / mVperAmp;        //En función del valor eficaz y del módulo empleado, se usa la constante de equivalencia correspondiente y se obtiene el valor de la corriente

    // Cálculo de la Potecia

    Potencia = 220 * TI * CosFI; // Valor estimativo

    if ((millis() - RefrescoWifi) > 30000) // Verificacion de conexion a Wifi
    {
      checkESP();
      checkWifi();
      RefrescoWifi = millis();
    }

    if ((millis() - Periodo_Refresco_Wifi) > 10 * 60 * 1000) // Envio de datos a ThingSpeak: Refresco cada 10 minutos
    {
      wdt_reset();
      ThingSUpdate();
      delay(1000);
      // Serial.println("ThingSUpdate ejecutada");
      Periodo_Refresco_Wifi = millis();
    }

    wdt_reset();

    // INFORMACIÓN A REFRESCAR (depende del menú en el que nos encontremos)
    if (millis() - Periodo_Refresco > 1000) // Refresco de valores
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

      Periodo_Refresco = millis(); // El período de refresco es a los fines de que la información mostrada no esté constanmente cambiando y la visualización sea más adecuada
    }

    //***************************
    // COMPROBACIONES DE SEGURIDAD (Aquí se determinan las posibles causas de alarmas
    //***************************

    // Control de Caudales
    {
      if ((Caud_Tacu < 100.0) && (digitalRead(DI_Marcha_on) == HIGH) && (Flag_Alarma_EN == true) && (Estado_Maquina == 3 || (Estado_Maquina == 7 && Flag_retardo_e7 == true))) // Se comprueba que el caudal no sea inferior a un cierto valor, para evitar daños a las bombas
      {
        Flag_CaudT = true;
      }

      if (Caud_Tacu > 100.0)
        Flag_CaudT = false;

      if (Caud_Hacu < 100.0 && (digitalRead(DI_Marcha_on) == HIGH) && (Flag_Alarma_EN == true) && (Estado_Maquina == 3 || (Estado_Maquina == 7 && Flag_retardo_e7 == true))) // Se comprueba que el caudal no sea inferior a un cierto valor, para evitar daños a las bombas
      {
        Flag_CaudH = true;
      }

      if (Caud_Hacu > 100.0)
        Flag_CaudH = false;
    }

    // Control de Temperaturas
    {
      if (Temp_comp_acu > 80.0) // Si la temperatura de operación del compresor es muy elevada o muy baja, se lo detiene para evitar daños
      {
        Cont_Temp_Comp_01++;
        if (Cont_Temp_Comp_01 > 3)
        {
          Flag_TempComp01 = true;
        }
      }
      else
        Cont_Temp_Comp_01 = 0;

      if (Temp_Descargaacu > 85.0)
      {
        Cont_Temp_Descarga++;
        if (Cont_Temp_Descarga > 3)
        {
          Flag_Temp_Descarga = true;
        }
      }
      else
        Cont_Temp_Descarga = 0;

      if (Temp_Admision < -4)
      {
        Flag_Temp_Adm = true;
      }
    }

    // Comprobación de Presiones
    {
      if (digitalRead(DI_Pres_HI) == LOW) // Si la presion de operación del compresor es muy elevada, se lo detiene para evitar daños
      {
        Cont_Press_HI++;
        if (Cont_Press_HI > 3)
        {
          Flag_PresHI = true;
        }
      }
      else
      {
        Cont_Press_HI = 0;
        Flag_PresHI = false;
      }

      if (digitalRead(DI_Pres_LOW) == LOW) // Si la presion de operación del compresor es muy elevada, se lo detiene para evitar daños
      {
        Cont_Press_LOW++;
        if (Cont_Press_LOW > 3)
        {
          Flag_PresLOW = true;
        }
      }
      else
      {
        Cont_Press_LOW = 0;
        Flag_PresLOW = false;
      }

      if (Flag_PresHI == true || Flag_PresLOW == true)
      {
        PressOK = false;
      }
      else
        PressOK = true;
    }

    // Control de calentamiento auxiliar con cartucho electrico.
    {
      if (((Temp_ACSacu >= (SetP_ACS + 7)) && Flag_ACS_EN) // Si la temp ACS alcanza el objetivo, apagamos el calentador
          || ((Temp_ACSacu < SetP_ACS) && Flag_ACS_EN)     // Si la temp es menor al seteo, lo apago porque estado = 7 -> generar acs
          || !Flag_ACS_EN)                                 // si apago generac ACS no hay delta t final.
      {
        flag_dtElectrico_final = false;
      }

      // Si la temperatura es 2 grados menor al objetivo, volvemos a prender el calendador
      //  pero solo si es mayor a la seteada, de manera tal que usamos el cartucho solo en el
      // ultimo tramo de ACS.
      if (Temp_ACSacu < (SetP_ACS + 5) && (Temp_ACSacu > SetP_ACS) && Flag_ACS_EN)
      {
        flag_dtElectrico_final = true;
      }

      // si acs elect apagado -> lo apagamos
      // si acs apagado -> lo apagamos
      if (Flag_ACS_EN_ELECT || (Flag_ACS_EN && flag_dtElectrico_final))
      {
        Valor_DO_Calentador = HIGH;
      }
      else
      {
        Valor_DO_Calentador = LOW;
      }
    }

    // TRANSICIÓN DE ESTADOS  //Definición del funcionamiento del equipo
    if (Estado_Maquina == 0) // Estado inicial del sistema, tanto el compresor como las bombas de circulación están apagados
    {
      descanso = 0;
      // if(Modo_Funcionamiento == false)  //false es modo Automatico
      //{
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
      //}else if(Modo_Funcionamiento == true)
      //      {Valor_DO_Comp_01 = SenalComp;
      //    }
    }

    if (Estado_Maquina == 1) // Aquí se espera la señal de Marcha_ON para iniciar la operacion del sistema
    {                        // Espera de Marcha_ON

      if (ModoFrioCalor == true)
        Valor_DO_V4V = LOW; // modo frio
      if (ModoFrioCalor == false)
        Valor_DO_V4V = HIGH; // modo calor

      if ((millis() - Periodo_Bombas) > 86400000) // rutina para activar las bombas una vez por dia durante 10 segundos, para evitar daños por inactividad (86400000)
      {
        Valor_DO_Bombas = HIGH;
        // Valor_DO_Bomba_H = HIGH;
        digitalWrite(DO_Buzzer, HIGH);
        Periodo_Bombas = millis();
        Activacion_Bombas = millis();
      }
      if ((millis() - Activacion_Bombas) > 10000)
      {
        Valor_DO_Bombas = LOW;
        // Valor_DO_Bomba_H = LOW;
        digitalWrite(DO_Buzzer, LOW);
      }

      if (Estado_Comp == 0 && Flag_ACS_EN == false) // Activacion de valvula de 4 vias
      {
        if (ModoFrioCalor == true) // Si estamos en modo FRIO, desactiva la valvula de 4 vias
        {
          Valor_DO_VACS = LOW;
        }
        else
          Valor_DO_VACS = HIGH; // si no, la valvula de 4v debe estar activa
      }

      if ((millis() - EsperaValv) > 15000)
      {
        if (Temp_ACS < (SetP_ACS - 2) && Flag_ACS_EN == true) // Control de ACS, modo calor && Cal == true
        {
          Valor_DO_V4V = HIGH; // si la temp ACS es baja, abre la valvula de 3v para calentar el agua
          // Cal_ACS = true;
          // Cal = false;
          Valor_DO_Bombas = HIGH;
          Valor_DO_VACS = HIGH; // la generacion de ACS requiere que la valv 4v este activa
          Nro_Alarma = 0;
          Estado_Maquina = 7; // Generacion ACS
          Ingreso_E7 = millis();
        }
      }

      if (Flag_ACS_EN == false) // si el ACS no esta habilitado, se debe abrir la valvula que deja pasar a loza
      {
        Valor_DO_VACS = LOW;
      }

      if (digitalRead(DI_Marcha_on) == HIGH && Flag_Marcha_ON == false) //
      {
        Salto_E1 = millis();
        Flag_Marcha_ON = true;
      }

      if ((digitalRead(DI_Marcha_on) == LOW && Flag_Marcha_ON == true)) //
      {                                                                 // Estado_Maquina = 0;
        Flag_Marcha_ON = false;
      }

      if ((millis() - Salto_E1 > E1_a_E2) && Modo_Funcionamiento == false && digitalRead(DI_Marcha_on) == HIGH) // Se espera un tiempo (2 min) para que abran las electrovalvulas de la loza radiante
      {
        Estado_Maquina = 2;
      }
    }

    if (Estado_Maquina == 2) // Arranque Compresor y Bombas
    {

      /*if (Flag_TempComp01 == true)      //Si la temperatura del compresor no fuera correcta, se produce una alarma y se detiene el proceso
        {
        Estado_Maquina = 4;
        MenuCuatroCero();
        MenuActual = 40;
        }*/

      // Valor_DO_Bomba_T = HIGH;
      // Valor_DO_Bomba_H = HIGH;
      Valor_DO_Bombas = HIGH;
      // Valor_DO_Aux = HIGH;
      Nro_Alarma = 0;
      if (Estado_Comp == 0)
      {
        Valor_DO_Comp_01 = HIGH;
        Estado_Comp = 1;
        // Valor_DO_Aux = LOW;   //apago calentador
        Activacion_Comp = millis();
      }

      if (digitalRead(DI_Marcha_on) == LOW)
      {
        Estado_Maquina = 0;
        Flag_Marcha_ON = false;
      }

      if (millis() - Activacion_Comp > 10000) // Transcurrido un cierto tiempo (30 seg), se avanza al siguiente estado
      {
        Estado_Maquina = 3;
        Ingreso_E3 = millis();
      }

      /*if (millis() - Activacion_Comp > 2500)
        { if (Flag_Corriente == true )
        {
          Estado_Maquina = 4;
          MenuCuatroCero();
          MenuActual = 40;
        }
        }*/

      /*if(digitalRead(DI_Alarma_Trif) == LOW)   //!Solo equipos trifasicos
        {
         Estado_Maquina = 4;
         MenuCuatroCero();
         MenuActual = 40;
         Flag_Alarma_Trif = true;
        }*/

      /*if (Flag_TempComp01 == true)      //Si la temperatura del compresor no fuera correcta, se produce una alarma y se detiene el proceso
        {
        Estado_Maquina = 4;
        MenuCuatroCero();
        MenuActual = 40;
        }*/

      if (Modo_Funcionamiento == true)
      {
        Estado_Maquina = 0;
      }
    }

    if (Estado_Maquina == 3) // Este es el estado final del sistema, donde se controlan las condiciones de alarma
    {

      if (Temp_ACS < (SetP_ACS - 2) && Flag_ACS_EN) // Si estamos en el estado 3 y hay que generar ACS, por razones de seguridad pasamos siempre por es estado 0
      {
        Estado_Maquina = 0;
      }

      // Valor_DO_Bomba_T = HIGH;
      // Valor_DO_Bomba_H = HIGH;
      // Valor_DO_V4V = LOW;
      Valor_DO_VACS = LOW;

      if (digitalRead(DI_Marcha_on) == LOW || Flag_Caldera)
      {
        Estado_Maquina = 0;
        Flag_Marcha_ON = false;
      }

      // Condiciones de Apagado del Compresor
      if ((millis() - Ingreso_E3) > 1000)
      {
        if (Flag_TempComp01 || PressOK || Flag_CaudT || Flag_CaudH || Flag_Temp_Adm || Flag_Temp_Descarga) //|| Flag_Aporte_E == true || Flag_Temp_Des == true
        {
          Estado_Maquina = 4;
          MenuCuatroCero();
          MenuActual = 40;
        }
      }

      //   ##########################################################################    Condiciones de descanso generales      #####################################

      if (Temp_out_H > 45.0) // Condicion para ir a Descanso
      {
        Estado_Maquina = 6;
        Ingreso_Descanso = millis();
      }
      if (Temp_out_H < 10.0) // Condicion para ir a Descanso
      {
        Estado_Maquina = 6;
        Ingreso_Descanso = millis();
      }
      if (Temp_out_T > 40.0) // Condicion para ir a Descanso
      {
        Estado_Maquina = 6;
        Ingreso_Descanso = millis();
      }
      if (Temp_out_T < 0) // Condicion para ir a Descanso
      {
        Estado_Maquina = 6;
        Ingreso_Descanso = millis();
      }
      if (Temp_Admision < -4) // Condicion para ir a Descanso
      {
        Estado_Maquina = 6;
        Ingreso_Descanso = millis();
      }

      //   #################    FIN Condiciones de descanso generales      #####################################

      if (Temp_out_Hacu > 50.0)
      {
        Estado_Maquina = 8;
        Ingreso_Descanso = millis();
      }

      if (Modo_Funcionamiento == true)
      {
        Estado_Maquina = 0;
      }
    }

    if (Estado_Maquina == 4) // Estado de Alarma
    {
      if (Estado_Comp == 1)
      {
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

      if (Alarma_Activa == false)
      {
        Alarmas();
        Alarma_Activa = true;
      }
      if (Nro_Alarma != 0)
      {
        Flag_Alarma_General = true;
        if (Flag_Buzzer == false)
        {
          Timer1.pwm(DO_Buzzer, 100, 1000000);
          Flag_Buzzer = true;
        }
      }
      /*if(Flag_Alarma_General == false)
        {Estado_Maquina = 0;
        }*/
      wdt_reset();
    }

    if (Estado_Maquina == 6) // Estado de descanso
    {
      Valor_DO_Comp_01 = LOW;
      Valor_DO_Bombas = LOW;
      Estado_Comp = 0;
      if ((millis() - Ingreso_Descanso > 400000)) // 6 min //una vez en el descanso, se espera una hora antes de enviar el sistema al estado inicial
      {
        Estado_Maquina = 0;
      }
    } // Fin Estado 6

    if (Estado_Maquina == 7) // Generacion ACS
    {
      if (millis() - Ingreso_E7 > 5000)
      {
        Flag_retardo_e7 = true;
        if (Flag_TempComp01 == true || PressOK == false || Flag_CaudT == true || Flag_CaudH == true || Flag_Temp_Adm == true || Flag_Temp_Descarga == true) //|| Flag_Aporte_E == true || Flag_Temp_Des == true Flag_TempComp01 == true ||
        {
          Estado_Maquina = 4;
          MenuCuatroCero();
          MenuActual = 40;
        }

        if (Temp_ACSacu > SetP_ACS)
        {
          flag_dtElectrico_final = true;
          Valor_DO_V4V = LOW; // si se alcanzan los 45 grados se deja de calentar
          Valor_DO_VACS = HIGH;
          Cal_ACS = false;
          Cal = true;
          Estado_Maquina = 0;
          Flag_retardo_e7 = false;
        }
        // if(Flag_TempComp01 == true)
        //   {
        //   Estado_Maquina = 0;
        //  Alarma_Eeprom = 6;
        //  EEPROM.write(Alarma_Address, Alarma_Eeprom);
        // }

        if ((millis() - Ingreso_E7) > 30000)
        {
          Valor_DO_Comp_01 = HIGH;
        }

        /**/
      }
      if (Temp_out_H > 50.0 || Temp_Descarga > 80.0) // se le da energia al ACS de a saltos para evitar pasar de presion y temperatura el circuito de gas
      {
        Estado_Maquina = 71;
        Ingreso_E71 = millis();
        Flag_retardo_e7 = false;
      }

      if (Temp_ACS >= SetP_ACS && ModoFrioCalor == false) //
      {
        Estado_Maquina = 0;
        Flag_retardo_e7 = false; // termino el ciclo en modo calor y vuelve a 0
      }
      else if (Temp_ACS >= SetP_ACS && ModoFrioCalor == true && Estado_Comp == 1) // termino el ciclo en modo frio y vuelve a 0, previa espera de 5 minutos en el estado 8
      {
        Valor_DO_Comp_01 = LOW;
        Estado_Comp = 0;
        Valor_DO_Bombas = HIGH;
        Periodo_Fin_ACS = millis();
        Estado_Maquina = 8;
        Flag_retardo_e7 = false;
      }
    }

    if (Estado_Maquina == 71) // Generacion ACS: Estado con bombas andando y compresor apagado
    {
      Valor_DO_Comp_01 = LOW;
      Estado_Comp = 0;
      Valor_DO_Bombas = HIGH;
      if ((millis() - Ingreso_E71) > 90000) // espero 0.5 minuto antes de volver al estado 7
      {
        Valor_DO_Bombas = HIGH; // las bombas arrancan enseguida, el compresor enciende 1 minuto despues
        Estado_Maquina = 7;
        Ingreso_E7 = millis();
      }
    } // FIn Estado 71

    if (Estado_Maquina == 8) // Estado de transicion al finalizar la generacion de ACS y el sistema esta en modo frio, evita circular agua muy caliente al circuito frio
    {
      if ((millis() - Periodo_Fin_ACS) > 30000)
      {
        Estado_Maquina = 0;
      }
    } // Fin Estado 8

    // IMAGEN DE SALIDAS
    digitalWrite(DO_Bombas, Valor_DO_Bombas);
    digitalWrite(DO_Calentador, Valor_DO_Calentador);
    digitalWrite(DO_Val2, Valor_DO_V4V);
    digitalWrite(DO_Comp_01, Valor_DO_Comp_01);
    digitalWrite(DO_Val1, Valor_DO_VACS);

    wdt_reset();
  }
  else if (Estado_ConfigWIFI == 1) // Esto se ejecuta cuando se activa la configuracion WIFI, el resto del codigo no se ejecuta hasta que se sale de este modo
  {

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
  } // Fin del estado config WIFI

} // Fin del loop

void Caudal1() // Función de Cuenta de Pulsos de Caudalímetro
{
  Pulsos_Caud_T++;
}

void Caudal2() // Función de Cuenta de Pulsos de Caudalímetro
{
  Pulsos_Caud_H++;
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
    if (readValue > maxValue)
    {
      /*record the maximum sensor value*/
      maxValue = readValue;
    }
    if (readValue < minValue)
    {
      /*record the maximum sensor value*/
      minValue = readValue;
    }
  }
  // Subtract min from max
  VPP = ((maxValue - minValue) * 5.0) / 1024.0;

  return VPP;
}
