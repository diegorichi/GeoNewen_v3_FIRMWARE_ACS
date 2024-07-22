#ifndef mqtt_cpp
#define mqtt_cpp
#include <Arduino.h>
#include <SoftwareSerial.h>

#include "alarm.h"
#include "machine_control.h"
#include "vars.h"

class SerialEsp8266 {
   private:
    // SoftwareSerial* _espSerial;
    HardwareSerial* _espSerial;
    unsigned int refresh_period = 350;  // 5 minutos
    /*
    this increment by 1 when we received 1
    1 is the mark that the status sent was proceced.
    So when this value = 1, then we push ACS temp
    So when this value = 2, then we push ACS on/off and so on
    */
    int what_should_be_pushed = 1;

    unsigned long Periodo_Refresco_Wifi = 0;

    String espWord = "";
    char espLetter;

    void handleProtocolWithEsp(String command) {
        if (command.indexOf("1") >= 0) {
            what_should_be_pushed += 1;
            if (what_should_be_pushed > 16) {
                what_should_be_pushed = 1;
            }
        } else if (command.indexOf("ACS_G:on") >= 0) {
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
        } else if (command.indexOf("ALARM_RESET") >= 0) {
            resetAlarms();
        } else if (command.indexOf("MODO_FRIO:on") >= 0) {
            modoFrio = true;
            changeModo();
        } else if (command.indexOf("MODO_FRIO:off") >= 0) {
            modoFrio = false;
            changeModo();
        } else if (command.indexOf("TEMP_ACS:") >= 0) {
            volatile uint8_t aux = command.substring(9, command.length() - 1).toInt();
            SetP_ACS = normalizeAcsTemp(&aux);
            SetP_ACS_Edit = SetP_ACS;
            EEPROMwrite(SetP_ACS_Address, SetP_ACS);
        }
    };

   public:
    // SerialEsp(int rx, int tx){
    //         espSerial = new SoftwareSerial(rx,tx);
    //         espSerial->begin(9600);
    // }

    SerialEsp8266(HardwareSerial* serialHardware) {
        this->_espSerial = serialHardware;
        this->_espSerial->begin(4800);
    }

    SerialEsp8266(HardwareSerial* serialHardware, long refresh_period_param) {
        this->_espSerial = serialHardware;
        this->_espSerial->begin(4800);
        this->refresh_period = refresh_period_param;
    }

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
    void sendStatusToEspSerial() {
        char buffer[26];
        char var_number[6];
        int i = 1;
        if (what_should_be_pushed == i) {
            sprintf(buffer, "contrl:ACS_GEO___:%s", EnableACS ? "1" : "0");
            _espSerial->println(buffer);
        }
        if (what_should_be_pushed == ++i) {
            sprintf(buffer, "contrl:ACS_DT_ELE:%s", EnableACS_DeltaElectrico ? "1" : "0");
            _espSerial->println(buffer);
        }
        if (what_should_be_pushed == ++i) {
            sprintf(buffer, "contrl:ACS_ELEC__:%s", EnableElectricACS ? "1" : "0");
            _espSerial->println(buffer);
        }
        if (what_should_be_pushed == ++i) {
            sprintf(buffer, "contrl:MODO_FRIO_:%s", modoFrio ? "1" : "0");
            _espSerial->println(buffer);
        }
        if (what_should_be_pushed == ++i) {
            dtostrf(Nro_Alarma, 2, 0, var_number);
            sprintf(buffer, "contrl:ALARMA____:%s", var_number);
            _espSerial->println(buffer);
        }

        if (what_should_be_pushed == ++i) {
            dtostrf(Temp_ACSacu, 4, 2, var_number);
            sprintf(buffer, "status:TEMP_ACS__:%s", var_number);
            _espSerial->println(buffer);
        }
        if (what_should_be_pushed == ++i) {
            sprintf(buffer, "status:STATE_____:%2d", Estado_Maquina);
            _espSerial->println(buffer);
        }
        if (what_should_be_pushed == ++i) {
            dtostrf(Caud_Hacu, 4, 0, var_number);
            sprintf(buffer, "status:CAU_HOGAR_:%s", var_number);
            _espSerial->println(buffer);
        }
        if (what_should_be_pushed == ++i) {
            dtostrf(Temp_in_Hacu, 4, 2, var_number);
            sprintf(buffer, "status:TEMP_IN_H_:%s", var_number);
            _espSerial->println(buffer);
        }
        if (what_should_be_pushed == ++i) {
            dtostrf(Temp_out_Hacu, 4, 2, var_number);
            sprintf(buffer, "status:TEMP_OUT_H:%s", var_number);
            _espSerial->println(buffer);
        }

        if (what_should_be_pushed == ++i) {
            dtostrf(Caud_Tacu, 4, 0, var_number);
            sprintf(buffer, "status:CAU_TIERRA:%s", var_number);
            _espSerial->println(buffer);
        }
        if (what_should_be_pushed == ++i) {
            dtostrf(Temp_in_T, 4, 2, var_number);
            sprintf(buffer, "status:TEMP_IN_T_:%s", var_number);
            _espSerial->println(buffer);
        }
        if (what_should_be_pushed == ++i) {
            dtostrf(Temp_out_T, 4, 2, var_number);
            sprintf(buffer, "status:TEMP_OUT_T:%s", var_number);
            _espSerial->println(buffer);
        }
        if (what_should_be_pushed == ++i) {
            dtostrf(Temp_Admision, 4, 2, var_number);
            sprintf(buffer, "status:TEMP_ADM__:%s", var_number);
            _espSerial->println(buffer);
        }
        if (what_should_be_pushed == ++i) {
            dtostrf(Temp_CompressorAcu, 4, 2, var_number);
            sprintf(buffer, "status:TEMP_COMP_:%s", var_number);
            _espSerial->println(buffer);
        }
        if (what_should_be_pushed == ++i) {
            dtostrf(Temp_Descargaacu, 4, 2, var_number);
            sprintf(buffer, "status:TEMP_DESC_:%s", var_number);
            _espSerial->println(buffer);
        }
    };

    // this should be called in main loop"
    void handleEspSerial() {
        if (_espSerial->available() > 0) {
            espLetter = _espSerial->read();
            espWord += espLetter;
            if (espLetter == '\n') {
                this->handleProtocolWithEsp(espWord);
                espWord = "";
            }
        }
        if (((millis() - this->Periodo_Refresco_Wifi) > this->refresh_period)) {
            this->sendStatusToEspSerial();
            this->Periodo_Refresco_Wifi = millis();
        }
    };
};

#endif
