#include "keyboard.h"

// BOTONES DE TECLADO (no se pueden modificar)
const int diTecladoArriba = 7;
const int diTecladoAbajo = 5;
const int diTecladoEnter = 6;
const int diTecladoAtras = 4;

// Pin de interrupcion para funcionamiento del teclado
const int interruptPin = 3;

volatile bool tecladoPendiente = false;
const unsigned long KEYBOARD_DEBOUNCE_MS = 150;

void keyboardSetup() {
    pinMode(diTecladoArriba, INPUT);
    pinMode(diTecladoAbajo, INPUT);
    pinMode(diTecladoEnter, INPUT);
    pinMode(diTecladoAtras, INPUT);
    pinMode(interruptPin, INPUT);
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

    const bool botonArriba = digitalRead(diTecladoArriba) == LOW;
    const bool botonAbajo = digitalRead(diTecladoAbajo) == LOW;
    const bool botonEnter = digitalRead(diTecladoEnter) == LOW;
    const bool botonAtras = digitalRead(diTecladoAtras) == LOW;

    if (botonEnter || botonAbajo || botonArriba || botonAtras) {
        flagBuzzer = true;
    }

    if (botonEnter) processMenuButton(BUTTON_ENTER);
    if (botonAbajo) processMenuButton(BUTTON_DOWN);
    if (botonArriba) processMenuButton(BUTTON_UP);
    if (botonAtras) processMenuButton(BUTTON_BACK);
}

void AtencionTeclado() {
    tecladoPendiente = true;
}
