#ifndef LIB_GET_MSG_h
#define LIB_GET_MSG_h
// #include "define.h"

#include "define.h"
#include "eeproms.h"
#include "frame_message.h"

#define TRUE 1
#define FALSE 0
#define COUNTER_TIMEOUT 100

typedef enum
{
    FSM_STATE_START = 0,
    FSM_STATE_WAIT = 1,
    FSM_STATE_END = 2,
} fsmListState_e;

typedef enum
{
    FSM_STATE_CHANGE_VALUE_START_FRAME = 0,
    FSM_STATE_CHANGE_VALUE_WAIT = 2,
    FSM_STATE_CHANGE_VALUE_END = 4,
} fsmValueNextStep_e;

extern int32_t timeout_wait;

typedef struct
{
    uint16_t wifi_timeout;
    char wifi_ssid[32];
    char wifi_pass[32];
} wifiConfigData_t;

typedef struct
{
    uint16_t servo1_up;
    uint16_t servo1_down;
    uint16_t servo2_up;
    uint16_t servo2_down;
    uint16_t servo3_up;
    uint16_t servo3_down;
    uint16_t servo4_up;
    uint16_t servo4_down;
} servoConfigData_t;

typedef struct
{
    uint8_t servo1_control;
    uint8_t servo2_control;
    uint8_t servo3_control;
    uint8_t servo4_control;
} servoControlData_t;

uint16_t Is_Message(uint16_t *lenght);
void Get_Message(uint8_t datain, uint8_t arr_message[]);

void Get_MessageConfigWifi(messageFrameMsg_t datain);
void Get_MessageConfigServo(messageFrameMsg_t datain);
void Get_MessageControlServo(messageFrameMsg_t datain, servoControlData_t *dataout);
#endif
