#ifndef LIB_MQTT_h
#define LIB_MQTT_h

#include "define.h"
#include "eeproms.h"

typedef struct
{
    bool flag;
    uint8_t servo[4];
    uint8_t time;
    uint8_t wait;
    uint8_t keep;
} mqttSwitchPress_t;
extern mqttSwitchPress_t mqtt_switch_press;

typedef struct
{
    bool flag;
    uint8_t status;
} mqttSwitchPower_t;
extern mqttSwitchPower_t mqtt_switch_power;

typedef struct
{
    bool flag;
    uint8_t type;
} mqttSwitchType_t;
extern mqttSwitchType_t mqtt_switch_type;

typedef struct
{
    bool flag;
    uint8_t pinout[4];
} mqttSwitchPinout_t;
extern mqttSwitchPinout_t mqtt_switch_pinout;

extern bool mqtt_flag_config;

void MQTT_Run(void);
void MQTT_Receive(char *datain, char *topic);
void MQTT_Init(void (*callback)(char *, byte *, unsigned int));
void MQTT_Response_OutputSwitch(uint8_t switch_id, uint8_t out1, uint8_t out2, uint8_t out3, uint8_t out4);
#endif
