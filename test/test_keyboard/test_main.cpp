#include <unity.h>
#include "keyboard.cpp"

unsigned long fake_millis_now = 0;
int fake_digital_inputs[64] = {};
volatile MenuId MenuActual = MENU_HOME;
volatile bool Flag_Buzzer = false;
int menu_button_calls = 0;
MenuButton last_button = BUTTON_UP;

unsigned long millis() { return fake_millis_now; }
int digitalRead(int pin) { return fake_digital_inputs[pin]; }
void pinMode(int, int) {}
void noInterrupts() {}
void interrupts() {}
void attachInterrupt(int, void (*)(), int) {}
void processMenuButton(MenuButton button) { ++menu_button_calls; last_button = button; }

void resetFixtures() {
    for (int& value : fake_digital_inputs) value = HIGH;
    fake_millis_now = 0;
    tecladoPendiente = false;
    Flag_Buzzer = false;
    menu_button_calls = 0;
}

void test_keyboard_debounces_and_dispatches_buttons(void) {
    resetFixtures();
    keyboardSetup();
    fake_digital_inputs[DI_Teclado_Enter] = LOW;
    fake_millis_now = 151;
    AtencionTeclado();
    procesarTeclado();
    TEST_ASSERT_EQUAL(1, menu_button_calls);
    TEST_ASSERT_EQUAL(BUTTON_ENTER, last_button);
    TEST_ASSERT_TRUE(Flag_Buzzer);

    fake_millis_now = 200;
    AtencionTeclado();
    procesarTeclado();
    TEST_ASSERT_EQUAL(1, menu_button_calls);

    fake_millis_now = 302;
    AtencionTeclado();
    procesarTeclado();
    TEST_ASSERT_EQUAL(2, menu_button_calls);
}

void test_keyboard_ignores_pending_event_without_pressed_button(void) {
    resetFixtures();
    fake_millis_now = 500;
    AtencionTeclado();
    procesarTeclado();
    TEST_ASSERT_EQUAL(0, menu_button_calls);
    TEST_ASSERT_FALSE(Flag_Buzzer);
}

void test_keyboard_dispatches_each_button(void) {
    resetFixtures();
    const int pins[] = {DI_Teclado_Arriba, DI_Teclado_Abajo, DI_Teclado_Enter, DI_Teclado_Atras};
    const MenuButton buttons[] = {BUTTON_UP, BUTTON_DOWN, BUTTON_ENTER, BUTTON_BACK};
    for (int i = 0; i < 4; ++i) {
        resetFixtures();
        fake_digital_inputs[pins[i]] = LOW;
        fake_millis_now = 10000 + (i * 200);
        AtencionTeclado();
        procesarTeclado();
        TEST_ASSERT_EQUAL(1, menu_button_calls);
        TEST_ASSERT_EQUAL(buttons[i], last_button);
        TEST_ASSERT_TRUE(Flag_Buzzer);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_keyboard_debounces_and_dispatches_buttons);
    RUN_TEST(test_keyboard_ignores_pending_event_without_pressed_button);
    RUN_TEST(test_keyboard_dispatches_each_button);
    return UNITY_END();
}
