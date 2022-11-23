#ifndef Menu_h
#define Menu_h
#include "Arduino.h"
#include "functionsLCDMenu.h"
#include "machine_control.h"
#include "alarm.h"



class Menu {

private:
  String _name;
  Menu* _up;
  Menu* _down;
  Menu* _enter;
  Menu* _escape;

  void (*_up_action)(void);     // puntero a funcion ej:MenuUnoCero
  void (*_down_action)(void);   // puntero a funcion ej:MenuUnoCero
  void (*_enter_action)(void);  // puntero a funcion ej:MenuUnoCero
  void (*_escape_action)(void); // puntero a funcion ej:MenuUnoCero
  void (*_showMenu)(void);      // puntero a funcion ej:MenuUnoCero
  void (*_refresh)(void);       // puntero a funcion ej:MenuUnoCero

public:
  int menuActual;

  Menu(String aName, void (*showMenu)(), void (*refreshMenu)(), int _menuActual);

  String getName();

  void setUp(Menu* up);
  void setDown(Menu* down);
  void setEnter(Menu* enter);
  void setEscape(Menu* escape);

  void setUpAction(void (*action)());
  void setDownAction(void (*action)());
  void setEnterAction(void (*action)());
  void setEscapeAction(void (*action)());

  Menu up();
  Menu down();
  Menu enter();
  Menu escape();
  void show();
  void refresh();
};

extern Menu *menuActivo;


void initializeAndSetupMenu();


#endif
