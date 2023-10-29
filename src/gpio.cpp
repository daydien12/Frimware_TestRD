#include "gpio.h"

gpio_ButtonState_t button_1, button_2, button_3;
Servo servo1, servo2, servo3, servo4;

static void GPIO_LED_Init(void);
static void GPIO_CLQ_Init(void);
static void GPIO_Servo_Init(void);
static void GPIO_Button_Init(void);
static void GPIO_Button_Read(const uint8_t input_signal, gpio_ButtonState_t *button_var);

void GPIO_Init(void)
{
    GPIO_LED_Init();
    GPIO_CLQ_Init();
    GPIO_Servo_Init();
    GPIO_Button_Init();
}

static void GPIO_LED_Init(void)
{
    pinMode(IO_LED_1, OUTPUT);
    pinMode(IO_LED_2, OUTPUT);
    pinMode(IO_LED_3, OUTPUT);
}

static void GPIO_Servo_Init(void)
{
    servo1.attach(IO_SERVO_1);
    servo2.attach(IO_SERVO_2);
    servo3.attach(IO_SERVO_3);
    servo4.attach(IO_SERVO_4);
}

void GPIO_Led_Control(uint8_t led_pin, uint8_t led_mode)
{
    digitalWrite(led_pin, led_mode);
}

static void GPIO_CLQ_Init(void)
{
    pinMode(IO_CLQ_1, INPUT_MODE);
    pinMode(IO_CLQ_2, INPUT_MODE);
    pinMode(IO_CLQ_3, INPUT_MODE);
    pinMode(IO_CLQ_4, INPUT_MODE);
}

static void GPIO_Button_Init(void)
{
    button_1.flag_check = 0;
    button_2.flag_check = 0;
    button_3.flag_check = 0;

    pinMode(IO_BUTTON_1, INPUT_MODE);
    pinMode(IO_BUTTON_2, INPUT_MODE);
    pinMode(IO_BUTTON_3, INPUT_MODE);
}

void GPIO_Button_ReadAll(void)
{
    GPIO_Button_Read(BT1_READ, (gpio_ButtonState_t *)(&button_1));
    GPIO_Button_Read(BT2_READ, (gpio_ButtonState_t *)(&button_2));
    GPIO_Button_Read(BT3_READ, (gpio_ButtonState_t *)(&button_3));
}

void GPIO_ServoSet(uint8_t servo_mode, uint8_t degrees)
{
    if (servo_mode == 1)
    {
        servo1.write(degrees);
    }
    else if (servo_mode == 2)
    {
        servo2.write(degrees);
    }
    else if (servo_mode == 3)
    {
        servo3.write(degrees);
    }
    else if (servo_mode == 4)
    {
        servo4.write(degrees);
    }
}

static void GPIO_Button_Read(const uint8_t input_signal, gpio_ButtonState_t *button_var)
{
    if (input_signal == state_button)
    {
        if (button_var->count_times <= MAXINPUT)
        {
            button_var->count_times++;
            if (button_var->count_times == 50)
            {
                button_var->flag_check = 1;
            }
        }
    }
    else
    {
        button_var->count_times = 0;
    }
}