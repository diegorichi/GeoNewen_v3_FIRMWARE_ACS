# Plan: separación de responsabilidades y segunda caldera

## Objetivo

Documentar la separación entre la caldera controlada por la MEGA y la segunda
caldera, que será controlada exclusivamente por el ESP8266.

La MEGA no controlará la segunda caldera ni se convertirá en un controlador
multiunidad. Solo recibirá su estado y bloqueará la señal de arranque de la
caldera actual mientras la segunda esté funcionando, sin afectar la generación
de ACS.

El alcance anterior de convertir toda la MEGA en una arquitectura multiunidad
queda fuera de este trabajo.

## Problemas actuales a separar

- `vars.h/.cpp` mezcla pines, estado operativo, sensores, salidas, alarmas y configuración.
- `measurement_and_calculations.cpp` mezcla lectura, interrupciones, caudales, promedios, protecciones y ACS.
- `machine_control.cpp` mezcla hardware, modo, salidas, buzzer y decisiones.
- `stateMachine.cpp` implementa una sola unidad con estado global.
- `functionsLCDMenu.cpp` mezcla dibujo, refresco, alarmas y navegación.
- `SerialEsp8266.h` mezcla transporte, protocolo, colas y serialización.
- EEPROM no separa configuración por unidad.

## Diseño objetivo

- Mantener la MEGA como controlador de una sola caldera.
- Agregar `segundaCalderaActiva`, inicializado en `false` y persistido en EEPROM.
- Recibir `CALDERA_2:on/off` desde el ESP8266 para activar o desactivar ese flag.
- Cuando el flag esté activo, bloquear `senalStart` y provocar `senalStop`.
- Mantener `heatingOff` separado: continúa siendo el apagado total.
- Permitir que los estados de generación de ACS sigan funcionando.
- Agregar la representación del flag al menú y definir si se persiste en EEPROM.
- No duplicar sensores, salidas, máquina de estados ni archivos de la MEGA.
- En una etapa posterior, la MEGA reportará su estado al ESP y el ESP deberá
  sincronizar el estado de la segunda caldera con ese reporte. Esa coordinación
  no forma parte de esta implementación.

## Orden

1. Definir el nombre del flag y el comando MEGA–ESP.
2. Definir si el flag persiste en EEPROM.
3. Implementar el bloqueo de arranque sin alterar `heatingOff` ni ACS.
4. Agregar menú, protocolo y tests.
5. Validar que la caldera actual se detenga y que ACS continúe disponible.

## Restricciones

- No implementar control directo de la segunda caldera en la MEGA.
- No usar `heatingOff` para representar el bloqueo de la segunda caldera.
- No cambiar pines ni la máquina de estados de la caldera actual.
