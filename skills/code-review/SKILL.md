---
name: code-review
description: Revisión crítica de calidad, seguridad, rendimiento y cobertura del firmware.
---

# Code Review

Revisar el código con foco en problemas reales, no solamente en formato.

## Categorías

1. **Lógica y seguridad**: estados imposibles, alarmas omitidas, salidas inseguras,
   entradas inválidas, secretos y pérdida de comunicación.
2. **Rendimiento y memoria**: bloqueos, uso de `String`, crecimiento de colas,
   tiempos de loop, RAM y flash.
3. **Contratos**: formato de tramas, secuencias, ACK, reintentos, longitudes y
   compatibilidad entre MEGA y ESP8266.
4. **Testing**: cobertura de casos límite, errores, duplicados, desconexiones y
   diferencia entre pruebas nativas y pruebas físicas.
5. **Mantenibilidad**: acoplamiento con Arduino, responsabilidades mezcladas,
   duplicación y cambios más amplios que la necesidad.

## Formato de revisión

```text
### CRÍTICO
- Problema, impacto y corrección obligatoria.

### IMPORTANTE
- Problema, riesgo y corrección recomendada.

### MENOR
- Mejora de claridad o mantenimiento.

### VERIFICADO
- Evidencia positiva comprobada.

### RESUMEN
- Cambios revisados, pruebas ejecutadas y pendientes.
```

No llamar “resuelto” a un problema si solo se compiló o se ejecutó un test nativo.
