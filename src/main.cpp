
#include "define.h"
#include "wifis.h"
#include "gpio.h"
#include "mqtts.h"
#include "conver.h"
#include "eeproms.h"
#include "get_message.h"
#include "frame_message.h"
#include <esp_task_wdt.h>

/*
1: WiFi
2: Servo Info
3: Servo Control
4: MQTT
*/
#define CREATE_MESSAGE_TEST 4

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
  // Create_MSGTest();
  // EEPROM_CreateEEprom(1);
  EEPROM_Print();
  if (eeprom_data.create_data != 123)
  {
    EEPROM_CreateEEprom(1);
    delay(500);
    EEPROM_Print();
  }
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
          ESP.restart();
        }
        else if (dataout.TypeMessage == TYPE_MSG_CONTROL_SERVO)
        {
          servoControlData_t servo_control;
          Get_MessageControlServo(dataout, &servo_control);
          DB_DEBUG("(MAIN) SV1: %d, SV2: %d, SV3: %d, SV4: %d \n", servo_control.servo1_control, servo_control.servo2_control, servo_control.servo3_control, servo_control.servo4_control);
        }
        else if (dataout.TypeMessage == TYPE_MSG_UPDATE_MQTT)
        {
          Get_MessageConfigMqtt(dataout);
          delay(1000);
          ESP.restart();
        }
        // EEPROM_Print();
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

  if (mqtt_switch_press.flag == true)
  {
    DB_DEBUG("(MAIN.c) Servo: %d, %d, %d, %d\n", mqtt_switch_press.servo[0], mqtt_switch_press.servo[1], mqtt_switch_press.servo[2], mqtt_switch_press.servo[3]);
    DB_DEBUG("(MAIN.c) Time: %d, wait: %d, keep: %d,\n", mqtt_switch_press.time, mqtt_switch_press.wait, mqtt_switch_press.keep);
    mqtt_switch_press.flag = false;
    if (mqtt_switch_press.servo[0] == 1)
    {

      GPIO_ServoInitValue(servo1_value);
      servo1_value.flag = 1;
      servo1_value.servo_numbers = 1;
      servo1_value.keep = mqtt_switch_press.keep;
      servo1_value.time = mqtt_switch_press.time;
      servo1_value.wait = mqtt_switch_press.wait;
    }

    if (mqtt_switch_press.servo[1] == 1)
    {
      GPIO_ServoInitValue(servo2_value);
      servo2_value.flag = 1;
      servo2_value.servo_numbers = 2;
      servo2_value.keep = mqtt_switch_press.keep;
      servo2_value.time = mqtt_switch_press.time;
      servo2_value.wait = mqtt_switch_press.wait;
    }

    if (mqtt_switch_press.servo[2] == 1)
    {
      GPIO_ServoInitValue(servo3_value);
      servo3_value.flag = 1;
      servo3_value.servo_numbers = 3;
      servo3_value.keep = mqtt_switch_press.keep;
      servo3_value.time = mqtt_switch_press.time;
      servo3_value.wait = mqtt_switch_press.wait;
    }

    if (mqtt_switch_press.servo[3] == 1)
    {
      GPIO_ServoInitValue(servo4_value);
      servo4_value.flag = 1;
      servo4_value.servo_numbers = 4;
      servo4_value.keep = mqtt_switch_press.keep;
      servo4_value.time = mqtt_switch_press.time;
      servo4_value.wait = mqtt_switch_press.wait;
    }
  }

  if (mqtt_switch_power.flag == true)
  {
    mqtt_switch_power.flag = false;
    GPIO_RelayControl(mqtt_switch_power.status);
    DB_DEBUG("(MAIN.c) Status: %d,\n", mqtt_switch_power.status);
  }

  if (mqtt_switch_type.flag == true)
  {
    mqtt_switch_type.flag = false;
    DB_DEBUG("(MAIN.c) Type: %d,\n", mqtt_switch_type.type);
  }

  if (mqtt_switch_pinout.flag == true)
  {
    uint8_t clq_1 = 2, clq_2 = 2, clq_3 = 2, clq_4 = 2;
    mqtt_switch_pinout.flag = false;
    if (mqtt_switch_pinout.pinout[0] == 1)
    {
      clq_1 = digitalRead(IO_CLQ_1);
    }

    if (mqtt_switch_pinout.pinout[1] == 1)
    {
      clq_2 = digitalRead(IO_CLQ_2);
    }

    if (mqtt_switch_pinout.pinout[2] == 1)
    {
      clq_3 = digitalRead(IO_CLQ_3);
    }

    if (mqtt_switch_pinout.pinout[3] == 1)
    {
      clq_4 = digitalRead(IO_CLQ_4);
    }

    DB_DEBUG("(MAIN.c) Pinout: %d, %d, %d, %d\n", mqtt_switch_pinout.pinout[0], mqtt_switch_pinout.pinout[1], mqtt_switch_pinout.pinout[2], mqtt_switch_pinout.pinout[3]);
    MQTT_Response_OutputSwitch(eeprom_data.switch_number, clq_1, clq_2, clq_3, clq_4);
  }

  if (button_1.flag_check == true)
  {
    button_1.flag_check = false;
    DB_DEBUG("(MAIN.c) Button 1\n");
  }

  if (button_2.flag_check == true)
  {
    button_2.flag_check = false;
    DB_DEBUG("(MAIN.c) Button 2\n");
  }

  if (button_3.flag_check == true)
  {
    button_3.flag_check = false;
    DB_DEBUG("(MAIN.c) Button 3\n");
  }

  if (mqtt_flag_config == true)
  {
    DB_INFO("(MAIN.c)  Messenger MQTT Config \n");
    DB_INFO("(MAIN.c)  wifi_ssid: %s \n", eeprom_data.wifi_ssid);
    DB_INFO("(MAIN.c)  wifi_pass: %s \n", eeprom_data.wifi_pass);
    DB_INFO("(MAIN.c)  mqtt_port: %d \n", eeprom_data.mqtt_port);
    DB_INFO("(MAIN.c)  mqtt_broker: %s \n", eeprom_data.mqtt_broker);
    DB_INFO("(MAIN.c)  mqtt_username: %s \n", eeprom_data.mqtt_username);
    DB_INFO("(MAIN.c)  mqtt_password: %s \n", eeprom_data.mqtt_password);
    EEPROM_WriteStruct(&eeprom_data);
    mqtt_flag_config = false;
    delay(1000);
    ESP.restart();
  }

  GPIO_ServoRun(servo1_value);
  GPIO_ServoRun(servo2_value);
  GPIO_ServoRun(servo3_value);
  GPIO_ServoRun(servo4_value);
  delayMicroseconds(1);
}

void IRAM_ATTR Timer_Handle()
{
  GPIO_Button_ReadAll();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  char buffer[length];
  DB_INFO("(MAIN.c) Message arrived [%s] topic \n", topic);
  for (int i = 0; i < length; i++)
  {
    buffer[i] = payload[i];
  }
  DB_INFO("(MAIN.c) Message:\n %s \n", buffer);
  MQTT_Receive(buffer, topic);
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

  // uint8_t lenth_ssid = strlen("P702_2G");
  // uint8_t lenth_pass = strlen("nhanma25");

  uint8_t lenth_ssid = strlen("Rang Dong Lab_2G");
  uint8_t lenth_pass = strlen("ktdt123456");

  wifiConfigData_t data, *data_temp;
  data.wifi_timeout = 20;
  memcpy(data.wifi_ssid, "Rang Dong Lab_2G", lenth_ssid);
  memcpy(data.wifi_pass, "ktdt123456", lenth_pass);

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
    // RD lab_2g a5 01 44 00 14 00 52 61 6e 67 20 44 6f 6e 67 20 4c 61 62 5f 32 47 00 34 08 40 30 0a 06 00 48 a8 08 80 b0 71 fc 3f 6b 74 64 74 31 32 33 34 35 36 00 ff 23 0a 06 00 20 0a 06 00 01 00 00 00 d0 34 08 80 90 71 fc 3f e5 8c
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

#elif CREATE_MESSAGE_TEST == 4
  uint8_t lenth_mqtt_broker = strlen("Mqtt.mysignage.vn123");
  uint8_t lenth_mqtt_ssid = strlen("admin");
  uint8_t lenth_mqtt_pass = strlen("admin");
  uint16_t mqtt_port = 1883;

  mqttConfigData_t data, *data_temp;
  data.mqtt_port = mqtt_port;
  memcpy(data.mqtt_broker, "Mqtt.mysignage.vn123", lenth_mqtt_broker);
  memcpy(data.mqtt_username, "admin", lenth_mqtt_ssid);
  memcpy(data.mqtt_password, "admin", lenth_mqtt_pass);

  data.mqtt_broker[lenth_mqtt_broker] = 0;
  data.mqtt_username[lenth_mqtt_ssid] = 0;
  data.mqtt_password[lenth_mqtt_pass] = 0;

  data_temp = &data;
  length_arr = CreateMessage(TYPE_MSG_UPDATE_MQTT, sizeof(mqttConfigData_t), (uint8_t *)data_temp, arr_out);
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
//  a5 04 64 00 5b 07 4d 71 74 74 2e 6d 79 73 69 67 6e 61 67 65 2e 76 6e 00 08 40 00 00 00 00 00 00 00 00 5c 03 fb 3f 61 64 6d 69 6e 00 00 00 00 00 00 00 04 46 08 40 f0 34 08 40 30 0a 06 00 2c a9 08 80 50 72 fc 3f 61 64 6d 69 6e 00 00 00 ff ff ff ff 23 0a 06 00 20 0a 06 00 01 00 00 00 f0 34 08 80 30 72 fc 3f 64 c4
//  a5 04 64 00 5b 07 4d 71 74 74 2e 6d 79 73 69 67 6e 61 67 65 2e 76 6e 31 32 33 00 00 00 00 00 00 00 00 4b 02 0e 80 61 64 6d 69 6e 00 00 00 ff ff 3f b3 5c 49 08 40 c4 36 08 40 30 08 06 00 88 e0 08 80 20 c2 fc 3f 61 64 6d 69 6e 00 00 00 ff ff ff ff 23 08 06 00 20 08 06 00 01 00 00 00 c4 36 08 80 00 c2 fc 3f 35 63
#endif
}
// ESP.restart();
// C:\Users\nhanma\Documents\github\Frimware_TestRD\.pio\build\esp32doit-devkit-v1