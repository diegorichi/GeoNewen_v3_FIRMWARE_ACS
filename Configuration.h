
#ifndef configuration__
#define configuration__

class Configuration {
        public:
        const uint16_t BEEP_LONG = 500;

        const uint16_t KEYBOARD_BEEP_LONG = 300;
        const uint8_t KEYBOARD_INTERRUPT = 1;

        const uint16_t DELAY_SYSTEM_HEALT_CHECK = 5000;

        //30 segs
        const uint16_t DELAY_START_COMPRESSOR = 30000;

        //1 DAY
        const uint32_t DONT_STUCK_PUMPS_PERIOD = 86400000;

        //10 segs
        const uint16_t DONT_STUCK_PUMPS_DURATION = 10000;

        //2 min
        const uint32_t DELAY_START_HEATING = 120000;

        //6 min
        const uint32_t TAKE_REST_DURATION = 400000;

        const uint8_t MAX_ACS = 48;
        const uint8_t MIN_ACS = 30;
        const uint8_t GAP_ACS = 2;
        const uint8_t DELTA_ACS_ELECTRICO = 7;

        const float MAX_TEMP_OUT_H = 49.0;
        const float MIN_TEMP_OUT_H = 10.0;
        const float MAX_TEMP_OUT_T = 40.0;
        const float MIN_TEMP_OUT_T = -6.0;
        const float MIN_TEMP_ADMISION = -7.0;


};
#endif
