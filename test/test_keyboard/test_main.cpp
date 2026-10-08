#include <unity.h>
#include "keyboard.cpp"

unsigned long fakeMillisNow = 0;
int fakeDigitalInputs[64] = {};
volatile MenuId menuActual = MENU_HOME;
volatile bool flagBuzzer = false;
int menu_button_calls = 0;
MenuButton last_button = BUTTON_UP;

unsigned long millis() { return fakeMillisNow; }
int digitalRead(int pin) { return fakeDigitalInputs[pin]; }
void pinMode(int, int) {}
void noInterrupts() {}
void interrupts() {}
void attachInterrupt(int, void (*)(), int) {}
void processMenuButton(MenuButton button) { ++menu_button_calls; last_button = button; }

void resetFixtures() {
    for (int& value : fakeDigitalInputs) value = HIGH;
    fakeMillisNow = 0;
    tecladoPendiente = false;
    flagBuzzer = false;
    menu_button_calls = 0;
}

void test_keyboard_debounces_and_dispatches_buttons(void) {
    resetFixtures();
    keyboardSetup();
    fakeDigitalInputs[diTecladoEnter] = LOW;
    fakeMillisNow = 151;
    AtencionTeclado();
    procesarTeclado();
    TEST_ASSERT_EQUAL(1, menu_button_calls);
    TEST_ASSERT_EQUAL(BUTTON_ENTER, last_button);
    TEST_ASSERT_TRUE(flagBuzzer);

    fakeMillisNow = 200;
    AtencionTeclado();
    procesarTeclado();
    TEST_ASSERT_EQUAL(1, menu_button_calls);

    fakeMillisNow = 302;
    AtencionTeclado();
    procesarTeclado();
    TEST_ASSERT_EQUAL(2, menu_button_calls);
}

void test_keyboard_ignores_pending_event_without_pressed_button(void) {
    resetFixtures();
    fakeMillisNow = 500;
    AtencionTeclado();
    procesarTeclado();
    TEST_ASSERT_EQUAL(0, menu_button_calls);
    TEST_ASSERT_FALSE(flagBuzzer);
}

void test_keyboard_dispatches_each_button(void) {
    resetFixtures();
    const int pins[] = {diTecladoArriba, diTecladoAbajo, diTecladoEnter, diTecladoAtras};
    const MenuButton buttons[] = {BUTTON_UP, BUTTON_DOWN, BUTTON_ENTER, BUTTON_BACK};
    for (int i = 0; i < 4; ++i) {
        resetFixtures();
        fakeDigitalInputs[pins[i]] = LOW;
        fakeMillisNow = 10000 + (i * 200);
        AtencionTeclado();
        procesarTeclado();
        TEST_ASSERT_EQUAL(1, menu_button_calls);
        TEST_ASSERT_EQUAL(buttons[i], last_button);
        TEST_ASSERT_TRUE(flagBuzzer);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_keyboard_debounces_and_dispatches_buttons);
    RUN_TEST(test_keyboard_ignores_pending_event_without_pressed_button);
    RUN_TEST(test_keyboard_dispatches_each_button);
    return UNITY_END();
}
