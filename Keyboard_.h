
#ifndef kumekeyboard__
#define kumekeyboard__
#include "vars.h"
#include "Pinout.h"
#include "Menu.h"
#include "Configuration.h"
#include "BuzzerHandler.h"

class KumeKeyboard {
        private:
        static Pinout* pinoutConfiguration;
        static Configuration* configuration;
        static Menu* _menuActivo;
        static BuzzerHandler* buzzerHandler;

        static void handleKeyPress() // Función de Navegación entre Menús y Modificación de Parámetros
        {
                bool BotonArriba = digitalRead(pinoutConfiguration->DI_KEYBOARD_UP) == LOW;
                bool BotonAbajo = digitalRead(pinoutConfiguration->DI_KEYBOARD_DOWN) == LOW;
                bool BotonEnter = digitalRead(pinoutConfiguration->DI_KEYBOARD_ENTER) == LOW;
                bool BotonAtras = digitalRead(pinoutConfiguration->DI_KEYBOARD_BACK) == LOW;

                if (BotonEnter || BotonAbajo || BotonArriba || BotonAtras) {
                    buzzerHandler->beep(configuration->KEYBOARD_BEEP_LONG);
                }
                if (BotonEnter) {
                        _menuActivo = &(_menuActivo->enter());
                }
                if (BotonAbajo) {
                        _menuActivo = &(_menuActivo->down());
                }
                if (BotonArriba) {
                        _menuActivo = &(_menuActivo->up());
                }
                if (BotonAtras) {
                        _menuActivo = &(_menuActivo->escape());
                }
                _menuActivo->refresh();
        }

        void init() {
                pinMode(pinoutConfiguration->DI_KEYBOARD_UP, INPUT); // Definición de entradas y salidas
                pinMode(pinoutConfiguration->DI_KEYBOARD_DOWN, INPUT);
                pinMode(pinoutConfiguration->DI_KEYBOARD_ENTER, INPUT);
                pinMode(pinoutConfiguration->DI_KEYBOARD_BACK, INPUT);
                pinMode(pinoutConfiguration->InterruptPin, INPUT);
                // Asignación de Interrupciones (se define el número de la interrupción, no del pin; la rutina de interrupción y el modo de activación)
                attachInterrupt(configuration->KEYBOARD_INTERRUPT, handleKeyPress, FALLING); 
        }

        public:

        KumeKeyboard(Pinout* pinout, Menu* menuInicial, Configuration* config,
            BuzzerHandler* buzzerHandler) {
                this->pinoutConfiguration = pinout;
                this->_menuActivo = menuInicial;
                this->configuration = config;
                this->buzzerHandler = buzzerHandler;
                init();
        }

};

#endif
