#ifndef MENU_ACTIONS_H
#define MENU_ACTIONS_H

#include "menu_navigation.h"

using MenuAction = void (*)();

MenuAction menuActionFor(MenuId id, MenuButton button);

#endif
