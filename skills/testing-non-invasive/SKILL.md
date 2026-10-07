---
name: testing-non-invasive
description: Agregar testing al firmware sin modificar su lógica productiva durante la primera etapa.
---

# Testing no invasivo

## Regla principal

La primera etapa de testing debe probar el firmware existente. No se permite
refactorizar, extraer funciones, reemplazar expresiones ni cambiar contratos
solo para hacer posible un test.

## Orden obligatorio

1. Registrar el estado actual del worktree.
2. Compilar el firmware sin cambios funcionales.
3. Crear tests de caracterización del comportamiento existente.
4. Usar mocks, fakes, adapters de test o harnesses externos para sensores,
   reloj, EEPROM, LCD, pines y serial.
5. Comparar resultados observados contra el comportamiento actual.
6. Reportar los casos que no puedan probarse sin refactor.
7. Recién después de tener cobertura y aprobación explícita, proponer cambios
   de diseño separados de la etapa de testing.

## Excepción controlada

Si una parte no puede probarse con mocks, fakes o un harness externo, primero
se debe demostrar y documentar el bloqueo concreto. Ejemplos válidos:

- una espera real que vuelve impracticable el test;
- una dependencia de hardware que no puede sustituirse en el entorno;
- una API o recurso que no permite inyección desde el test.

Solo en ese caso se puede extraer la mínima pieza necesaria para desbloquear el
test. La extracción debe:

1. estar justificada por el bloqueo observado;
2. ser lo más pequeña posible;
3. preservar exactamente el comportamiento existente;
4. tener una regresión antes y después;
5. no incluir mejoras, limpieza ni cambios de contrato.

No se permite extraer funciones por comodidad, estética o anticipación.

## Prohibiciones de la primera etapa

- No extraer lógica a nuevas bibliotecas productivas.
- No reemplazar cálculos por helpers equivalentes.
- No cambiar `>` por `>=`, límites, timeouts o prioridades.
- No agregar validaciones nuevas que rechacen entradas antes aceptadas.
- No modificar el protocolo serial, payloads, secuencias o ACK.
- No cambiar la máquina de estados ni el orden del loop.
- No afirmar que una extracción es equivalente sin pruebas de regresión del
  comportamiento anterior.

## Criterio de aceptación

La etapa queda completa cuando los tests describen el comportamiento existente,
compilan y pasan sin alterar la lógica de producción. Los defectos descubiertos
se registran como hallazgos; no se corrigen silenciosamente dentro de la etapa
de testing.
