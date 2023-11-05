#ifndef LIB_GPIOS_h
#define LIB_GPIOS_h
#include "define.h"
#include "eeproms.h"

/*Define IO Relay*/
#define IO_RELAY 5

/*Define IO Servo*/
#define IO_SERVO_1 15
#define IO_SERVO_2 4
#define IO_SERVO_3 16
#define IO_SERVO_4 17

/*Define IO Led*/
#define IO_LED_1 32
#define IO_LED_2 25
#define IO_LED_3 27

/*Define IO Switch*/
#define IO_BUTTON_1 21
#define IO_BUTTON_2 19
#define IO_BUTTON_3 18

/*Define IO Led*/
#define IO_CLQ_1 27
#define IO_CLQ_2 26
#define IO_CLQ_3 25
#define IO_CLQ_4 33

/*Config Button */
#define MAXINPUT (1000)
#define INPUT_MODE INPUT_PULLUP
#define state_button 0

#define BT1_READ digitalRead(IO_BUTTON_1)
#define BT2_READ digitalRead(IO_BUTTON_2)
#define BT3_READ digitalRead(IO_BUTTON_3)

typedef struct
{
	uint8_t flag_check;
	uint16_t count_times;
} gpio_ButtonState_t;
extern gpio_ButtonState_t button_1, button_2, button_3;

typedef struct
{
	uint8_t flag;
	uint8_t time;
	uint8_t wait;
	uint8_t keep;
	uint8_t state;
	uint8_t servo_numbers;
	unsigned long time_last;
} servoControlstate_t;
extern servoControlstate_t servo1_value, servo2_value, servo3_value, servo4_value;

void GPIO_Init(void);
void GPIO_Button_ReadAll(void);
void GPIO_Led_Control(uint8_t led_pin, uint8_t led_mode);
void GPIO_CLQ_Read(uint8_t led_pin, uint8_t led_mode);
void GPIO_RelayControl(uint8_t mode);

void GPIO_ServoInitValue(servoControlstate_t &servo_value);
void GPIO_ServoRun(servoControlstate_t &servo_value);
void GPIO_ServoPressAndRelease(uint8_t servo_mode, uint8_t time, uint8_t wait, uint8_t keep);
#endif
