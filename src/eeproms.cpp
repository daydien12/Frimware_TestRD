#include "eeproms.h"


uint8_t arr_finger[EEPROM_SIZE] = {0};

void EEPROM_Init(void)
{
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to initialise EEPROM"); 
  }

}

uint8_t EEPROM_Write(uint8_t *arr)
{
  uint8_t address = EEPROM_ADDRESS;
  for (uint8_t i = 0; i < EEPROM_SIZE; i++)
  {
    EEPROM.writeByte(address, arr[i]);
    EEPROM.commit();
    address += 1;
  }
  return 1;
}

uint8_t EEPROM_Read(uint8_t *arr)
{
  uint8_t address = EEPROM_ADDRESS;
  for (uint8_t i = 0; i < EEPROM_SIZE; i++)
  {
    arr[i] = EEPROM.readByte(address);
    address += 1;
  }
  return 1;
}
