#ifndef LIB_MQTT_h
#define LIB_MQTT_h
#include "define.h"

void MQTT_Init(void(*callback) (char *, byte *, unsigned int));
void MQTT_Run(void);
#endif
