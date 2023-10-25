#ifndef LIB_EEPROM_h
#define LIB_EEPROM_h
#include "define.h"


#define EEPROM_SIZE     127
#define EEPROM_ADDRESS  0

void EEPROM_Init(void);
uint8_t EEPROM_Write(uint8_t *arr);
uint8_t EEPROM_Read(uint8_t *arr);

#endif

