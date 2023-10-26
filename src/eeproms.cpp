#include "eeproms.h"

eepromDataStruct_t eeprom_data;

void EEPROM_Init(void)
{
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    DB_ERROR("failed to initialise EEPROM\n");
  }
}

uint8_t EEPROM_WriteStruct(const eepromDataStruct_t *data_struct)
{
  uint8_t *arr_temp = (uint8_t *)data_struct;
  uint8_t address = EEPROM_ADDRESS;

  for (uint16_t i = 0; i < sizeof(eepromDataStruct_t); i++)
  {
    EEPROM.writeByte(address, arr_temp[i]);
    EEPROM.commit();
    address += 1;
  }
  return 1;
}

uint8_t EEPROM_ReadStruct(eepromDataStruct_t *data_struct)
{
  uint8_t address = EEPROM_ADDRESS;
  eepromDataStruct_t *data_temp;
  static uint8_t arr_temp[sizeof(eepromDataStruct_t)];
  for (uint16_t i = 0; i < sizeof(eepromDataStruct_t); i++)
  {
    arr_temp[i] = EEPROM.readByte(address);
    address += 1;
  }
  data_temp = (eepromDataStruct_t *)arr_temp;

  data_struct->servo1_up = data_temp->servo1_up;
  data_struct->servo1_down = data_temp->servo1_down;
  data_struct->servo2_up = data_temp->servo2_up;
  data_struct->servo2_down = data_temp->servo2_down;
  data_struct->servo3_up = data_temp->servo3_up;
  data_struct->servo3_down = data_temp->servo3_down;
  data_struct->servo4_up = data_temp->servo4_up;
  data_struct->servo4_down = data_temp->servo4_down;
  data_struct->wifi_timeout = data_temp->wifi_timeout;
  memcpy(data_struct->wifi_ssid, data_temp->wifi_ssid, strlen(data_temp->wifi_ssid) + 1);
  memcpy(data_struct->wifi_pass, data_temp->wifi_pass, strlen(data_temp->wifi_pass) + 1);
  return 1;
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

void EEPROM_TestWrite(void)
{
  eepromDataStruct_t data_in;

  data_in.servo1_up = 20;
  data_in.servo1_down = 11;

  data_in.servo2_up = 30;
  data_in.servo2_down = 12;

  data_in.servo3_up = 40;
  data_in.servo3_down = 13;

  data_in.servo4_up = 50;
  data_in.servo4_down = 14;

  data_in.wifi_timeout = 10 * 10; // 50*100 = 5 seconds

  memcpy(data_in.wifi_ssid, "Ai-Photonic 2G", strlen("Ai-photonic 2G") + 1);
  memcpy(data_in.wifi_pass, "ptitlab@123", strlen("ptitlab@123") + 1);
  EEPROM_WriteStruct(&data_in);
}

void EEPROM_TestRead(void)
{
  EEPROM_ReadStruct(&eeprom_data);
  Serial.println(eeprom_data.servo1_up);
  Serial.println(eeprom_data.servo1_down);

  Serial.println(eeprom_data.servo2_up);
  Serial.println(eeprom_data.servo2_down);

  Serial.println(eeprom_data.servo3_up);
  Serial.println(eeprom_data.servo3_down);

  Serial.println(eeprom_data.servo4_up);
  Serial.println(eeprom_data.servo4_down);

  Serial.println(eeprom_data.wifi_ssid);
  Serial.println(eeprom_data.wifi_pass);
  Serial.println(eeprom_data.wifi_timeout);
}

uint8_t EEPROM_WriteConfig(void)
{
}
uint8_t EEPROM_ReadConfig(void)
{
}