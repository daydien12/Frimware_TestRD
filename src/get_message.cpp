#include "get_message.h"

int16_t length_message = 0;
uint8_t flag_new_message;
uint16_t count_element_arr;
uint16_t data_after_length;

int32_t timeout_start;
int32_t timeout_wait;
fsmListState_e fsm_state;

static void ClearState(void);
static void Time_Out_Get_Message(void);
/**
   @brief : Flag of the new message

   @param lenght : Length of the new message
   @return uint16_t : Return 1 if received successfully message, else return 0
*/
uint16_t Is_Message(uint16_t *lenght)
{
    Time_Out_Get_Message();
    if (flag_new_message == TRUE)
    {
        timeout_wait = FALSE;
        flag_new_message = FALSE;
        *lenght = length_message;
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
   @brief Get the Message:: Time Out object
   time to receive the new message
   used to avoid the case where the message is long or the message is shorter than the message length
*/
void Time_Out_Get_Message(void)
{
    if (timeout_wait == TRUE)
    {
        if (timeout_start >= COUNTER_TIMEOUT)
        {
            length_message = 0;
            ClearState();
        }
        timeout_start++;
    }
    else
    {
        ClearState();
    }
}

/**
   @brief Get the message from message buffer received from serial port
   @param datain : One byte data receive
   @param arr_message : Array data out of message
*/
void Get_Message(uint8_t datain, uint8_t arr_message[])
{
    timeout_wait = TRUE;
    timeout_start = 0;

    arr_message[count_element_arr] = datain;
    count_element_arr++;

    switch (fsm_state)
    {
    case FSM_STATE_START:
        if (count_element_arr == FSM_STATE_CHANGE_VALUE_WAIT)
        {
            if (arr_message[0] != START_BYTE)
            {
                ClearState();
            }
            else
            {
                fsm_state = FSM_STATE_WAIT;
            }
        }
        break;

    case FSM_STATE_WAIT:
        if (count_element_arr == FSM_STATE_CHANGE_VALUE_END)
        {
            data_after_length = Bts_Convert_From_Bytes_To_Uint16(arr_message[2], arr_message[3]);
            fsm_state = FSM_STATE_END;
        }
        break;

    case FSM_STATE_END:
        if (count_element_arr == (data_after_length + DEFAULT_BYTE))
        {
            flag_new_message = TRUE;
            length_message = count_element_arr;
            ClearState();
        }
        break;
    }
}

/**
   @brief Used to reset elements when get message successfully or timeout.

*/
void ClearState(void)
{
    count_element_arr = 0;
    data_after_length = 0;
    timeout_start = 0;
    timeout_wait = FALSE;
    fsm_state = FSM_STATE_START;
}

void Get_MessageConfigWifi(const messageFrameMsg_t datain)
{
    EEPROM_ReadStruct(&eeprom_data);
    wifiConfigData_t *wifi_config_data;
    wifi_config_data = (wifiConfigData_t *)datain.Data;
    for (int j = 0; j < sizeof(eeprom_data.wifi_ssid); j++)
    {
        eeprom_data.wifi_ssid[j] = 0;
        eeprom_data.wifi_pass[j] = 0;
    }
    eeprom_data.wifi_timeout = wifi_config_data->wifi_timeout;
    memcpy(eeprom_data.wifi_ssid, wifi_config_data->wifi_ssid, strlen(wifi_config_data->wifi_ssid));
    memcpy(eeprom_data.wifi_pass, wifi_config_data->wifi_pass, strlen(wifi_config_data->wifi_pass));
    EEPROM_WriteStruct(&eeprom_data);
#if DB_GET_MESSAGE
    DB_DEBUG("(Get_MSG) SSID: %s, PASS: %s, TimeOut: %d \n", wifi_config_data->wifi_ssid, wifi_config_data->wifi_pass, wifi_config_data->wifi_timeout);
#endif
}

void Get_MessageConfigServo(messageFrameMsg_t datain)
{
    EEPROM_ReadStruct(&eeprom_data);
    servoConfigData_t *servo_config_data;
    servo_config_data = (servoConfigData_t *)datain.Data;

    eeprom_data.servo1_up = servo_config_data->servo1_up;
    eeprom_data.servo1_down = servo_config_data->servo1_down;
    eeprom_data.servo2_up = servo_config_data->servo2_up;
    eeprom_data.servo2_down = servo_config_data->servo2_down;
    eeprom_data.servo3_up = servo_config_data->servo3_up;
    eeprom_data.servo3_down = servo_config_data->servo3_down;
    eeprom_data.servo4_up = servo_config_data->servo4_up;
    eeprom_data.servo4_down = servo_config_data->servo4_down;
    EEPROM_WriteStruct(&eeprom_data);
#if DB_GET_MESSAGE
    DB_DEBUG("(Get_MSG) SV1: UP(%d) - DOWN(%d)\n", servo_config_data->servo1_up, servo_config_data->servo1_down);
    DB_DEBUG("(Get_MSG) SV2: UP(%d) - DOWN(%d)\n", servo_config_data->servo2_up, servo_config_data->servo2_down);
    DB_DEBUG("(Get_MSG) SV3: UP(%d) - DOWN(%d)\n", servo_config_data->servo3_up, servo_config_data->servo3_down);
    DB_DEBUG("(Get_MSG) SV4: UP(%d) - DOWN(%d)\n", servo_config_data->servo4_up, servo_config_data->servo4_down);
#endif
}

void Get_MessageControlServo(messageFrameMsg_t datain, servoControlData_t *dataout)
{
    servoControlData_t *servo_control_data;
    servo_control_data = (servoControlData_t *)datain.Data;
    dataout->servo1_control = servo_control_data->servo1_control;
    dataout->servo2_control = servo_control_data->servo2_control;
    dataout->servo3_control = servo_control_data->servo3_control;
    dataout->servo4_control = servo_control_data->servo4_control;
#if DB_GET_MESSAGE
    DB_DEBUG("(Get_MSG) SV1: %d, SV2: %d, SV3: %d, SV4: %d \n", servo_control_data->servo1_control, servo_control_data->servo2_control, servo_control_data->servo3_control, servo_control_data->servo4_control);
#endif
}