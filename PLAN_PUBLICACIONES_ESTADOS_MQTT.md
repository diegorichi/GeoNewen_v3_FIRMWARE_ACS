# Plan: publicaciones inmediatas de estados MEGA → ESP → MQTT

## Objetivo

Hacer que los cambios relevantes de la MEGA lleguen inmediatamente a la nube,
sin esperar la tanda periódica de estados.

La MEGA detecta el cambio y genera una trama `status`. El ESP la recibe,
confirma por ACK y la publica en MQTT conservando el formato del valor.

## Publicaciones inmediatas

La MEGA deberá publicar cuando cambie cualquiera de estos valores:

- estado de máquina;
- estado del termostato;
- ACS geotérmico habilitado/deshabilitado;
- ACS eléctrico habilitado/deshabilitado;
- delta eléctrico habilitado/deshabilitado;
- setpoint de ACS;
- `heating_off`, que indica apagado manual de la caldera.

El setpoint debe publicarse tanto cuando cambia por MQTT como cuando cambia
desde el menú de la MEGA.

## Reglas de publicación

- Publicar solo ante un cambio real del valor.
- Mantener el ancho y los espacios definidos por el protocolo actual.
- No publicar bombas, contactores, caudal ni alarmas como variables adicionales
  si ya quedan representados por el estado de máquina o por el estado de alarma.
- Mantener la publicación periódica de respaldo: cada 4 minutos si corresponde
  por cambio y cada 90 minutos aunque el valor permanezca igual.
- No incorporar el modo invierno/verano en esta iteración.

## MEGA

- Centralizar la comparación entre valor anterior y valor actual.
- Generar la trama inmediatamente después de confirmar el nuevo valor local.
- Encolar la trama sin bloquear la máquina de estados ni las mediciones.
- Usar el protocolo actual con secuencia y ACK:

```text
MEGA → ESP: status:<sequence>:<payload>#
ESP → MEGA: ack:status:<sequence>#
```

- Mantener una sola trama de estado en vuelo, con timeout y reintentos.

## ESP

- Recibir la trama completa delimitada por `#`.
- Validar secuencia y payload antes de encolarlo para MQTT.
- Confirmar a la MEGA únicamente después de aceptar la trama localmente.
- Publicar en el topic MQTT correspondiente con payload exacto, incluyendo
  espacios significativos.
- Mantener los valores retenidos para que la nube pueda recuperarlos después
  de reconectar.
- Registrar publicación, descarte, cola llena y trama inválida.

## Validación

- Cambiar cada valor desde la MEGA y verificar publicación inmediata.
- Cambiar el setpoint desde el menú y verificar que la nube se actualice.
- Cambiar el setpoint por MQTT y verificar que la MEGA y la nube coincidan.
- Verificar `heating_off` en ambos sentidos.
- Confirmar que los payloads de estado y alarma mantengan dos caracteres.
- Confirmar que temperaturas y caudales mantengan su formato actual.
- Verificar que los cambios no generen tramas fusionadas ni publicaciones
  duplicadas.
- Verificar la publicación periódica de respaldo.

## Fuera de alcance

- Modo invierno/verano.
- `EnableFlowAlarm`.
- Control de la segunda caldera.
- OTA de la MEGA.
