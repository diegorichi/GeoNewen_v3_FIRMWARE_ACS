# Plan: menú indexado y legible

## Objetivo

Mantener la navegación actual, eliminar los números mágicos (`210`, `281`, etc.) y centralizar las transiciones sin usar la implementación OO descartada.

## Alcance

- Crear identificadores nombrados para cada pantalla (`MENU_HOME`, `MENU_MONITOR`, `MENU_CONFIGURATION`, etc.).
- Crear una tabla de navegación con arriba, abajo, enter, atrás, dibujo, refresco y acción opcional.
- Reemplazar los `switch` duplicados de `keyboard.cpp` por una función de navegación común.
- Mantener inicialmente las funciones LCD existentes y renombrarlas después solo si aporta claridad.
- Conservar la navegación circular ya definida.

## Validación

- Recorrer todos los menús con arriba y abajo.
- Verificar los saltos circulares.
- Verificar enter, atrás y edición de temperatura ACS.
- Verificar alarmas, EEPROM y refresco de pantalla.
- Confirmar que no queden referencias a `MenuActual` numérico fuera de la capa de navegación.

## Restricciones

- No reintroducir objetos con punteros a temporales.
- No usar `String` ni memoria dinámica para describir el menú.
- No cambiar el comportamiento operativo durante este refactor.
