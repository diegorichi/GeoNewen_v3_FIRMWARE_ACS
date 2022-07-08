#include "Arduino.h"
#include "menu.h"

Menu::Menu(String aName, void (*showMenu)(),void (*refreshMenu)(), int _menuActual)
{
  _name = aName;
  _showMenu = showMenu;
  _refresh = refreshMenu;
  menuActual = _menuActual;
}

void Menu::setUp(Menu *up)
{
  _up = up;
}
void Menu::setDown(Menu *down)
{
  _down = down;
}
void Menu::setEnter(Menu *enter)
{
  _enter = enter;
}
void Menu::setEscape(Menu *escape)
{
  _escape = escape;
}

void Menu::setUpAction(void (*action)())
{
  _up_action = action;
}
void Menu::setDownAction(void (*action)())
{
  _down_action = action;
}

void Menu::setEnterAction(void (*action)())
{
  _enter_action = action;
}
void Menu::setEscapeAction(void (*action)())
{
  _escape_action = action;
}

String Menu::getName()
{
  return _name;
}

Menu Menu::up()
{
  if (_up_action)
  {
    _up_action();
  }
  if (_up)
  {
    _up->show();
    return (*_up);
  }
  return *this;
}
Menu Menu::down()
{
  if (_down_action)
  {
    _down_action();
  }
  if (_down)
  {
    _down->show();
    return (*_down);
  }
  return *this;
}

Menu Menu::enter()
{
  if (_enter_action)
  {
    _enter_action();
  }
  if (_enter)
  {
    _enter->show();
    return (*_enter);
  }
  return *this;
}

Menu Menu::escape()
{
  if (_escape_action){
    _escape_action();
  }
  if (_escape){
    _escape->show();
    return (*_escape);
  }  
  return *this;
}

void Menu::show()
{
  if (_showMenu)
  {
    _showMenu();
  }
}

void Menu::refresh()
{
  if (_refresh){
    _refresh();
  }
}
