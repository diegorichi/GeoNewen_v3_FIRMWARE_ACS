#include <unity.h>
#include "ui_controller.cpp"

volatile int Estado_Maquina = 1;
volatile MenuId MenuActual = MENU_HOME;
bool locked = false;
int navigation_calls = 0;
MenuId last_navigation = MENU_NONE;

bool isModeChangeLocked() { return locked; }
void navigateTo(MenuId id) { ++navigation_calls; last_navigation = id; MenuActual = id; }

void resetFixtures() {
    Estado_Maquina = 1;
    MenuActual = MENU_HOME;
    locked = false;
    navigation_calls = 0;
    last_navigation = MENU_NONE;
}

void test_ui_enters_and_exits_alarm_menu_on_edges(void) {
    resetFixtures();
    processUiEvents();
    Estado_Maquina = 4;
    processUiEvents();
    TEST_ASSERT_EQUAL(1, navigation_calls);
    TEST_ASSERT_EQUAL(MENU_ALARM_ACTIVE, last_navigation);
    processUiEvents();
    TEST_ASSERT_EQUAL(1, navigation_calls);
    Estado_Maquina = 1;
    processUiEvents();
    TEST_ASSERT_EQUAL(2, navigation_calls);
    TEST_ASSERT_EQUAL(MENU_ALARM_MONITOR, last_navigation);
}

void test_ui_enters_and_exits_mode_lock_screen_on_edges(void) {
    resetFixtures();
    locked = true;
    processUiEvents();
    TEST_ASSERT_EQUAL(1, navigation_calls);
    TEST_ASSERT_EQUAL(MENU_MODE_CHANGING, last_navigation);
    processUiEvents();
    TEST_ASSERT_EQUAL(1, navigation_calls);
    locked = false;
    processUiEvents();
    TEST_ASSERT_EQUAL(2, navigation_calls);
    TEST_ASSERT_EQUAL(MENU_MODE, last_navigation);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ui_enters_and_exits_alarm_menu_on_edges);
    RUN_TEST(test_ui_enters_and_exits_mode_lock_screen_on_edges);
    return UNITY_END();
}
