# Tests no invasivos de la MEGA

Estos tests ejecutan partes del código existente con mocks de Arduino, sin
modificar su lógica productiva.

Ejecutar desde la raíz del proyecto:

    PLATFORMIO_CORE_DIR=/private/tmp/geonewen-platformio pio test -e native

La suite ejecuta funciones reales del firmware; reloj, pines, EEPROM, TimerOne,
sensores, LCD, teclado, `String`, cola y `HardwareSerial` son mocks.

Incluye máquina de estados —incluido ACS y estado 71—, alarmas, cálculos y
umbrales, EEPROM, control de máquina, navegación, teclado, UI, LCD y protocolo
serial MEGA–ESP8266. La ejecución física y MQTT quedan fuera de esta etapa.

Resultado de referencia actual: 75/75 tests nativos exitosos.
