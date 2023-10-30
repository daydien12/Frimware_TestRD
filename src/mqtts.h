#ifndef LIB_MQTT_h
#define LIB_MQTT_h

#include "define.h"
#include "eeproms.h"


void MQTT_Response_PowerSwitch(uint8_t switch_id);
void MQTT_Response_PressSwitch(uint8_t switch_id);
void MQTT_Response_OutputSwitch(uint8_t switch_id, uint8_t out1, uint8_t out2, uint8_t out3, uint8_t out4);

void MQTT_Receive(char *datain);

void MQTT_Response_SetConfigWireless(uint8_t switch_id);
void MQTT_Response_GetInfoWireless(uint8_t switch_id);


void MQTT_Run(void);
void MQTT_Init(void(*callback) (char *, byte *, unsigned int));

#endif
