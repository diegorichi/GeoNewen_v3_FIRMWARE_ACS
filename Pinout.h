
#ifndef kume_pinout__
#define kume_pinout__
#include <Arduino.h>

class Pinout {
        public:
        const int DO_CALENTADOR = 23;     // Compresor
        const int DO_COMPRESSOR = 25;     // Boombas de circulacion
        const int DO_BOMBAS = 27;         // Valvula Calefaccion
        const int DO_VALVULA_4_VIAS = 29; // V4V
        const int DO_VALVULA_ACS = 31;    // V ACS

        const int DO_TRIAC = 11;  // Triacs,Pin salida PWM (no se puede modificar)
        const int DO_BUZZER = 12; //

        const int DI_CAUDAL_TIERRA = 18; // ENTRADAS DE CAUDALIMETROS (no se pueden modificar)
        const int DI_CAUDAL_HOGAR = 19;
        const int DI_MARCHA = 33;       // Entrada de señal de Marcha
        const int DI_PRESSURE_HI = 35;  // Preostato de alta
        const int DI_PRESSURE_LOW = 37; // Presotato de baja

        const uint8_t DI_TEMP_IN_HOGAR [8] = { 0x28, 0xDA, 0xB6, 0xF8, 0x1A, 0x19, 0x01, 0x8B };      // n1    ok
        const uint8_t DI_TEMP_COMPRESSOR [8] = { 0x28, 0xE5, 0xAC, 0x26, 0x1B, 0x19, 0x01, 0x3C }; // n2    ok
        const uint8_t DI_TEMP_AADMISION [8] = { 0x28, 0x34, 0x76, 0x57, 0x1A, 0x19, 0x01, 0xA0 };  // n10   ok
        const uint8_t DI_TEMP_OUT_T [8] = { 0x28, 0xD6, 0x3C, 0xE0, 0x1B, 0x19, 0x01, 0x0F };     // n7   ok

        const uint8_t DI_TEMP_DESCARGA [8] = { 0x28, 0x49, 0x4B, 0x01, 0x1C, 0x19, 0x01, 0x1A }; // n11
        const uint8_t DI_TEMP_ACS [8] = { 0x28, 0xAE, 0x16, 0xFF, 0x1B, 0x19, 0x01, 0xD1 };      // n12
        const uint8_t DI_TEMP_OUT_H [8] = { 0x28, 0x87, 0x9F, 0xE9, 0x1B, 0x19, 0x01, 0xF8 };    // n13
        const uint8_t DI_TEMP_IN_T [8] = { 0x28, 0x83, 0x69, 0x3D, 0x1B, 0x19, 0x01, 0x11 };     // n14


        const int DI_KEYBOARD_UP = 7; // BOTONES DE TECLADO (no se pueden modificar)
        const int DI_KEYBOARD_DOWN = 5;
        const int DI_KEYBOARD_ENTER = 6;
        const int DI_KEYBOARD_BACK = 4;

        const int InterruptPin = 3; // Pin de interrupcion para funcionamiento del teclado


        void setupDigitalInputs() {
                pinMode(DI_MARCHA, INPUT);
                pinMode(DI_PRESSURE_HI, INPUT);
                pinMode(DI_PRESSURE_LOW, INPUT);
                pinMode(DI_CAUDAL_TIERRA, INPUT);
                pinMode(DI_CAUDAL_HOGAR, INPUT);
        }

        void setupDigitalOuputs() {

                pinMode(DO_COMPRESSOR, OUTPUT);
                pinMode(DO_VALVULA_4_VIAS, OUTPUT);
                pinMode(DO_BOMBAS, OUTPUT);
                pinMode(DO_CALENTADOR, OUTPUT);
                pinMode(DO_VALVULA_ACS, OUTPUT);

                pinMode(DO_TRIAC, OUTPUT);
                pinMode(DO_BUZZER, OUTPUT);

                pinMode(DIR, OUTPUT);
                pinMode(STEP, OUTPUT);
                pinMode(ED_ENABLE, OUTPUT);
        }

        void initializeDigitalOuputs() {
                digitalWrite(ED_ENABLE, LOW); // Inicialización de salidas
                digitalWrite(DO_COMPRESSOR, LOW);
                digitalWrite(DO_VALVULA_ACS, LOW); // Inicia con la valvula de loza encendida
                digitalWrite(DO_VALVULA_4_VIAS, LOW);
                digitalWrite(DO_BOMBAS, LOW);
                digitalWrite(DO_CALENTADOR, LOW);
                digitalWrite(DO_TRIAC, LOW);
                digitalWrite(DO_BUZZER, LOW);
        }

        Pinout() {
                setupDigitalInputs();
                setupDigitalOuputs();
                initializeDigitalOuputs();
        }

};

#endif
