# Plan de testing: Arduino MEGA y ESP8266

## Objetivo

Agregar pruebas para verificar:

1. compilación de ambos proyectos;
2. lógica sin hardware;
3. protocolo MEGA–ESP;
4. MQTT;
5. funcionamiento físico completo.

No empezar conectando todo el equipo: eso no permite aislar errores.

## Regla obligatoria de la Etapa 1

La primera etapa debe ser no invasiva. Se prueba el firmware existente sin
modificar su lógica productiva.

Durante la Etapa 1 no se permite:

- extraer lógica a nuevas bibliotecas productivas;
- reemplazar cálculos, alarmas, navegación o transiciones por helpers;
- cambiar límites, operadores, timeouts, prioridades o contratos;
- agregar validaciones nuevas que rechacen entradas antes aceptadas;
- modificar el protocolo serial, payloads, secuencias o ACK;
- cambiar la máquina de estados o el orden del loop.

Si una función no puede probarse sin refactorizarla, se debe crear un mock,
fake, adapter de test o harness externo. Si eso tampoco alcanza, el caso queda
registrado como pendiente y no se modifica producción para forzarlo.

Si los mocks o el harness demuestran un bloqueo concreto —por ejemplo, una
espera real que vuelve impracticable el test— recién entonces se puede extraer
la mínima pieza necesaria. La extracción debe documentar el bloqueo, preservar
el comportamiento y tener una regresión antes y después. No se permiten
extracciones por comodidad, estética o anticipación.

Los defectos descubiertos se documentan como hallazgos. No se corrigen dentro
de la primera etapa. Las refactorizaciones y correcciones se planifican como
etapas separadas, después de contar con regresiones.

## Proyectos

- MEGA: /Users/diegorichi/projects/GeoNewen_v3_FIRMWARE_ACS
- ESP8266: /Users/diegorichi/projects/mqtt_esp8266_serial

## Estado inicial conocido

- Ningún proyecto tiene carpeta test/ ni un simulador.
- El código mezcla lógica, hardware, Serial, EEPROM, MQTT y variables globales.
- La comunicación MEGA–ESP tiene pruebas físicas parciales.
- Falta validar completamente ESP8266 -> MQTT.
- La compilación de PlatformIO quedó bloqueada previamente por permisos del directorio de PlatformIO.
- Hay cambios sin commit. No borrarlos ni revertirlos.

---

# Etapa 0: preparar el entorno

## 0.1. Guardar el estado de Git

En cada proyecto:

    git status --short

No ejecutar:

    git reset --hard
    git checkout -- .
    git clean -fd

Guardar la salida de ambos proyectos.

## 0.2. Compilar antes de modificar

El ejecutable de PlatformIO es:

    /Users/diegorichi/.platformio/penv/bin/pio

Preparar un directorio escribible:

    mkdir -p /private/tmp/geonewen-platformio

Compilar la MEGA:

    cd /Users/diegorichi/projects/GeoNewen_v3_FIRMWARE_ACS
    PLATFORMIO_CORE_DIR=/private/tmp/geonewen-platformio \
    /Users/diegorichi/.platformio/penv/bin/pio run -e megaatmega2560

Compilar el ESP8266:

    cd /Users/diegorichi/projects/mqtt_esp8266_serial
    PLATFORMIO_CORE_DIR=/private/tmp/geonewen-platformio \
    /Users/diegorichi/.platformio/penv/bin/pio run -e nodemcu

Resultado esperado: SUCCESS.

Si falla, clasificar el error antes de tocar código:

- permisos;
- dependencia;
- Internet;
- compilación;
- código fuente.

---

# Etapa 1: tests del proyecto MEGA

Proyecto:

    /Users/diegorichi/projects/GeoNewen_v3_FIRMWARE_ACS

Crear inicialmente:

    test/
    ├── test_protocol/
    ├── test_state_machine/
    ├── test_alarms/
    ├── test_measurements/
    ├── test_eeprom/
    └── test_ui/

No intentar compilar todo el firmware como programa nativo desde el primer día. Primero separar funciones que no dependan directamente de pines, LCD o HardwareSerial.

## 1.1. Protocolo serial

La MEGA utiliza:

    cmd:<secuencia>:<payload>#
    status:<secuencia>:<payload>#
    ack:cmd:<secuencia>#
    ack:status:<secuencia>#

Extraer funciones testeables para validar y construir estas tramas.

Casos mínimos:

- cmd:1:ACS_G:on# es válido;
- cmd:25:TEMP_ACS:45# es válido;
- una trama sin # queda incompleta;
- una secuencia no numérica es inválida;
- payload vacío es inválido;
- una trama demasiado larga se descarta;
- caracteres inválidos se descartan;
- una secuencia repetida se ejecuta una sola vez;
- una secuencia nueva se ejecuta.

## 1.2. ACK y reintentos

La MEGA usa una trama pendiente, timeout de 2 segundos y máximo de 3 intentos.

Probar:

1. envío de una trama;
2. ACK correcto;
3. ACK de otra secuencia;
4. timeout;
5. primer reintento;
6. tercer intento;
7. descarte sin ACK;
8. bloqueo de nuevas tramas mientras existe una pendiente.

No usar delay(2000) en tests. Usar reloj falso o tiempo inyectable.

## 1.3. Máquina de estados

Archivo principal:

    stateMachine.cpp

Probar:

- equipo apagado;
- marcha apagada;
- marcha encendida;
- arranque;
- funcionamiento normal;
- alarma activa;
- reset de alarma;
- descanso;
- generación de ACS;
- modo frío;
- cambio frío/calor;
- cambio de modo con compresor funcionando;
- cambio de modo durante bloqueo de seguridad.

Verificar en cada caso:

- Estado_Maquina;
- Valor_DO_Bombas;
- Valor_DO_Compressor;
- Valor_DO_VACS;
- Valor_DO_V4V;
- Valor_DO_Calentador;
- estado de alarma.

No verificar solo el estado interno. Verificar también las salidas calculadas.

## 1.4. Alarmas

Archivos involucrados:

    alarm.cpp
    measurement_and_calculations.cpp
    stateMachine.cpp

Probar:

- falta de caudal;
- presión alta;
- presión baja;
- temperatura excesiva del compresor;
- temperatura excesiva de descarga;
- sensor desconectado;
- varias alarmas simultáneas;
- alarma mientras se solicita marcha;
- reset;
- historial;
- prioridad de alarma sobre navegación y funcionamiento.

No inventar una política nueva para sensores inválidos. Primero documentar el comportamiento actual y después decidir si el valor inválido conserva el anterior, genera alarma o fuerza una salida segura.

## 1.5. Sensores y cálculos

Probar:

- 0 °C;
- 25 °C;
- 45 °C;
- 85 °C, valor típico de inicialización del DS18B20;
- -127 °C, valor típico de desconexión;
- valores fuera de rango;
- caudal cero;
- caudal alto;
- pulsos;
- ausencia de pulsos;
- lecturas intermitentes.

Verificar temperatura, caudal, acumuladores, flags, alarmas y salidas.

Los casos 85 °C y -127 °C deben ser tests separados.

## 1.6. EEPROM

Probar:

- recuperación de valores;
- valor inválido con valor seguro;
- setpoint dentro de rango;
- un valor no modifica otros;
- dirección correcta para cada flag;
- no escribir EEPROM en cada loop.

La EEPROM real se valida luego sobre la MEGA. En tests nativos usar una EEPROM falsa en memoria.

## 1.7. Teclado y navegación

Probar:

- subir;
- bajar;
- aceptar;
- cancelar;
- navegación circular;
- menú principal;
- configuración;
- alarmas;
- modos modales;
- acciones;
- impedir que una tecla ejecute navegación y acción al mismo tiempo.

Primero probar estado y acciones. La LCD real queda para la etapa física.

---

# Etapa 2: tests del proyecto ESP8266

Proyecto:

    /Users/diegorichi/projects/mqtt_esp8266_serial

Crear inicialmente:

    test/
    ├── test_mqtt_commands/
    ├── test_mega_frames/
    ├── test_command_retries/
    ├── test_status_publishing/
    ├── test_frame_limits/
    └── test_littlefs/

## 2.1. MQTT hacia la MEGA

La función callback() transforma MQTT en comandos seriales.

Probar los temas:

    kume/modo_frio
    kume/acs_g
    kume/acs_dt_e
    kume/acs_e
    kume/acs_temp_set
    kume/alarm

Casos esperados:

- modo frío 1 -> MODO_FRIO:on;
- modo frío 0 -> MODO_FRIO:off;
- ACS 1 -> ACS_G:on;
- ACS 0 -> ACS_G:off;
- setpoint 45 -> TEMP_ACS:45;
- alarma reset -> ALARM:reset.

Probar también:

- 10;
- 01;
- texto vacío;
- texto con espacios;
- texto que contiene 1 pero no representa un valor válido.

Esto es necesario porque el código actual busca el carácter 1 dentro del texto.

## 2.2. Construcción de comandos

Cada comando debe tener:

    cmd:<secuencia>:<payload>#

Probar:

- secuencia inicial;
- incremento;
- payload vacío;
- payload largo;
- cola llena;
- preservación del orden;
- no reutilización incorrecta de secuencias.

## 2.3. Recepción desde la MEGA

La función handleUno() recibe bytes y arma tramas terminadas en #.

Probar:

- una trama completa;
- una trama recibida en varios fragmentos;
- dos tramas juntas;
- trama sin #;
- trama demasiado larga;
- trama vacía;
- trama inválida;
- cola llena;
- bytes basura antes de una válida.

No asumir que Serial.read() entrega una trama completa: puede entregar un byte por vez.

## 2.4. ACK de comandos

sendPendingCommand() debe:

- enviar un único comando;
- esperar ACK;
- aceptar solo la secuencia correcta;
- ignorar ACK incorrecto;
- reintentar después de 700 ms;
- intentar como máximo 3 veces;
- descartar sin ACK;
- no enviar otro comando mientras espera.

Casos mínimos:

    comando -> ACK correcto -> finaliza
    comando -> ACK incorrecto -> sigue esperando
    comando -> sin ACK -> reintenta
    tres intentos sin ACK -> descarta

## 2.5. Recepción de estados

Al recibir:

    status:<secuencia>:<payload>#

el ESP debe extraer la secuencia, encolar el payload y responder:

    ack:status:<secuencia>#

Probar status válido, status sin secuencia, status sin payload, status demasiado largo, status duplicado y cola llena.

## 2.6. Publicación MQTT

checkAndPublish() debe conservar exactamente el tamaño del campo.

Probar especialmente:

- estado de máquina de 2 caracteres;
- temperaturas de 6 caracteres;
- caudales de 4 caracteres;
- alarma de 2 caracteres;
- espacios iniciales;
- ceros iniciales;
- signo negativo;
- payload vacío;
- payload más largo que el permitido.

Verificar:

- mismo valor no se publica inmediatamente;
- mismo valor se publica después de 90 minutos;
- valor distinto se publica inmediatamente;
- longitud publicada igual a longitud real;
- ausencia de bytes basura.

No usar trim(). Los espacios pueden formar parte del contrato.

## 2.7. MQTT desconectado

Usar un cliente MQTT falso para probar:

- error de publicación;
- reconexión;
- pérdida de conexión;
- cola acumulada;
- recuperación;
- duplicados;
- reinicio durante publicación.

publish() aceptado localmente no demuestra recepción del broker. Eso se valida con un broker de test.

## 2.8. Faltantes detectados en la primera auditoría

Los tests existentes no deben considerarse cobertura completa. Antes de cerrar
las etapas de software hay que completar los siguientes casos.

### Faltantes de la MEGA

#### Acciones y navegación

- Probar los límites inferior y superior de `increaseAcsSetpoint()` y
  `decreaseAcsSetpoint()`.
- Probar cada acción individual de `menu_actions.cpp` y su dirección EEPROM.
- Probar acciones inválidas para cada menú y botón.
- Recorrer todas las rutas UP, DOWN, ENTER y BACK de todos los menús.
- Verificar que `MENU_NONE` no cambie el menú ni ejecute una acción.
- Probar `navigateTo()` con un identificador inválido.
- Probar que una tecla no navegue y ejecute una acción al mismo tiempo.

#### Máquina de estados

- Probar los límites exactos de 15 s, 25 s, 60 s, 20 s y 400 s.
- Completar Estado 7: ACS deshabilitado, setpoint exacto, bombas antes y
  después de 15 s, compresor antes y después de 20 s y cada transición a 71.
- Completar Estado 71: timeout, temperatura dentro y fuera de rango, cada
  condición de retorno y salidas resultantes.
- Probar Estado 4 con cada alarma, buzzer ya activo y alarma ya convertida.
- Probar Estado 6 exactamente en el límite y con `heating_off`.
- Probar dos ciclos completos de la rutina diaria de bombas, incluyendo que no
  se rearme durante los 10 s.
- Verificar salidas después de cada salto de estado, no solamente el número de
  estado.

#### Alarmas, mediciones y EEPROM

- Probar `resetAlarms()` fuera del Estado 4.
- Probar todas las combinaciones de presión alta y baja y su prioridad.
- Verificar persistencia cuando no existe alarma.
- Probar límites exactos de aceptación y rechazo de los ocho sensores.
- Probar conversión pendiente, conversión incompleta, conversión completa y
  nueva solicitud de temperatura después de un segundo.
- Probar caudal hogar, caudal tierra, ambos, cero pulsos y ventana exacta de un
  segundo.
- Probar `flowControl()` fuera de los estados 3 y 7.
- Probar activación y recuperación de cada contador de temperatura y presión.
- Cubrir toda la tabla de condiciones de `auxiliaryACSHeatingControl()`.
- Verificar que una escritura EEPROM no modifique otras direcciones ni se
  repita en cada loop.

#### LCD y protocolo serial MEGA

- Verificar el texto renderizado de cada pantalla, no solamente que la función
  haya sido llamada.
- Cubrir cada modo, estado, valor numérico y código de alarma en LCD.
- Probar los 16 mensajes generados por `enqueueStatusToSend()`.
- Probar todos los comandos entrantes: ACS, delta, ACS eléctrico, alarma, modo
  y target ACS, en ON y OFF cuando corresponda.
- Probar payload vacío, payload largo, caracteres inválidos, cola llena y
  secuencias inválidas.
- Probar ACK correcto, incorrecto, duplicado, no numérico y desbordamiento de
  secuencia.
- Probar longitud, formato y contenido exactos de cada status.

### Faltantes del ESP8266

#### MQTT y comandos

- Probar ON y OFF de cada tópico de modo, ACS general, delta y ACS eléctrico.
- Probar target ACS válido, vacío, negativo y fuera de rango.
- Probar alarma distinta de `reset`, tópico desconocido y payload vacío.
- Probar los valores `10`, `01`, texto con espacios y texto que contiene `1`
  pero no representa un valor booleano válido. El código actual busca el
  carácter `1`; el comportamiento esperado debe quedar definido por estos
  tests.
- Probar secuencia inicial, incremento, payload largo, cola llena, orden y no
  reutilización de secuencias.

#### Serial, ACK y colas

- Probar frame vacío, frames concatenados, fragmentación, frame sin `#`, frame
  mayor a 80 caracteres, caracteres basura y cola llena.
- Probar `status:` sin secuencia, sin payload, secuencia no numérica y ACK de
  status inválido.
- Probar espera exacta de 250 ms, timeout exacto de 700 ms, tres intentos,
  descarte, cola vacía y comando siguiente después de ACK o descarte.
- Probar ACK correcto, incorrecto, duplicado y no numérico.
- Probar que `processUnoFrames()` y `processCloudQueue()` respeten sus límites
  por llamada.

#### Publicación e inicialización

- Probar cada canal de estado MQTT, no solamente `STATE_MACH`.
- Probar cada longitud de campo, espacios, ceros iniciales, negativos, vacío y
  payload más largo que el contrato.
- Probar mismo valor, valor distinto y keepalive después de 90 minutos.
- Probar cola cloud llena y descarte explícito.
- Probar con mocks `setup_wifi()`, `setDateTime()`, `reconnect()`, `setup()` y
  el orden de `loop()`.
- Probar cliente MQTT desconectado, reconexión, cola acumulada, duplicados y
  error de publicación.

### Faltantes del contrato MEGA-ESP

Crear una suite común que ejecute el flujo completo:

    ESP genera cmd -> MEGA procesa -> MEGA genera status -> ESP procesa
    -> ESP confirma ACK -> MEGA cierra el reintento

Debe incluir comandos, status, ACK, duplicados, secuencias inválidas, frames
truncados, frames largos, payload vacío, espacios, signos y caracteres
inválidos. Los parsers no deben validarse únicamente por separado.

---

# Etapa 3: contrato compartido

Crear casos comunes, por ejemplo:

    test/protocol_cases.json

Ejemplo:

    [
      {
        "input": "cmd:1:ACS_G:on#",
        "expected": "ACS_G:on",
        "valid": true
      },
      {
        "input": "status:15:status:STATE_MACH: 2#",
        "valid": true
      }
    ]

Ejecutar los mismos casos contra el parser de la MEGA y el parser del ESP8266.

Incluir:

- comandos;
- status;
- ACK;
- secuencias duplicadas;
- tramas truncadas;
- tramas demasiado largas;
- espacios;
- signos;
- payload vacío;
- caracteres inválidos.

---

# Etapa 4: integración sin instalación completa

Crear una prueba:

    simulador MEGA <-> simulador ESP8266

Debe:

1. recibir un comando MQTT falso;
2. convertirlo en trama;
3. devolver ACK;
4. generar un estado;
5. devolver ACK del estado;
6. publicar el estado en un broker de pruebas;
7. verificar topic y payload.

Debe detectar mensajes fusionados, perdidos, secuencias incorrectas, ACK incorrectos, reintentos, longitudes incorrectas y publicaciones duplicadas.

No usar el broker de producción.

---

# Etapa 5: MQTT con broker de test

No poner credenciales reales dentro de tests ni del repositorio.

Probar:

1. conexión del ESP;
2. suscripción;
3. comando MQTT;
4. conversión del comando;
5. recepción en la MEGA;
6. respuesta de la MEGA;
7. publicación del ESP;
8. recepción correcta en el broker.

Repetir con:

- Wi-Fi desconectado;
- MQTT desconectado;
- serial desconectado;
- ACK perdido;
- reinicio del ESP;
- reinicio de la MEGA;
- cola llena;
- mensaje duplicado.

---

# Etapa 6: prueba física de la MEGA

## 6.1. Banco seguro

Antes de energizar:

- desconectar compresor;
- desconectar bombas;
- desconectar válvulas de potencia;
- desconectar triacs de cargas reales;
- usar LEDs, relés de baja tensión o instrumentos;
- usar fuente limitada en corriente;
- verificar masa común;
- comprobar tensiones con multímetro.

No probar cargas de red sin procedimiento eléctrico seguro.

## 6.2. Entradas

Activar una a la vez:

- caudal tierra;
- caudal hogar;
- marcha;
- presión alta;
- presión baja.

Registrar entrada, variable, estado, alarma y salida.

## 6.3. Sensores

Probar:

- sensor conectado;
- desconectado;
- temperatura ambiente;
- temperatura elevada;
- lectura inválida;
- arranque.

Guardar logs y salidas observadas.

## 6.4. LCD y teclado

Verificar:

- cada pantalla;
- navegación;
- setpoint;
- cambio de modo;
- configuración ACS;
- alarmas;
- historial;
- regreso al menú.

Registrar pantallas incorrectas y acciones duplicadas.

## 6.5. Salidas

Con cargas seguras verificar:

- bombas;
- compresor simulado;
- válvula de cuatro vías;
- válvula ACS;
- calentador;
- buzzer;
- triac.

Para cada condición registrar:

    condición de entrada
    estado esperado
    estado observado

## 6.6. MEGA–ESP

A 4800 baudios verificar:

- comandos MQTT llegan a la MEGA;
- ACK de comandos;
- estados salen de la MEGA;
- ACK de estados;
- reintentos;
- pérdida de ACK;
- recuperación;
- orden de mensajes;
- ausencia de tramas corruptas.

Guardar logs de ambos equipos.

---

# Etapa 7: rendimiento y watchdog

La MEGA mide:

    loop_max_us
    serial2_max_pending

Probar con:

1. ningún periférico;
2. LCD;
3. sensores;
4. teclado;
5. serial activo;
6. entradas cambiando;
7. funcionamiento normal.

Registrar máximo del loop, bytes pendientes, reinicios, pérdida de comunicación y alarmas falsas.

No declarar rendimiento validado con sensores, LCD y cargas desconectadas.

---

# Etapa 8: suite completa

## MEGA

    cd /Users/diegorichi/projects/GeoNewen_v3_FIRMWARE_ACS
    PLATFORMIO_CORE_DIR=/private/tmp/geonewen-platformio \
    /Users/diegorichi/.platformio/penv/bin/pio test -e native
    PLATFORMIO_CORE_DIR=/private/tmp/geonewen-platformio \
    /Users/diegorichi/.platformio/penv/bin/pio run -e megaatmega2560

## ESP8266

    cd /Users/diegorichi/projects/mqtt_esp8266_serial
    PLATFORMIO_CORE_DIR=/private/tmp/geonewen-platformio \
    /Users/diegorichi/.platformio/penv/bin/pio test -e native
    PLATFORMIO_CORE_DIR=/private/tmp/geonewen-platformio \
    /Users/diegorichi/.platformio/penv/bin/pio run -e nodemcu

## Revisión final

    git diff --check
    git status --short

No incluir como cambios de código los archivos generados dentro de .pio/.

---

# Criterio de finalización

No alcanza con que compile.

El testing queda completo únicamente cuando:

- pasan los tests nativos de ambos proyectos;
- pasan los tests de contrato;
- pasan ACK, duplicados y reintentos;
- se verifica la longitud exacta de payloads;
- se prueba MQTT con broker de test;
- se prueba comunicación física MEGA–ESP;
- se prueban entradas, sensores, teclado, LCD y salidas;
- se prueba watchdog;
- se mide el loop con periféricos;
- se documentan los casos no reproducidos.

## Control de completitud

### Revisión 1

Antes de comenzar, confirmar que el plan cubre:

- tests nativos;
- simulador;
- contrato compartido;
- MQTT;
- periféricos;
- reintentos;
- colas;
- artefactos generados;
- estado de Git.

### Revisión 2

Antes de declarar terminado, revisar nuevamente:

- casos normales;
- casos inválidos;
- duplicados;
- tramas truncadas;
- colas llenas;
- ACK perdido;
- Wi-Fi desconectado;
- MQTT desconectado;
- serial desconectado;
- reinicios;
- watchdog;
- rendimiento;
- payload esperado contra payload recibido.

Estado inicial: implementación pendiente.

---

# Registro de ejecución no invasiva

## Etapas 0 y 1 rehechas

### Etapa 0

- Se verificó el estado del worktree antes de modificar.
- Se confirmó que los archivos productivos de la MEGA volvieron al estado
  anterior a la primera implementación de testing.
- La MEGA compiló con `megaatmega2560`.
- El ESP8266 compiló con `nodemcu`.

### Etapa 1

- La lógica productiva se dejó intacta durante la creación de las pruebas. Luego
  de demostrar tres regresiones rojas, se corrigieron únicamente esos tres
  comportamientos detectados: cancelación ACS con BACK, ciclo diario de bombas
  y validación de secuencias seriales.
- No se agregaron helpers productivos.
- No se cambiaron límites, operadores, timeouts, payloads, ACK, estados ni el
  orden del loop.
- Se configuró un entorno `native` de PlatformIO solamente para tests.
- Se agregaron mocks de reloj, pines, TimerOne, EEPROM, sensores, LCD, teclado,
  `String`, cola y `HardwareSerial`.
- `test/test_state_machine/test_main.cpp` incluye y ejecuta el
  `stateMachine.cpp` original: 21 tests exitosos.
- `test/test_alarm/test_main.cpp` incluye y ejecuta el `alarm.cpp` original:
  7 tests exitosos.
- `test/test_eeprom/test_main.cpp` incluye y ejecuta el `kume_eeprom.cpp`
  original: 3 tests exitosos.
- `test/test_machine_control/test_main.cpp` incluye y ejecuta el
  `machine_control.cpp` original: 7 tests exitosos.
- `test/test_measurements/test_main.cpp` incluye y ejecuta el
  `measurement_and_calculations.cpp` original: 10 tests exitosos.
- `test/test_navigation/test_main.cpp` incluye y ejecuta navegación y acciones
  originales: 9 tests exitosos.
- `test/test_keyboard/test_main.cpp` incluye y ejecuta el `keyboard.cpp`
  original: 2 tests exitosos.
- `test/test_ui_controller/test_main.cpp` incluye y ejecuta el
  `ui_controller.cpp` original: 2 tests exitosos.
- `test/test_lcd/test_main.cpp` incluye y ejecuta el `functionsLCDMenu.cpp`
  original con LCD falso: 3 tests exitosos.
- `test/test_serial/test_main.cpp` incluye y ejecuta el protocolo original de
  `SerialEsp8266.h`: 10 tests exitosos.
- Resultado actual: 75/75 tests nativos exitosos.

En el ESP8266 se agregaron 21 tests nativos con mocks para comandos MQTT,
parser serial, ACK, reintentos, colas y publicaciones simuladas. Estos números
son resultados de ejecución, no una declaración de cobertura completa.

### Pendientes explícitos

- Estos tests ejecutan funciones reales del firmware, pero no ejecutan el
  firmware completo ni validan hardware físico.
- No se validó hardware físico ni MQTT en esta etapa.
- No se hizo ningún refactor productivo para habilitar estas pruebas.
- La validación física MEGA–ESP8266 pertenece a la etapa de hardware.

Estado: completado dentro del alcance sin hardware: ambos proyectos tienen
tests nativos ejecutables, mocks, validación de protocolo, colas, ACK,
reintentos, límites, canales MQTT simulados y builds reales exitosos. La
validación sobre placa, serial físico, WiFi/NTP real, broker real, OTA y LCD o
sensores físicos queda como siguiente etapa de hardware/integración.

### Hallazgos observados y corregidos

- Una secuencia no numérica en una trama `cmd:` se convertía a cero mediante
  `String::toInt()` y recibía ACK. Se agregó validación numérica antes de
  ejecutar o confirmar la orden.
- La rutina diaria de bombas podía rearmarse en cada loop. Ahora conserva la
  activación hasta completar los diez segundos y luego reinicia el ciclo.
- El botón BACK en edición de target ACS dejaba el buffer de edición aplicado.
  Ahora cancela el buffer y conserva el valor anterior; ENTER sigue siendo el
  commit.
- La conversión usada para validar la secuencia fue ajustada a `.c_str()` para
  compilar también con el toolchain AVR.
- La MEGA aceptaba una trama `status:x:...` como secuencia cero. Se agregó un
  test rojo y validación numérica antes de activar el reintento.
- El ESP interpretaba `10` como ON porque buscaba cualquier carácter `1`. Se
  agregó un test rojo y se cambió la condición a igualdad exacta con `1`.

## Resultado final de la iteración

- Terminé 75 tests en MEGA.
- Terminé 21 tests en ESP8266.
- En esta iteración fallaron 2 tests porque detectaron bugs reales; ambos fueron
  corregidos. Sumados a las 3 regresiones rojas de la iteración anterior, hubo
  5 fallas rojas acumuladas por bugs reales. La ejecución final quedó en 96/96
  tests exitosos.

### Explicación de las fallas

1. `test_non_numeric_status_sequence_is_not_sent`: la MEGA convertía una
   secuencia no numérica a cero con `toInt()` y la enviaba. Se agregó validación
   numérica antes de marcar el status como pendiente.
2. `test_boolean_mqtt_payload_requires_exact_value`: el ESP convertía `10` en
   ON porque usaba `indexOf("1")`. Se cambió a comparación exacta con `"1"`.

### Verificación final

- MEGA native: 75/75.
- ESP8266 native: 21/21.
- Build MEGA `megaatmega2560`: SUCCESS.
- Build ESP8266 `nodemcu`: SUCCESS.
- `git diff --check`: sin errores.

### Siguientes pasos

- Probar con placa MEGA y ESP8266 reales.
- Validar el enlace serial físico y pérdida/reconexión del enlace.
- Validar WiFi, NTP, TLS, broker MQTT real, LittleFS/certificados y OTA.
- Verificar sensores, teclado, LCD, salidas, watchdog y rendimiento del loop.

Plan `PLAN_TESTING_MEGA_ESP8266.md` completado dentro del alcance sin hardware.

## Ejecución verificada: 2026-10-07

Se repitió la validación con el estado actual de ambos repositorios:

- MEGA: `pio test -e native`: 75/75 exitosos.
- ESP8266: `pio test -e native`: 21/21 exitosos.
- MEGA: `pio run -e megaatmega2560`: `SUCCESS`.
- ESP8266: `pio run -e nodemcu`: `SUCCESS`.
- `git diff --check`: sin errores.

La ejecución requirió reconstruir un cache temporal de PlatformIO a partir de
paquetes ya instalados porque el cache indicado inicialmente no estaba completo.
Eso no modifica el firmware ni constituye evidencia de hardware.

## Estado real de cierre

El alcance local sin hardware queda verificado. El plan general todavía no está
completo: no hay puertos seriales de las placas ni broker de test disponible en
este entorno. Siguen pendientes y no se deben marcar como ejecutados:

- contrato común MEGA–ESP y simulador extremo a extremo;
- broker MQTT de test, Wi-Fi, NTP, TLS, LittleFS/certificados y OTA;
- comunicación serial física, entradas, sensores, teclado, LCD y salidas;
- watchdog y medición del loop con periféricos conectados.

Estado: `parcial`, con el software local cerrado y la validación física bloqueada
por falta de placas/periféricos y broker accesibles.

## Ejecución adicional sin hardware: 2026-10-07

Se completó la parte reproducible sin placas:

- MEGA native: 76/76 tests exitosos.
- ESP8266 native: 22/22 tests exitosos.
- Casos de protocolo equivalentes, definidos y ejecutados dentro de cada
  repositorio, sin imports ni tests cruzados.
- Simulador host: 4/4 tests exitosos para comando, ACK, status, ACK de status,
  duplicados y tramas inválidas.
- Builds de MEGA y ESP8266: `SUCCESS` después de los cambios.

La cobertura detectó y corrigió un defecto real del ESP8266: aceptaba una
secuencia no numérica en `status:x:...` y enviaba ACK. Ahora descarta esa trama.

No se marca como cerrado el broker real, porque no hay Mosquitto/EMQX instalado
ni un broker de test disponible. Eso no requiere hardware, pero requiere
instalar y ejecutar el broker antes de validar conexión, publicación y
reconexión reales.

## Decisión posterior a la primera ejecución

La primera implementación de la Etapa 1 modificó lógica productiva para hacerla
testeable. Esa estrategia quedó descartada y la Etapa 1 fue rehecha con
testing no invasivo. La regla queda como criterio permanente: primero se agrega
el test rojo y recién después se corrige el comportamiento probado.

Plan `PLAN_TESTING_MEGA_ESP8266.md` completado dentro del alcance sin hardware.
