
#include "define.h"
#include "wifis.h"
#include "gpio.h"
#include "mqtts.h"
#include "conver.h"
#include "eeproms.h"
#include "get_message.h"
#include "frame_message.h"

#define CREATE_MESSAGE_TEST 3

uint16_t lenght_msg = 0, count_time_1ms = 0;
hw_timer_t *my_timer = NULL;
uint8_t arr_receive[string_size];

int8_t flag_wifi_connect = 0;

static void TIMER_Init(void);
static void Create_MSGTest(void);

void callback(char *topic, byte *payload, unsigned int length);

void setup()
{
  GPIO_Init();
  EEPROM_Init();
  Serial.begin(115200);
  delay(1000);
  // EEPROM_TestWrite();
  EEPROM_TestRead();
  delay(1000);
  flag_wifi_connect = WIFI_Connect();
  if (flag_wifi_connect != DB_FUNC_ERROR)
  {
    WIFI_Ota_Init();
    MQTT_Init(callback);
  }
  TIMER_Init();
}

void loop()
{
  if (Serial.available() > 0)
  {
    Get_Message(Serial.read(), arr_receive);
  }

  if (count_time_1ms >= 1000)
  {
    if (Is_Message(&lenght_msg) != 0)
    {
      if (lenght_msg > 0)
      {
        messageFrameMsg_t dataout;
        printf("\n-(Size    : %d)-\n", DetectMessage(arr_receive, &dataout));
        DebugMessage(arr_receive);
        if (dataout.TypeMessage == TYPE_MSG_UPDATE_WIFI)
        {
          Get_MessageConfigWifi(dataout);
          delay(1000);
          ESP.restart();
        }
        else if (dataout.TypeMessage == TYPE_MSG_UPDATE_SERVO)
        {
          Get_MessageConfigServo(dataout);
          delay(1000);
        }
        else if (dataout.TypeMessage == TYPE_MSG_CONTROL_SERVO)
        {
          servoControlData_t servo_control;
          Get_MessageControlServo(dataout, &servo_control);
          DB_DEBUG("(MAIN) SV1: %d, SV2: %d, SV3: %d, SV4: %d \n", servo_control.servo1_control, servo_control.servo2_control, servo_control.servo3_control, servo_control.servo4_control);
        }
        EEPROM_TestRead();
      }
    }
    if (flag_wifi_connect != DB_FUNC_ERROR)
    {
      MQTT_Run();
      WIFI_Ota_Run();
    }
    count_time_1ms = 0;
  }
  else
  {
    count_time_1ms += 1;
  }
  delayMicroseconds(1);
}

void IRAM_ATTR Timer_Handle()
{
  GPIO_Button_ReadAll();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  char buffer[length];
  Serial.print("\nMessage arrived [");
  Serial.print(topic);
  Serial.println("]");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    buffer[i] = payload[i];
  }
  Serial.println();
  MQTT_Receive(buffer);
}

void TIMER_Init(void)
{
  // chia tần 80MHz/80 = 1000000 thì được 1s
  my_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(my_timer, &Timer_Handle, true);
  timerAlarmWrite(my_timer, 1000, true); // 1ms
  timerAlarmEnable(my_timer);
}

static void Create_MSGTest(void)
{
  delay(2000);
  uint8_t arr_out[200], length_arr;
#if CREATE_MESSAGE_TEST == 1
  // uint8_t lenth_ssid = strlen("Ai-Photonic 2G");
  // uint8_t lenth_pass = strlen("ptitlab@123");

  uint8_t lenth_ssid = strlen("P702_2G");
  uint8_t lenth_pass = strlen("nhanma25");

  wifiConfigData_t data, *data_temp;
  data.wifi_timeout = 20;
  memcpy(data.wifi_ssid, "P702_2G", lenth_ssid);
  memcpy(data.wifi_pass, "nhanma25", lenth_pass);

  data.wifi_ssid[lenth_ssid] = 0;
  data.wifi_pass[lenth_pass] = 0;

  data_temp = &data;
  length_arr = CreateMessage(TYPE_MSG_UPDATE_WIFI, sizeof(wifiConfigData_t), (uint8_t *)data_temp, arr_out);
  Serial.println(length_arr);

  for (int i = 0; i < length_arr; i++)
  {
    char buff[5];
    if (arr_out[i] < 15)
    {
      sprintf(buff, "0%x", arr_out[i]);
    }
    else
    {
      sprintf(buff, "%x", arr_out[i]);
    }
    Serial.print(buff);
    Serial.print(" ");
    delay(10);

    // AI    a5 01 44 00 14 00 41 69 2d 50 68 6f 74 6f 6e 69 63 20 32 47 00 80 00 27 fb 3f 0e 00 00 00 0e 00 00 00 e3 87 08 80 70 74 69 74 6c 61 62 40 31 32 33 00 27 52 0d 80 60 27 fb 3f bc 4e 40 3f 00 00 00 00 45 31 0d 80 8b fc
    // P702  a5 01 44 00 14 00 50 37 30 32 5f 32 47 00 ff ff 3f b3 32 4f 0d 80 00 27 fb 3f 0e 00 00 00 0e 00 00 00 e3 87 08 80 6e 68 61 6e 6d 61 32 35 00 ff 3f b3 db 54 0d 80 60 27 fb 3f e4 4e 40 3f 00 00 00 00 f9 33 0d 80 bd 1b
  }

#elif CREATE_MESSAGE_TEST == 2
  servoConfigData_t data, *data_temp;
  /*a5 02 12 00 0a 00 14 00 1e 00 28 00 32 00 3c 00 46 00 5a 00 3e 30*/
  // data.servo1_up = 10;
  // data.servo1_down = 20;
  // data.servo2_up = 30;
  // data.servo2_down = 40;
  // data.servo3_up = 50;
  // data.servo3_down = 60;
  // data.servo4_up = 70;
  // data.servo4_down = 90;
  /*a5 02 12 00 01 00 02 00 03 00 ff 00 05 00 06 00 68 01 08 00 d5 ae*/
  data.servo1_up = 1;
  data.servo1_down = 2;
  data.servo2_up = 3;
  data.servo2_down = 255;
  data.servo3_up = 5;
  data.servo3_down = 6;
  data.servo4_up = 360;
  data.servo4_down = 8;

  data_temp = &data;
  length_arr = CreateMessage(TYPE_MSG_UPDATE_SERVO, sizeof(servoConfigData_t), (uint8_t *)data_temp, arr_out);
  Serial.println(length_arr);

  for (int i = 0; i < length_arr; i++)
  {
    char buff[5];
    if (arr_out[i] < 15)
    {
      sprintf(buff, "0%x", arr_out[i]);
    }
    else
    {
      sprintf(buff, "%x", arr_out[i]);
    }
    Serial.print(buff);
    Serial.print(" ");
    delay(10);
  }
#elif CREATE_MESSAGE_TEST == 3
  servoControlData_t data, *data_temp;
  /*a5 03 06 00 01 00 01 00 b9 46*/
  data.servo1_control = SERVO_CONTROL_UP;
  data.servo2_control = SERVO_CONTROL_DOWN;
  data.servo3_control = SERVO_CONTROL_UP;
  data.servo4_control = SERVO_CONTROL_DOWN;

  /*a5 03 06 00 01 01 01 01 29 46 */
  // data.servo1_control = SERVO_CONTROL_UP;
  // data.servo2_control = SERVO_CONTROL_UP;
  // data.servo3_control = SERVO_CONTROL_UP;
  // data.servo4_control = SERVO_CONTROL_UP;

  data_temp = &data;
  length_arr = CreateMessage(TYPE_MSG_CONTROL_SERVO, sizeof(servoControlData_t), (uint8_t *)data_temp, arr_out);
  Serial.println(length_arr);

  for (int i = 0; i < length_arr; i++)
  {
    char buff[5];
    if (arr_out[i] < 15)
    {
      sprintf(buff, "0%x", arr_out[i]);
    }
    else
    {
      sprintf(buff, "%x", arr_out[i]);
    }
    Serial.print(buff);
    Serial.print(" ");
    delay(10);
  }
#endif
}

// ESP.restart();
// C:\Users\nhanma\Documents\github\Frimware_TestRD\.pio\build\esp32doit-devkit-v1