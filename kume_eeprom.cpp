#include "kume_eeprom.h"


void EEPROMUpdate() //Función de guardado de valores en memoria EEPROM
{
  //Se almacenan los bytes más y menos significativos, debido a que la variable de tipo int ocupa 16 bits, y cada posición de memoria EEPROM es de 8 bits
  H = highByte(Ciclo_Trabajo);
  L = lowByte(Ciclo_Trabajo);
  //Se escriben los bytes en direcciones de memoria contiguas.
  EEPROM.write(DutyCAddress, H);
  EEPROM.write(DutyCAddress + 1, L);
  H = highByte(SetP_ACS);
  L = lowByte(SetP_ACS);
  EEPROM.write(SetP_ACS_Address, H);
  EEPROM.write(SetP_ACS_Address + 1, L);
  H = highByte(Var_Mod_2);
  L = lowByte(Var_Mod_2);
  EEPROM.write(Var_2_Address, H);
  EEPROM.write(Var_2_Address + 1, L);
  H = highByte(Var_Mod_3);
  L = lowByte(Var_Mod_3);
  EEPROM.write(Var_3_Address, H);
  EEPROM.write(Var_3_Address + 1, L);
  EEPROM.write(Alarma_EN_Address, Flag_Alarma_EN);
  EEPROM.write(SetP_ACS_Address, SetP_ACS);
  EEPROM.write(ACS_EN_Address, Flag_ACS_EN);
  EEPROM.write(ACS_DT_EN_Address, Flag_ACS_DT_EN);
  EEPROM.write(ACS_EN_ELECT_Address, Flag_ACS_EN_ELECT);
  EEPROM.write(ModeAddress, ModoFrioCalor); //La variable "ModoFrioCalor es de tipo bool y ocupa solo 8 bits, por lo que no hay que dividirla en 2 partes
  //EEPROM.write(Mode_F_Address, Modo_Funcionamiento);
  //H = highByte(Pos_Valv);
  //L = lowByte(Pos_Valv);
  //EEPROM.write(Pos_Valv_Address, H);
  //EEPROM.write(Pos_Valv_Address+1, L);
}

void EEPROMLectura() //Función de lectura de valores almacenados en memoria EEPROM
{
  byte lowByte = EEPROM.read(DutyCAddress + 1); //Se leen los 2 bytes que componen la variable int por separado
  byte highByte = EEPROM.read(DutyCAddress);
  Ciclo_Trabajo = ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00); //Se ensamblan las 2 mitades de la variable
  Ciclo_Trabajoedit = Ciclo_Trabajo;                                    //Se asigna el valor a la variable de edición, para poder mostrar el valor actual de la misma
  lowByte = EEPROM.read(SetP_ACS_Address + 1);
  highByte = EEPROM.read(SetP_ACS_Address);
  SetP_ACS = ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
  SetP_ACS = EEPROM.read(SetP_ACS_Address);
  SetP_ACS_Edit = SetP_ACS;
  lowByte = EEPROM.read(Var_2_Address + 1);
  highByte = EEPROM.read(Var_2_Address);
  Var_Mod_2 = ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
  Var_Mod_2_Edit = Var_Mod_2;
  lowByte = EEPROM.read(Var_3_Address + 1);
  highByte = EEPROM.read(Var_3_Address);
  Var_Mod_3 = ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
  Var_Mod_3_Edit = Var_Mod_3;
  Flag_Alarma_EN = EEPROM.read(Alarma_EN_Address);
  Flag_ACS_EN = EEPROM.read(ACS_EN_Address);
  Flag_ACS_DT_EN = EEPROM.read(ACS_DT_EN_Address);
  Flag_ACS_EN_ELECT = EEPROM.read(ACS_EN_ELECT_Address);
  ModoFrioCalor = EEPROM.read(ModeAddress);
  //lowByte = EEPROM.read(SetP_Temp_Admision_Address + 1);
  //highByte = EEPROM.read(SetP_Temp_Admision_Address);
  //SetP_Temp_Admision = ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
  //SetP_Temp_Admision = EEPROM.read(SetP_Temp_Amision_Address);
  //SetP_Temp_Admision_Edit = SetP_Temp_Admision;


}
