#include "keyboard.h"

// BOTONES DE TECLADO (no se pueden modificar)
const int DI_Teclado_Arriba = 7;
const int DI_Teclado_Abajo = 5;
const int DI_Teclado_Enter = 6;
const int DI_Teclado_Atras = 4;

// Pin de interrupcion para funcionamiento del teclado
const int InterruptPin = 3;

volatile bool tecladoPendiente = false;
const unsigned long KEYBOARD_DEBOUNCE_MS = 150;

void keyboardSetup() {
    pinMode(DI_Teclado_Arriba, INPUT);
    pinMode(DI_Teclado_Abajo, INPUT);
    pinMode(DI_Teclado_Enter, INPUT);
    pinMode(DI_Teclado_Atras, INPUT);
    pinMode(InterruptPin, INPUT);
    attachInterrupt(1, AtencionTeclado, FALLING);
}

void procesarTeclado() {
    static unsigned long ultimoEvento = 0;

    if (!tecladoPendiente) {
        return;
    }

    noInterrupts();
    tecladoPendiente = false;
    interrupts();

    unsigned long ahora = millis();
    if (ahora - ultimoEvento < KEYBOARD_DEBOUNCE_MS) {
        return;
    }

    ultimoEvento = ahora;

    const bool botonArriba = digitalRead(DI_Teclado_Arriba) == LOW;
    const bool botonAbajo = digitalRead(DI_Teclado_Abajo) == LOW;
    const bool botonEnter = digitalRead(DI_Teclado_Enter) == LOW;
    const bool botonAtras = digitalRead(DI_Teclado_Atras) == LOW;

    if (botonEnter || botonAbajo || botonArriba || botonAtras) {
        Flag_Buzzer = true;
    }

    if (botonEnter) processMenuButton(BUTTON_ENTER);
    if (botonAbajo) processMenuButton(BUTTON_DOWN);
    if (botonArriba) processMenuButton(BUTTON_UP);
    if (botonAtras) processMenuButton(BUTTON_BACK);
}

void AtencionTeclado() {
    tecladoPendiente = true;
}
