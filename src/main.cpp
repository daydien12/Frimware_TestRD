
#include "define.h"
#include "wifis.h"
#include "gpio.h"
#include "conver.h"
#include "eeproms.h"
#include "get_message.h"
#include "frame_message.h"

uint16_t lenght = 0, count_time_1ms = 0;
hw_timer_t *my_timer = NULL;
uint8_t arr_receive[string_size];

uint8_t flag_wifi_connect = 0;
static void TIMER_Init(void);
static void Create_MSGTest(void);

void setup()
{
  GPIO_Init();
  EEPROM_Init();
  Serial.begin(115200);

  // EEPROM_TestWrite();
  EEPROM_TestRead();
  delay(1000);
  flag_wifi_connect = WIFI_Connect();
  TIMER_Init();

  Create_MSGTest();
}

void loop()
{
  if (Serial.available() > 0)
  {
    Get_Message(Serial.read(), arr_receive);
  }

  if (count_time_1ms >= 1000)
  {
    if (Is_Message(&lenght) != 0)
    {
      if (lenght > 0)
      {
        messageFrameMsg_t dataout;
        printf("\n-(Size    : %d)-\n", DetectMessage(arr_receive, &dataout));
        DebugMessage(arr_receive);
        //Get_MessageConfigWifi(dataout);
        // ESP.restart();
      }
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

  uint8_t lenth_ssid = strlen("Ai-Photonic 2G");
  uint8_t lenth_pass = strlen("ptitlab@123");

  uint8_t arr_out[200], length_arr;
  wifiConfigData_t data, *data_temp;
  data.wifi_timeout = 20;
  memcpy(data.wifi_ssid, "Ai-Photonic 2G", lenth_ssid);
  memcpy(data.wifi_pass, "ptitlab@123",    lenth_pass);

  data.wifi_ssid[lenth_ssid] = 0;
  data.wifi_pass[lenth_pass] = 0;

  data_temp = &data;
  length_arr = CreateMessage(TYPE_MSG_UPDATE_WIFI, sizeof(wifiConfigData_t), (uint8_t *)data_temp, arr_out);
  Serial.println(length_arr);
  DB_DEBUG("%s %s", data.wifi_ssid,  data.wifi_pass);
  for (int i = 0; i < sizeof(wifiConfigData_t); i++)
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
}