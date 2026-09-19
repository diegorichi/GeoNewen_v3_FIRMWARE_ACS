# Plan: separación de responsabilidades y segunda caldera

## Objetivo

Preparar el firmware para una segunda caldera sin duplicar archivos ni multiplicar variables globales.

## Problemas actuales a separar

- `vars.h/.cpp` mezcla pines, estado operativo, sensores, salidas, alarmas y configuración.
- `measurement_and_calculations.cpp` mezcla lectura, interrupciones, caudales, promedios, protecciones y ACS.
- `machine_control.cpp` mezcla hardware, modo, salidas, buzzer y decisiones.
- `stateMachine.cpp` implementa una sola unidad con estado global.
- `functionsLCDMenu.cpp` mezcla dibujo, refresco, alarmas y navegación.
- `SerialEsp8266.h` mezcla transporte, protocolo, colas y serialización.
- EEPROM no separa configuración por unidad.

## Diseño objetivo

- Introducir `UnitId` (`CALDERA_1`, `CALDERA_2`).
- Agrupar por unidad sensores, caudales, salidas, alarmas, modo, setpoints, timers y estado de máquina.
- Ejecutar la misma lógica de control para cada unidad mediante un contexto por unidad.
- Mantener teclado, LCD y comunicación como recursos compartidos.
- Definir explícitamente qué alarmas e interlocks son por unidad y cuáles son globales.
- Separar el mapa de pines y las direcciones OneWire de la lógica de control.
- Reservar EEPROM por unidad.
- Versionar el contrato de nube con identificador de unidad, por ejemplo:

```text
status:CALDERA_1:TEMP_ACS:47.8#
status:CALDERA_2:TEMP_ACS:46.9#
```

## Orden

1. Auditar consumidores y contratos actuales.
2. Extraer tipos y contexto de unidad sin cambiar comportamiento.
3. Migrar lecturas y cálculos al contexto.
4. Migrar máquina de estados y alarmas.
5. Migrar salidas y EEPROM.
6. Actualizar menú y protocolo para seleccionar/distinguir unidad.
7. Validar una unidad idéntica a la actual antes de habilitar la segunda.

## Restricciones

- No duplicar `stateMachine.cpp` para crear una segunda caldera.
- No cambiar pines ni contratos sin mapa de hardware aprobado.
- No llamar completada la migración sin probar la unidad actual con todos sus periféricos.
