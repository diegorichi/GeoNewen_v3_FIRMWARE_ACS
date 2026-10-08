#include <unity.h>
#include "ui_controller.cpp"

volatile int estadoMaquina = 1;
volatile MenuId menuActual = MENU_HOME;
bool locked = false;
int navigation_calls = 0;
MenuId last_navigation = MENU_NONE;

bool isModeChangeLocked() { return locked; }
void navigateTo(MenuId id) { ++navigation_calls; last_navigation = id; menuActual = id; }

void resetFixtures() {
    estadoMaquina = 1;
    menuActual = MENU_HOME;
    locked = false;
    navigation_calls = 0;
    last_navigation = MENU_NONE;
}

void test_ui_enters_and_exits_alarm_menu_on_edges(void) {
    resetFixtures();
    processUiEvents();
    estadoMaquina = 4;
    processUiEvents();
    TEST_ASSERT_EQUAL(1, navigation_calls);
    TEST_ASSERT_EQUAL(MENU_ALARM_ACTIVE, last_navigation);
    processUiEvents();
    TEST_ASSERT_EQUAL(1, navigation_calls);
    estadoMaquina = 1;
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
