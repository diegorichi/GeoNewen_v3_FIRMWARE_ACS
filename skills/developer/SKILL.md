---
name: developer
description: Reglas de desarrollo mantenible y testeable para el firmware.
---

# Reglas de desarrollo

1. Separar lógica de negocio, hardware, transporte serial, EEPROM, LCD y MQTT.
2. Hacer cambios pequeños y reversibles.
3. No eliminar código ajeno al cambio solicitado.
4. Agregar tests para cada comportamiento nuevo o modificado.
5. Probar casos normales, límites, entradas inválidas y errores de comunicación.
6. Preferir funciones pequeñas, nombres claros y responsabilidades únicas.
7. Documentar por qué existe una decisión compleja; no llenar el código de comentarios obvios.
8. Mantener los contratos seriales y los anchos de payload salvo autorización explícita.
9. No afirmar que el hardware funciona basándose únicamente en una compilación o un test nativo.
10. Revisar el diff final y los artefactos generados antes de declarar terminado el cambio.
