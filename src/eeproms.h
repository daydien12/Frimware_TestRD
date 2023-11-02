#ifndef LIB_EEPROM_h
#define LIB_EEPROM_h

#include "define.h"

#define EEPROM_SIZE     500
#define EEPROM_ADDRESS  10

typedef struct 
{
    uint16_t create_data; 
    uint16_t servo1_up;
    uint16_t servo1_down;
    uint16_t servo2_up;
    uint16_t servo2_down;
    uint16_t servo3_up;
    uint16_t servo3_down;
    uint16_t servo4_up;
    uint16_t servo4_down;

    uint16_t wifi_timeout;
    char wifi_ssid[32];
    char wifi_pass[32];

    uint16_t mqtt_port;
    char mqtt_broker[32];
    char mqtt_username[32];
    char mqtt_password[32];	
}eepromDataStruct_t;

extern eepromDataStruct_t eeprom_data;

void EEPROM_Init(void);
void EEPROM_CreateEeprom(void);
void EEPROM_TestRead(void);

uint8_t EEPROM_Write(uint8_t *arr);
uint8_t EEPROM_Read(uint8_t *arr);

uint8_t EEPROM_WriteStruct(const eepromDataStruct_t *data_struct);
uint8_t EEPROM_ReadStruct(eepromDataStruct_t *data_struct);

uint8_t EEPROM_WriteConfig(void);
uint8_t EEPROM_ReadConfig(void);

#endif

