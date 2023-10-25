#ifndef LIB_FRAME_MSG_h
#define LIB_FRAME_MSG_h
#include "define.h"

#define RETRUN_ERROR -1

#define START_BYTE 0xA5

/*Start(1byte) + TypeMessage(1byte) + Length(2byte) = 4byte */
#define DEFAULT_BYTE 4

#define DEFAULT_BYTE_CHECKSUM 2 //  2byte checksum
#define DEFAULT_MAX_SIZE_DATA 140

typedef struct
{
    uint8_t Start;
    uint8_t TypeMessage;
    uint16_t Length;
    uint8_t Data[DEFAULT_MAX_SIZE_DATA];
    uint16_t Crc;
} messageFrameMsg_t;

typedef enum
{
    TYPE_MSG_UPDATE_WIFI = 0x01,
} typeMessage_e;

int16_t DetectMessage(uint8_t *dataint, messageFrameMsg_t *dataout);
int16_t CreateMessage(uint8_t type_msg, uint16_t length, uint8_t *datain, uint8_t *dataout);

int16_t DebugMessage(uint8_t *dataint);

#endif
