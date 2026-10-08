#ifndef mqtt_cpp
#define mqtt_cpp
#include <Arduino.h>
#include <ArduinoQueue.h>
#include <arduino-timer.h>
#include <SoftwareSerial.h>

#include "alarm.h"
#include "machine_control.h"
#include "stateMachine.h"
#include "vars.h"

constexpr uint8_t QUEUE_SIZE_ITEMS = 32;
constexpr uint8_t ESP_MESSAGE_SIZE = 64;
constexpr unsigned long STATUS_ACK_TIMEOUT_MS = 2000;
constexpr uint8_t STATUS_MAX_ATTEMPTS = 3;

struct EspMessage {
    char text[ESP_MESSAGE_SIZE];
};

ArduinoQueue<EspMessage> espQueue(QUEUE_SIZE_ITEMS);

EspMessage pendingEspMessage{};
uint16_t pendingStatusSequence = 0;
uint8_t pendingStatusAttempts = 0;
unsigned long pendingStatusSentAt = 0;
bool pendingStatusActive = false;

bool enqueueEspMessage(const char* message) {
    EspMessage item{};
    strncpy(item.text, message, ESP_MESSAGE_SIZE - 1);
    item.text[ESP_MESSAGE_SIZE - 1] = '\0';
    return espQueue.enqueue(item);
}

bool sendToSerial(HardwareSerial* espSerial) {
    wdt_reset();

    if (pendingStatusActive) {
        if (millis() - pendingStatusSentAt < STATUS_ACK_TIMEOUT_MS) {
            return true;
        }

        if (pendingStatusAttempts >= STATUS_MAX_ATTEMPTS) {
            GEO_LOG_PRINT("status descartado sin ACK: ");
            GEO_LOG_PRINTLN(pendingStatusSequence);
            pendingStatusActive = false;
            return true;
        }

        GEO_LOG_PRINT("reintentando status (intento ");
        GEO_LOG_PRINT(pendingStatusAttempts + 1);
        GEO_LOG_PRINT("): ");
        GEO_LOG_PRINTLN(pendingEspMessage.text);
        espSerial->print(pendingEspMessage.text);
        pendingStatusAttempts++;
        pendingStatusSentAt = millis();
        return true;
    }

    if (!espQueue.isEmpty()) {
        pendingEspMessage = espQueue.dequeue();
        String message = String(pendingEspMessage.text);
        int separator = message.indexOf(':', 7);

        if (!message.startsWith("status:") || separator <= 7) {
            GEO_LOG_PRINT("trama descartada antes de enviar: ");
            GEO_LOG_PRINTLN(message);
            return true;
        }

        String sequence = message.substring(7, separator);
        const char* sequenceText = sequence.c_str();
        for (const char* character = sequenceText; *character != '\0'; ++character) {
            if (!isDigit(*character)) {
                GEO_LOG_PRINT("trama descartada antes de enviar: ");
                GEO_LOG_PRINTLN(message);
                return true;
            }
        }

        pendingStatusSequence = sequence.toInt();
        pendingStatusAttempts = 1;
        pendingStatusActive = true;
        pendingStatusSentAt = millis();

        GEO_LOG_PRINT("message deque para esp:");
        GEO_LOG_PRINTLN(pendingEspMessage.text);
        espSerial->print(pendingEspMessage.text);
    }

    return true;
}

char bufferAcsGeo[26];
char bufferAcsDtEle[26];
char bufferAcsElec[26];
char bufferHeatingOff[26];
char bufferThermostat[26];
char bufferModoFrio[26];
char bufferAlarma[26];
char bufferTempAcs[26];
char bufferTempAcsDes[26];
char bufferTempAcsSet[26];
char bufferStateMach[26];
char bufferCauHogar[26];
char bufferTempInH[26];
char bufferTempOutH[26];
char bufferCauTierra[26];
char bufferTempInT[26];
char bufferTempOutT[26];
char bufferTempAdm[26];
char bufferTempComp[26];
char bufferTempDesc[26];

class SerialEsp8266 {
    static const int SLIDING_BUFFER_LEN = 48;

   private:
    // SoftwareSerial* _espSerial;
    HardwareSerial* _espSerial;

    Timer<1, &millis, HardwareSerial*> timerSendToEsp;

    unsigned long refreshPeriod = 240000;  // 4min

    unsigned long periodRefreshWifi = 0;

    char slidingBuffer[SLIDING_BUFFER_LEN + 1];
    uint8_t slidingBufferIndex = 0;
    bool slidingBufferOverflow = false;
    uint16_t nextStatusSequence = 1;
    uint16_t lastCommandSequence = 0;

    bool statusSnapshotInitialized = false;
    int lastPublishedMachineState = 0;
    bool lastPublishedThermostatState = false;
    bool lastPublishedAcsEnabled = false;
    bool lastPublishedAcsDeltaEnabled = false;
    bool lastPublishedAcsElectricEnabled = false;
    bool lastPublishedHeatingOff = false;
    bool lastPublishedColdMode = false;
    uint8_t lastPublishedAcsDesired = 0;
    uint8_t lastPublishedAcsSet = 0;

    void clearBuffer() {
        for (int i = 0; i <= SLIDING_BUFFER_LEN; i++) {
            slidingBuffer[i] = '\0';
        }
        slidingBufferIndex = 0;
        slidingBufferOverflow = false;
    }

    void handleProtocolWithEsp() {
        String command = String(slidingBuffer);
        wdt_reset();
        GEO_LOG_PRINT("Handle message from esp:");
        GEO_LOG_PRINTLN(command);

        if (command.indexOf("STATUS:publish") >= 0) {
            enqueueStatusToSend();
        } else if (command.indexOf("ACS_G:on") >= 0) {
            enableAcs = true;
            eepromWrite(enableAcsAddress, enableAcs);
        } else if (command.indexOf("ACS_G:off") >= 0) {
            enableAcs = false;
            eepromWrite(enableAcsAddress, enableAcs);
        } else if (command.indexOf("ACS_DT_E:on") >= 0) {
            enableAcsDeltaElectrico = true;
            eepromWrite(enableAcsDeltaElectricoAddress, enableAcsDeltaElectrico);
        } else if (command.indexOf("ACS_DT_E:off") >= 0) {
            enableAcsDeltaElectrico = false;
            eepromWrite(enableAcsDeltaElectricoAddress, enableAcsDeltaElectrico);
        } else if (command.indexOf("ACS_E:on") >= 0) {
            enableElectricAcs = true;
            eepromWrite(enableElectricAcsAddress, enableElectricAcs);
        } else if (command.indexOf("ACS_E:off") >= 0) {
            enableElectricAcs = false;
            eepromWrite(enableElectricAcsAddress, enableElectricAcs);
        } else if (command.indexOf("HEATING_OFF:on") >= 0) {
            heatingOff = true;
            eepromWrite(heatingOffAddress, heatingOff);
        } else if (command.indexOf("HEATING_OFF:off") >= 0) {
            heatingOff = false;
            eepromWrite(heatingOffAddress, heatingOff);
        } else if (command.indexOf("ALARM:reset") >= 0) {
            resetAlarms();
            estadoMaquina = 0;
        } else if (command.indexOf("MODO_FRIO:on") >= 0) {
            estadoMaquina = 0;
            stateMachine0();  // setea para detener la maquina y
            // retorna estado_maquina 1, requerido para cambio de modo.
            // cambio de modo de forma segura.
            changeModo(true);
        } else if (command.indexOf("MODO_FRIO:off") >= 0) {
            estadoMaquina = 0;
            stateMachine0();  // setea para detiener la maquina y
            // sale estado_maquina 1, requerido para cambio de modo.
            changeModo(false);
        } else if (command.indexOf("TEMP_ACS:") >= 0) {
            int start = command.indexOf("TEMP_ACS:") + 9;  // 9 caracteres tiene "TEMP_ACS:"
            volatile uint8_t aux = command.substring(start, start + 2).toInt();
            acsSetpoint = normalizeAcsTemp(&aux);
            acsSetpointEdit = acsSetpoint;
            eepromWrite(acsSetpointAddress, acsSetpoint);
        }
    };

    void sendCommandAck(const String& sequence) {
        _espSerial->print("ack:cmd:");
        _espSerial->print(sequence);
        _espSerial->print('#');
    }

    void enqueueStatusFrame(const char* payload) {
        char frame[ESP_MESSAGE_SIZE];
        snprintf(frame, sizeof(frame), "status:%u:%s", nextStatusSequence++, payload);
        enqueueEspMessage(frame);
    }

    /*
    value from index: 18
    contrl:ACS_GEO___:1;
    contrl:ACS_DT_ELE:1,
    contrl:ACS_ELEC__:1;
    contrl:HEATING_OFF:1;
    contrl:TERMOSTATO:1;
    status:TEMP_ACS__:000000;
    status:TEMP_ACS_DES:00;
    status:TEMP_ACS_SET:00;
    contrl:MODO_FRIO_:0;
    status:STATE_____:0000;
    status:CAU_HOGAR_:0000;
    status:TEMP_IN_H_:000000;
    status:TEMP_OUT_H:000000;
    status:CAU_TIERRA:0000;
    status:TEMP_IN_T_:000000;
    status:TEMP_OUT_T:000000;
    status:ALARMA____:0000;
    status:TEMP_ADM__:000000;
    status:TEMP_COMP_:000000;
    status:TEMP_DESC_:000000
    */
    void enqueueStatusToSend() {
        char varNumber[6];
        wdt_reset();

        GEO_LOG_PRINTLN("enqueue status to send to esp");

        sprintf(bufferAcsGeo, "contrl:ACS_GEO___:%s#", enableAcs ? "1" : "0");
        enqueueStatusFrame(bufferAcsGeo);

        sprintf(bufferAcsDtEle, "contrl:ACS_DT_ELE:%s#", enableAcsDeltaElectrico ? "1" : "0");
        enqueueStatusFrame(bufferAcsDtEle);

        sprintf(bufferAcsElec, "contrl:ACS_ELEC__:%s#", enableElectricAcs ? "1" : "0");
        enqueueStatusFrame(bufferAcsElec);

        sprintf(bufferHeatingOff, "contrl:HEATING_OFF:%s#", heatingOff ? "1" : "0");
        enqueueStatusFrame(bufferHeatingOff);

        sprintf(bufferThermostat, "contrl:TERMOSTATO:%s#", senalStart ? "1" : "0");
        enqueueStatusFrame(bufferThermostat);

        sprintf(bufferModoFrio, "contrl:MODO_FRIO_:%s#", modoFrio ? "1" : "0");
        enqueueStatusFrame(bufferModoFrio);

        dtostrf(nroAlarma, 2, 0, varNumber);
        sprintf(bufferAlarma, "contrl:ALARMA____:%s#", varNumber);
        enqueueStatusFrame(bufferAlarma);

        dtostrf(tempAcsAcu, 4, 2, varNumber);
        sprintf(bufferTempAcs, "status:TEMP_ACS__:%s#", varNumber);
        enqueueStatusFrame(bufferTempAcs);

        sprintf(bufferTempAcsDes, "status:TEMP_ACS_DES:%02u#", acsSetpointEdit);
        enqueueStatusFrame(bufferTempAcsDes);

        sprintf(bufferTempAcsSet, "status:TEMP_ACS_SET:%02u#", acsSetpoint);
        enqueueStatusFrame(bufferTempAcsSet);

        sprintf(bufferStateMach, "status:STATE_MACH:%2d#", estadoMaquina);
        enqueueStatusFrame(bufferStateMach);

        dtostrf(caudHacu, 4, 0, varNumber);
        sprintf(bufferCauHogar, "status:CAU_HOGAR_:%s#", varNumber);
        enqueueStatusFrame(bufferCauHogar);

        dtostrf(tempInHacu, 4, 2, varNumber);
        sprintf(bufferTempInH, "status:TEMP_IN_H_:%s#", varNumber);
        enqueueStatusFrame(bufferTempInH);

        dtostrf(tempOutHacu, 4, 2, varNumber);
        sprintf(bufferTempOutH, "status:TEMP_OUT_H:%s#", varNumber);
        enqueueStatusFrame(bufferTempOutH);

        dtostrf(caudTacu, 4, 0, varNumber);
        sprintf(bufferCauTierra, "status:CAU_TIERRA:%s#", varNumber);
        enqueueStatusFrame(bufferCauTierra);

        dtostrf(tempInT, 4, 2, varNumber);
        sprintf(bufferTempInT, "status:TEMP_IN_T_:%s#", varNumber);
        enqueueStatusFrame(bufferTempInT);

        dtostrf(tempOutT, 4, 2, varNumber);
        sprintf(bufferTempOutT, "status:TEMP_OUT_T:%s#", varNumber);
        enqueueStatusFrame(bufferTempOutT);

        dtostrf(tempAdmision, 4, 2, varNumber);
        sprintf(bufferTempAdm, "status:TEMP_ADM__:%s#", varNumber);
        enqueueStatusFrame(bufferTempAdm);

        dtostrf(tempCompressorAcu, 4, 2, varNumber);
        sprintf(bufferTempComp, "status:TEMP_COMP_:%s#", varNumber);
        enqueueStatusFrame(bufferTempComp);

        dtostrf(tempDescargaAcu, 4, 2, varNumber);
        sprintf(bufferTempDesc, "status:TEMP_DESC_:%s#", varNumber);
        enqueueStatusFrame(bufferTempDesc);
        GEO_LOG_PRINTLN("finished: enqueue status to send to esp");
    };

    bool discreteStatusChanged() {
        const bool changed = !statusSnapshotInitialized ||
            lastPublishedMachineState != estadoMaquina ||
            lastPublishedThermostatState != senalStart ||
            lastPublishedAcsEnabled != enableAcs ||
            lastPublishedAcsDeltaEnabled != enableAcsDeltaElectrico ||
            lastPublishedAcsElectricEnabled != enableElectricAcs ||
            lastPublishedHeatingOff != heatingOff ||
            lastPublishedColdMode != modoFrio ||
            lastPublishedAcsDesired != acsSetpointEdit ||
            lastPublishedAcsSet != acsSetpoint;

        lastPublishedMachineState = estadoMaquina;
        lastPublishedThermostatState = senalStart;
        lastPublishedAcsEnabled = enableAcs;
        lastPublishedAcsDeltaEnabled = enableAcsDeltaElectrico;
        lastPublishedAcsElectricEnabled = enableElectricAcs;
        lastPublishedHeatingOff = heatingOff;
        lastPublishedColdMode = modoFrio;
        lastPublishedAcsDesired = acsSetpointEdit;
        lastPublishedAcsSet = acsSetpoint;
        statusSnapshotInitialized = true;
        return changed;
    }

   public:
    SerialEsp8266(HardwareSerial* serialHardware) {
        this->_espSerial = serialHardware;
        this->_espSerial->begin(4800);
        this->_espSerial->setTimeout(300);
        this->clearBuffer();
        timerSendToEsp.every(100, sendToSerial, this->_espSerial);
    }

    // this should be called in main loop"
    void handleEspSerial() {
        wdt_reset();
        while (_espSerial->available() > 0) {
            char c = _espSerial->read();

            if (isAlphaNumeric(c) || c == ':' || c == '_' || c == '#') {
                if (c == '#') {
                    if (!slidingBufferOverflow && slidingBufferIndex > 0) {
                        String command = String(slidingBuffer);
                        if (command.startsWith("cmd:")) {
                            int separator = command.indexOf(':', 4);
                            if (separator > 4) {
                                String sequence = command.substring(4, separator);
                                const char* sequenceText = sequence.c_str();
                                bool validSequence = sequenceText[0] != '\0';
                                for (const char* character = sequenceText; *character != '\0'; ++character) {
                                    if (!isDigit(*character)) {
                                        validSequence = false;
                                        break;
                                    }
                                }
                                if (validSequence) {
                                    String payload = command.substring(separator + 1);
                                    if (sequence.toInt() != lastCommandSequence) {
                                        payload.toCharArray(slidingBuffer, SLIDING_BUFFER_LEN + 1);
                                        this->handleProtocolWithEsp();
                                        lastCommandSequence = sequence.toInt();
                                    } else {
                                        GEO_LOG_PRINT("Comando duplicado, solo ACK: ");
                                        GEO_LOG_PRINTLN(sequence);
                                    }
                                    this->sendCommandAck(sequence);
                                }
                            }
                        } else if (command.startsWith("ack:status:")) {
                            uint16_t sequence = command.substring(11).toInt();
                            GEO_LOG_PRINT("ACK status recibido: ");
                            GEO_LOG_PRINTLN(sequence);

                            if (pendingStatusActive && sequence == pendingStatusSequence) {
                                pendingStatusActive = false;
                            }
                        }
                    }
                    this->clearBuffer();
                } else {
                    if (slidingBufferIndex < SLIDING_BUFFER_LEN) {
                        slidingBuffer[slidingBufferIndex++] = c;
                        slidingBuffer[slidingBufferIndex] = '\0';
                    } else {
                        // Descarta la trama completa hasta encontrar '#'.
                        slidingBufferOverflow = true;
                    }
                }
            }
        }
        if (((millis() - this->periodRefreshWifi) > this->refreshPeriod)) {
            this->enqueueStatusToSend();
            this->periodRefreshWifi = millis();
        }
        timerSendToEsp.tick();
    };

    void detectAndEnqueueChangedStatus() {
        if (discreteStatusChanged()) {
            enqueueStatusToSend();
        }
    }
};

#endif
