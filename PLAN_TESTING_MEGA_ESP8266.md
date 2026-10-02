# Plan de testing: Arduino MEGA y ESP8266

## Objetivo

Agregar pruebas para verificar:

1. compilación de ambos proyectos;
2. lógica sin hardware;
3. protocolo MEGA–ESP;
4. MQTT;
5. funcionamiento físico completo.

No empezar conectando todo el equipo: eso no permite aislar errores.

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
