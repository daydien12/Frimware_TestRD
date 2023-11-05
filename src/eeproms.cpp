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
  data_struct->switch_number = data_temp->switch_number;
  data_struct->create_data = data_temp->create_data;

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

  data_struct->mqtt_port = data_temp->mqtt_port;
  memcpy(data_struct->mqtt_broker, data_temp->mqtt_broker, strlen(data_temp->mqtt_broker) + 1);
  memcpy(data_struct->mqtt_username, data_temp->mqtt_username, strlen(data_temp->mqtt_username) + 1);
  memcpy(data_struct->mqtt_password, data_temp->mqtt_password, strlen(data_temp->mqtt_password) + 1);
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

void EEPROM_CreateEEprom(uint16_t switch_number)
{
  eepromDataStruct_t data_in;

  data_in.create_data = 123;
  data_in.servo1_up = 0;
  data_in.servo1_down = 180;

  data_in.servo2_up = 0;
  data_in.servo2_down = 180;

  data_in.servo3_up = 0;
  data_in.servo3_down = 180;

  data_in.servo4_up = 0;
  data_in.servo4_down = 180;

  data_in.wifi_timeout = 10 * 10; // 50*100 = 5 seconds
  data_in.switch_number = switch_number;
  memcpy(data_in.wifi_ssid, "Ai-Photonic 2G", strlen("Ai-photonic 2G") + 1);
  memcpy(data_in.wifi_pass, "ptitlab@123", strlen("ptitlab@123") + 1);

  data_in.mqtt_port = 1883;
  memcpy(data_in.mqtt_broker, "Mqtt.mysignage.vn", strlen("Mqtt.mysignage.vn") + 1);
  memcpy(data_in.mqtt_username, "admin", strlen("admin") + 1);
  memcpy(data_in.mqtt_password, "admin", strlen("admin") + 1);

  EEPROM_WriteStruct(&data_in);
}

void EEPROM_Print(void)
{
  EEPROM_ReadStruct(&eeprom_data);
  DB_INFO("create_data: %d, switch_number: %d\n", eeprom_data.create_data, eeprom_data.switch_number);
  DB_INFO("servo1_up: %d, servo1_down: %d\n", eeprom_data.servo1_up, eeprom_data.servo1_down);
  DB_INFO("servo2_up: %d, servo2_down: %d\n", eeprom_data.servo2_up, eeprom_data.servo2_down);
  DB_INFO("servo3_up: %d, servo3_down: %d\n", eeprom_data.servo3_up, eeprom_data.servo3_down);
  DB_INFO("servo4_up: %d, servo4_down: %d\n", eeprom_data.servo4_up, eeprom_data.servo4_down);
  DB_INFO("wifi_ssid: %s, wifi_pass: %s, wifi_timeout: %d\n", eeprom_data.wifi_ssid, eeprom_data.wifi_pass, eeprom_data.wifi_timeout);
  DB_INFO("mqtt_port: %d, mqtt_broker: %s, mqtt_username: %s, mqtt_password: %s\n", eeprom_data.mqtt_port, eeprom_data.mqtt_broker, eeprom_data.mqtt_username, eeprom_data.mqtt_password);
}
