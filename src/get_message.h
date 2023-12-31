#ifndef LIB_GET_MSG_h
#define LIB_GET_MSG_h
#include "define.h"

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

uint16_t Is_Message(uint16_t *lenght);
void BTS_Get_Message(uint8_t datain, uint8_t arr_message[]);

#endif
