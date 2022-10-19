#include "Arduino.h"
#include "menu.h"

Menu menuMonitorRegistroAlarmas("MonitorRegistroAlarmas", &MenuCincoCero, &RefreshMenuCincoCero, 50);

Menu menuMonitorAlarmas("MonitorAlarmas", &MenuCuatroCero, &RefreshMenuCuatroCero, 40);

//Menu menuConfCalderaElectrica("ConfCalderaElectrica", &MenuTresCero, &RefreshMenuTresCero, 30);

Menu menuConfiguracionEditTemperaturaACS("ConfEditTemperaturaACS", &MenuDosUnoCero, &RefreshMenuDosUnoCero, 210);

Menu menuConfiguracionACSElectrico("ConfACSElectrico", &MenuDosNueve, &RefreshMenuDosNueve, 29);
Menu menuConfiguracionACSDT("ConfACSDT", &MenuDosOchoUno, &RefreshMenuDosOchoUno, 281);
Menu menuConfiguracionACS("ConfACS", &MenuDosOcho, &RefreshMenuDosOcho, 28);
Menu menuConfiguracionWifi("ConfWifi", &MenuDosSiete, &RefreshMenuDosSiete, 27);
Menu menuConfiguracionModoAutomatico("ConfModoAutomatico", &MenuDosCuatro, &RefreshMenuDosCuatro, 24);
Menu menuConfiguracionAlarmasCaudal("ConfAlarmasCaudal", &MenuDosTres, &RefreshMenuDosTres, 23);
// [](){} es una funcion lambra que []= esos son los argumentos ()agumentos que recibe {} cuerpo
Menu menuConfiguracionTemperaturaACS(
  "ConfTemperaturaACS", &MenuDosUno, [ ] () {}, 21);
Menu menuConfiguracionModoFrioCalor("ConfModoFrioCalor", &MenuDosCero, &RefreshMenuDosCero, 20);

//Menu menuMonitorSistemaCompresor("MonitorSistemaCompresor", &MenuUnoDos, &RefreshMenuUnoDos, 12);
Menu menuMonitorSistemaCaudalesYTemperaturas("MonitorSistemaCaudalesYTemperaturas", &MenuUnoUno, &RefreshMenuUnoUno, 11);
Menu menuMonitorSistemaACSADM("MonitorSistemaACSADM", &MenuUnoCero, &RefreshMenuUnoCero, 10);

Menu menuRegistroAlarmas(
  "RegistroAlarmas", &MenuCinco, [ ] () {}, 5);
Menu menuAlarmas(
  "Alarmas", &MenuCuatro, [ ] () {}, 4);
//Menu menuCalderaElectrica(
//  "ConfCalderaElectrica", &MenuTres, [ ] () {}, 3);
Menu menuConfiguracionSistema(
  "ConfigracionSistema", &MenuDos, [ ] () {}, 2);
Menu menuMonitorSistema(
  "MonitorSistema", &MenuUno, [ ] () {}, 1);

Menu menuInicial("MenuInicial", &MenuCero, &RefreshMenuCero, 0);

Menu* menuActivo;

void initializeAndSetupMenu() {

  // 29
  menuConfiguracionACSElectrico.setUp(&menuConfiguracionACSDT);
  menuConfiguracionACSElectrico.setDown(&menuConfiguracionModoFrioCalor);
  menuConfiguracionACSElectrico.setEnterAction([ ] () {
    Flag_ACS_EN_ELECT = !Flag_ACS_EN_ELECT;
    EEPROMUpdate(); });
  menuConfiguracionACSElectrico.setEscape(&menuConfiguracionSistema);

  //281
  menuConfiguracionACSDT.setUp(&menuConfiguracionACSElectrico);
  menuConfiguracionACSDT.setDown(&menuConfiguracionACS);
  menuConfiguracionACSDT.setEnterAction([ ] () {
    Flag_ACS_DT_EN = !Flag_ACS_DT_EN;
    EEPROMUpdate(); });
  menuConfiguracionACSDT.setEscape(&menuConfiguracionSistema);

  // 28
  menuConfiguracionACS.setUp(&menuConfiguracionWifi);
  menuConfiguracionACS.setDown(&menuConfiguracionACSDT);
  menuConfiguracionACS.setEnterAction([ ] () {
    Flag_ACS_EN = !Flag_ACS_EN;
    EEPROMUpdate(); });
  menuConfiguracionACS.setEscape(&menuConfiguracionSistema);

  // 27
  menuConfiguracionWifi.setUp(&menuConfiguracionModoAutomatico);
  menuConfiguracionWifi.setDown(&menuConfiguracionACS);
  menuConfiguracionWifi.setEnterAction([ ] () {
    Estado_ConfigWIFI = 1;
    EntradaConfigWifi = millis();
    Serial3.println("AT+CWSTARTSMART"); });
  menuConfiguracionWifi.setEscape(&menuConfiguracionSistema);
  menuConfiguracionWifi.setEscapeAction([ ] () { Estado_ConfigWIFI = 0; });

  // 24
  menuConfiguracionModoAutomatico.setUp(&menuConfiguracionAlarmasCaudal);
  menuConfiguracionModoAutomatico.setDown(&menuConfiguracionWifi);
  menuConfiguracionModoAutomatico.setEnterAction([ ] () {
    Modo_Funcionamiento = !Modo_Funcionamiento;
    EEPROMUpdate(); });
  menuConfiguracionModoAutomatico.setEscape(&menuConfiguracionSistema);

  // 23
  menuConfiguracionAlarmasCaudal.setUp(&menuConfiguracionTemperaturaACS);
  menuConfiguracionAlarmasCaudal.setDown(&menuConfiguracionModoAutomatico);
  menuConfiguracionAlarmasCaudal.setEnterAction([ ] () {
    Flag_Alarma_EN = !Flag_Alarma_EN;
    EEPROMUpdate(); });
  menuConfiguracionAlarmasCaudal.setEscape(&menuConfiguracionSistema);

  // 210
  menuConfiguracionEditTemperaturaACS.setUpAction([ ] () {
    SetP_ACS_Edit = SetP_ACS_Edit + 1;
    if (SetP_ACS_Edit > 48) {
      SetP_ACS_Edit = 48;
    } });
  menuConfiguracionEditTemperaturaACS.setDownAction([ ] () {
    SetP_ACS_Edit = SetP_ACS_Edit - 1;
    if (SetP_ACS_Edit < 30) {
      SetP_ACS_Edit = 30;
    } });
  menuConfiguracionEditTemperaturaACS.setEnterAction([ ] () {
    SetP_ACS = SetP_ACS_Edit;
    EEPROMUpdate(); });
  menuConfiguracionEditTemperaturaACS.setEscape(&menuConfiguracionTemperaturaACS);

  // 21
  menuConfiguracionTemperaturaACS.setUp(&menuConfiguracionModoFrioCalor);
  menuConfiguracionTemperaturaACS.setDown(&menuConfiguracionAlarmasCaudal);
  menuConfiguracionTemperaturaACS.setEnter(&menuConfiguracionEditTemperaturaACS);
  menuConfiguracionTemperaturaACS.setEscape(&menuConfiguracionSistema);

  // 20
  menuConfiguracionModoFrioCalor.setUp(&menuConfiguracionACSElectrico);
  menuConfiguracionModoFrioCalor.setDown(&menuConfiguracionTemperaturaACS);
  menuConfiguracionModoFrioCalor.setEnterAction([ ] () {
    if (Estado_Maquina == 1) //|| Estado_Maquina == 3
    {
      FrioCalor();
      EEPROMUpdate();
    } });
  menuConfiguracionModoFrioCalor.setEscape(&menuConfiguracionSistema);

  // 12
  //menuMonitorSistemaCompresor.setUp(&menuMonitorSistemaCaudalesYTemperaturas);
  //menuMonitorSistemaCompresor.setDown(&menuMonitorSistemaACSADM);
  // 11
  menuMonitorSistemaCaudalesYTemperaturas.setUp(&menuMonitorSistemaACSADM);
  menuMonitorSistemaCaudalesYTemperaturas.setDown(&menuMonitorSistemaACSADM);
  // 10
  menuMonitorSistemaACSADM.setUp(&menuMonitorSistemaCaudalesYTemperaturas);
  menuMonitorSistemaACSADM.setDown(&menuMonitorSistemaCaudalesYTemperaturas);

  // 50
  menuMonitorRegistroAlarmas.setEscape(&menuRegistroAlarmas);
  menuMonitorRegistroAlarmas.setEnterAction([ ] () { EEPROM.write(Alarma_Address, 0); });

  // 5
  menuRegistroAlarmas.setUp(&menuAlarmas);
  menuRegistroAlarmas.setDown(&menuMonitorSistema);
  menuRegistroAlarmas.setEscape(&menuInicial);
  menuRegistroAlarmas.setEnter(&menuMonitorRegistroAlarmas);

  // 40
  menuMonitorAlarmas.setUpAction([ ] () { digitalWrite(DO_Buzzer, LOW); });
  menuMonitorAlarmas.setDownAction([ ] () { digitalWrite(DO_Buzzer, LOW); });
  menuMonitorAlarmas.setEscape(&menuAlarmas);
  menuMonitorAlarmas.setEnterAction([ ] () {
    Flag_Alarma_General = false;
    Nro_Alarma = 0;
    digitalWrite(DO_Buzzer, LOW);
    Flag_Buzzer = false;
    Alarma_Activa = false;
    ResetFlags();
    Estado_Maquina = 0; });

  // 4
  menuAlarmas.setUp(&menuConfiguracionSistema);
  menuAlarmas.setDown(&menuRegistroAlarmas);
  menuAlarmas.setEscape(&menuInicial);
  menuAlarmas.setEnter(&menuMonitorAlarmas);

  // 2
  menuConfiguracionSistema.setUp(&menuMonitorSistema);
  menuConfiguracionSistema.setDown(&menuAlarmas);
  menuConfiguracionSistema.setEscape(&menuInicial);
  menuConfiguracionSistema.setEnter(&menuConfiguracionModoFrioCalor);
  // 1
  menuMonitorSistema.setUp(&menuRegistroAlarmas);
  menuMonitorSistema.setDown(&menuConfiguracionSistema);
  menuMonitorSistema.setEnter(&menuMonitorSistemaACSADM);
  menuMonitorSistema.setEscape(&menuInicial);

  menuInicial.setEnter(&menuMonitorSistema);
  menuActivo = &menuInicial;
  menuActivo->show();
}

Menu::Menu(String aName, void (*showMenu)(), void (*refreshMenu)(), int _menuActual) {
  _name = aName;
  _showMenu = showMenu;
  _refresh = refreshMenu;
  menuActual = _menuActual;
}

void Menu::setUp(Menu* up) {
  _up = up;
}
void Menu::setDown(Menu* down) {
  _down = down;
}
void Menu::setEnter(Menu* enter) {
  _enter = enter;
}
void Menu::setEscape(Menu* escape) {
  _escape = escape;
}

void Menu::setUpAction(void (*action)()) {
  _up_action = action;
}
void Menu::setDownAction(void (*action)()) {
  _down_action = action;
}

void Menu::setEnterAction(void (*action)()) {
  _enter_action = action;
}
void Menu::setEscapeAction(void (*action)()) {
  _escape_action = action;
}

String Menu::getName() {
  return _name;
}

Menu Menu::up() {
  if (_up_action) {
    _up_action();
  }
  if (_up) {
    _up->show();
    return (*_up);
  }
  return *this;
}
Menu Menu::down() {
  if (_down_action) {
    _down_action();
  }
  if (_down) {
    _down->show();
    return (*_down);
  }
  return *this;
}

Menu Menu::enter() {
  if (_enter_action) {
    _enter_action();
  }
  if (_enter) {
    _enter->show();
    return (*_enter);
  }
  return *this;
}

Menu Menu::escape() {
  if (_escape_action) {
    _escape_action();
  }
  if (_escape) {
    _escape->show();
    return (*_escape);
  }
  return *this;
}

void Menu::show() {
  if (_showMenu) {
    _showMenu();
  }
}

void Menu::refresh() {
  if (_refresh) {
    _refresh();
  }
}
