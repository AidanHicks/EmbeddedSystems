#ifndef EEPROM_CFG_H
#define EEPROM_CFG_H

#include "config_bits.h"

void EEPROM_Load_Settings(SystemSettings *s);
void EEPROM_Save_Settings(SystemSettings *s);

#endif