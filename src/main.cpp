
#include "define.h"

uint16_t lenght = 0;
hw_timer_t *my_timer = NULL;
uint8_t arr_receive[string_size];

static void TIMER_Init(void);
static void Create_MSGTest(void);

void setup()
{
  GPIO_Init();
  EEPROM_Init();
  Serial.begin(115200);
  // Create_MSGTest();
  //EEPROM_TestWrite();
  EEPROM_TestRead();
  delay(1000);
  WIFI_Connect();
  
  TIMER_Init();
}

void loop()
{
  if (Serial.available() > 0)
  {
    BTS_Get_Message(Serial.read(), arr_receive);
  }

  if (Is_Message(&lenght) != 0)
  {
    if (lenght > 0)
    {
      messageFrameMsg_t dataout;
      printf("\n-(Size    : %d)-\n", DetectMessage(arr_receive, &dataout));
      DebugMessage(arr_receive);
      ESP.restart();
    }
  }

  delay(1);
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
  uint8_t arr_in[2] = {1, 2}, arr_out[20], length_arr;
  length_arr = CreateMessage(TYPE_MSG_UPDATE_WIFI, 2, arr_in, arr_out);
  Serial.println(length_arr);
  for (int i = 0; i < length_arr; i++)
  {
    char buff[10];
    if (arr_out[i] < 10)
    {
      sprintf(buff, "0%x", arr_out[i]);
    }
    else
    {
      sprintf(buff, "%x", arr_out[i]);
    }

    Serial.print(buff);
    Serial.print(" ");
  }
}