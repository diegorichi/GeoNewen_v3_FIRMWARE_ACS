#ifndef mqtt_cpp
#define mqtt_cpp
#include <Arduino.h>
#include <SoftwareSerial.h>

#include "alarm.h"
#include "machine_control.h"
#include "stateMachine.h"
#include "vars.h"

#include <ArduinoQueue.h>

#define QUEUE_SIZE_ITEMS 16
ArduinoQueue<char*> espQueue(QUEUE_SIZE_ITEMS);

bool sendToSerial(HardwareSerial* espSerial) {
    if (!espQueue.isEmpty()){
        wdt_reset();
        char* message = espQueue.dequeue();
        Serial.print("message deque para esp:");
        Serial.println(message);
        espSerial->print(message);
    }
    Serial.println("returning new schedule in a few segs");
    return true;
}

char buffer_ACS_GEO___[26];
char buffer_ACS_DT_ELE[26];
char buffer_ACS_ELEC__[26];
char buffer_MODO_FRIO_[26];
char buffer_ALARMA____[26];
char buffer_TEMP_ACS__[26];
char buffer_STATE_MACH[26];
char buffer_CAU_HOGAR_[26];
char buffer_TEMP_IN_H_[26];
char buffer_TEMP_OUT_H[26];
char buffer_CAU_TIERRA[26];
char buffer_TEMP_IN_T_[26];
char buffer_TEMP_OUT_T[26];
char buffer_TEMP_ADM__[26];
char buffer_TEMP_COMP_[26];
char buffer_TEMP_DESC_[26];

class SerialEsp8266 {
    static const int SLIDING_BUFFER_LEN = 20;

   private:
    // SoftwareSerial* _espSerial;
    HardwareSerial* _espSerial;
    
    Timer<1, &millis, HardwareSerial*> timerSendToEsp;

    unsigned long refresh_period = 300000; // 5min

    unsigned long period_refresh_wifi = 0;

    char slidingBuffer[SLIDING_BUFFER_LEN];

    void clearBuffer() {
        for (int i = 0; i < SLIDING_BUFFER_LEN; i++) {
          slidingBuffer[i] = 0;
        }
    }

    void handleProtocolWithEsp() {

        String command = String(slidingBuffer);
        wdt_reset();
        Serial.print("Handle message from esp:");
        Serial.println(command);

        if (command.indexOf("ACS_G:on") >= 0) {
            EnableACS = true;
            EEPROMwrite(EnableACS_Address, EnableACS);
        } else if (command.indexOf("ACS_G:off") >= 0) {
            EnableACS = false;
            EEPROMwrite(EnableACS_Address, EnableACS);
        } else if (command.indexOf("ACS_DT_E:on") >= 0) {
            EnableACS_DeltaElectrico = true;
            EEPROMwrite(EnableACS_DeltaElectrico_Address, EnableACS_DeltaElectrico);
        } else if (command.indexOf("ACS_DT_E:off") >= 0) {
            EnableACS_DeltaElectrico = false;
            EEPROMwrite(EnableACS_DeltaElectrico_Address, EnableACS_DeltaElectrico);
        } else if (command.indexOf("ACS_E:on") >= 0) {
            EnableElectricACS = true;
            EEPROMwrite(EnableElectricACS_Address, EnableElectricACS);
        } else if (command.indexOf("ACS_E:off") >= 0) {
            EnableElectricACS = false;
            EEPROMwrite(EnableElectricACS_Address, EnableElectricACS);
        } else if (command.indexOf("ALARM:reset") >= 0) {
            resetAlarms();
        } else if (command.indexOf("MODO_FRIO:on") >= 0) {
            Estado_Maquina = 0;
            stateMachine0(); //setea para detener la maquina y 
            // retorna estado_maquina 1, requerido para cambio de modo.
            // cambio de modo de forma segura.
            changeModo(true);
        } else if (command.indexOf("MODO_FRIO:off") >= 0) {
            Estado_Maquina = 0;
            stateMachine0(); //setea para detiener la maquina y 
            // sale estado_maquina 1, requerido para cambio de modo.
            changeModo(false);
        } else if (command.indexOf("TEMP_ACS:") >= 0) {
            volatile uint8_t aux = command.substring(9, command.length()).toInt();
            SetP_ACS = normalizeAcsTemp(&aux);
            SetP_ACS_Edit = SetP_ACS;
            EEPROMwrite(SetP_ACS_Address, SetP_ACS);
        }
    };

    /*
    value from index: 18
    contrl:ACS_GEO___:1;
    contrl:ACS_DT_ELE:1,
    contrl:ACS_ELEC__:1;
    status:TEMP_ACS__:000000;
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
        char var_number[6];
        wdt_reset();

        Serial.println("enqueue status to send to esp");

        sprintf(buffer_ACS_GEO___, "contrl:ACS_GEO___:%s#", EnableACS ? "1" : "0");
        espQueue.enqueue(buffer_ACS_GEO___);

        sprintf(buffer_ACS_DT_ELE, "contrl:ACS_DT_ELE:%s#", EnableACS_DeltaElectrico ? "1" : "0");
        espQueue.enqueue(buffer_ACS_DT_ELE);

        sprintf(buffer_ACS_ELEC__, "contrl:ACS_ELEC__:%s#", EnableElectricACS ? "1" : "0");
        espQueue.enqueue(buffer_ACS_ELEC__);

        sprintf(buffer_MODO_FRIO_, "contrl:MODO_FRIO_:%s#", modoFrio ? "1" : "0");
        espQueue.enqueue(buffer_MODO_FRIO_);

        dtostrf(Nro_Alarma, 2, 0, var_number);
        sprintf(buffer_ALARMA____, "contrl:ALARMA____:%s#", var_number);
        espQueue.enqueue(buffer_ALARMA____);

        dtostrf(Temp_ACSacu, 4, 2, var_number);
        sprintf(buffer_TEMP_ACS__, "status:TEMP_ACS__:%s#", var_number);
        espQueue.enqueue(buffer_TEMP_ACS__);

        sprintf(buffer_STATE_MACH, "status:STATE_MACH:%2d#", Estado_Maquina);
        espQueue.enqueue(buffer_STATE_MACH);

        dtostrf(Caud_Hacu, 4, 0, var_number);
        sprintf(buffer_CAU_HOGAR_, "status:CAU_HOGAR_:%s#", var_number);
        espQueue.enqueue(buffer_CAU_HOGAR_);

        dtostrf(Temp_in_Hacu, 4, 2, var_number);
        sprintf(buffer_TEMP_IN_H_, "status:TEMP_IN_H_:%s#", var_number);
        espQueue.enqueue(buffer_TEMP_IN_H_);

        dtostrf(Temp_out_Hacu, 4, 2, var_number);
        sprintf(buffer_TEMP_OUT_H, "status:TEMP_OUT_H:%s#", var_number);
        espQueue.enqueue(buffer_TEMP_OUT_H);

        dtostrf(Caud_Tacu, 4, 0, var_number);
        sprintf(buffer_CAU_TIERRA, "status:CAU_TIERRA:%s#", var_number);
        espQueue.enqueue(buffer_CAU_TIERRA);

        dtostrf(Temp_in_T, 4, 2, var_number);
        sprintf(buffer_TEMP_IN_T_, "status:TEMP_IN_T_:%s#", var_number);
        espQueue.enqueue(buffer_TEMP_IN_T_);

        dtostrf(Temp_out_T, 4, 2, var_number);
        sprintf(buffer_TEMP_OUT_T, "status:TEMP_OUT_T:%s#", var_number);
        espQueue.enqueue(buffer_TEMP_OUT_T);

        dtostrf(Temp_Admision, 4, 2, var_number);
        sprintf(buffer_TEMP_ADM__, "status:TEMP_ADM__:%s#", var_number);
        espQueue.enqueue(buffer_TEMP_ADM__);

        dtostrf(Temp_CompressorAcu, 4, 2, var_number);
        sprintf(buffer_TEMP_COMP_, "status:TEMP_COMP_:%s#", var_number);
        espQueue.enqueue(buffer_TEMP_COMP_);

        dtostrf(Temp_Descargaacu, 4, 2, var_number);
        sprintf(buffer_TEMP_DESC_, "status:TEMP_DESC_:%s#", var_number);
        espQueue.enqueue(buffer_TEMP_DESC_);
        Serial.println("finished: enqueue status to send to esp");
    };

    public:

    SerialEsp8266(HardwareSerial* serialHardware) {
        this->_espSerial = serialHardware;
        this->_espSerial->begin(4800);
        this->_espSerial->setTimeout(300);
        this->clearBuffer();
        timerSendToEsp.every(5000, sendToSerial, this->_espSerial);
    }

    // this should be called in main loop"
    void handleEspSerial() {
        wdt_reset();
        if (_espSerial->available() > 0) {
            char c = _espSerial->read();

            if (isAlphaNumeric(c) || c == ':' || c == '_' || c == '#') {
                if (c == '#') {
                    this->handleProtocolWithEsp();
                    this->clearBuffer();
                    this->enqueueStatusToSend();
                } else {
                    // Shift buffer
                    for (int i = 0; i < SLIDING_BUFFER_LEN - 1; i++) {
                        slidingBuffer[i] = slidingBuffer[i + 1];
                    }                    
                    // add char
                    slidingBuffer[SLIDING_BUFFER_LEN - 1] = c;
                }
            }
        }
        if (((millis() - this->period_refresh_wifi) > this->refresh_period)) {
            this->enqueueStatusToSend();
            this->period_refresh_wifi = millis();
        }
        timerSendToEsp.tick();
    };
};

#endif
