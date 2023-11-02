#include "gpio.h"

gpio_ButtonState_t button_1, button_2, button_3;
Servo servo1, servo2, servo3, servo4;
servoControlstate_t servo1_value, servo2_value, servo3_value, servo4_value;
static void GPIO_LED_Init(void);
static void GPIO_CLQ_Init(void);
static void GPIO_Servo_Init(void);
static void GPIO_Button_Init(void);
static void GPIO_Button_Read(const uint8_t input_signal, gpio_ButtonState_t *button_var);

static void Servo_ControlUp(uint8_t servo_number);
static void Servo_ControlDown(uint8_t servo_number);

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

    GPIO_ServoInitValue(servo1_value);
    GPIO_ServoInitValue(servo2_value);
    GPIO_ServoInitValue(servo3_value);
    GPIO_ServoInitValue(servo4_value);
}

void GPIO_ServoInitValue(servoControlstate_t &servo_value)
{
    servo_value.flag = 0;
    servo_value.time = 0;
    servo_value.wait = 0;
    servo_value.keep = 0;
    servo_value.state = 0;
    servo_value.servo_numbers = 0;
    servo_value.time_last = 0;
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

static void Servo_ControlUp(uint8_t servo_number)
{
    DB_DEBUG("Servo_%d: Up\n", servo_number);
    if (servo_number == 1)
    {
        servo1.write(eeprom_data.servo1_up);
    }
    else if (servo_number == 2)
    {
        servo2.write(eeprom_data.servo2_up);
    }
    else if (servo_number == 3)
    {
        servo3.write(eeprom_data.servo3_up);
    }
    else if (servo_number == 4)
    {
        servo4.write(eeprom_data.servo4_up);
    }
}

void Servo_ControlDown(uint8_t servo_number)
{
    DB_DEBUG("Servo_%d: Down\n", servo_number);
    if (servo_number == 1)
    {
        servo1.write(eeprom_data.servo1_down);
    }
    else if (servo_number == 2)
    {
        servo2.write(eeprom_data.servo2_down);
    }
    else if (servo_number == 3)
    {
        servo3.write(eeprom_data.servo3_down);
    }
    else if (servo_number == 4)
    {
        servo4.write(eeprom_data.servo4_down);
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

void GPIO_ServoRun(servoControlstate_t &servo_value)
{
    if (servo_value.flag == 1)
    {
        switch (servo_value.state)
        {
        case 0:
            servo_value.state = 1;

            Servo_ControlDown(servo_value.servo_numbers);
            servo_value.time_last = millis();
            break;
        case 1:
            if ((millis() - servo_value.time_last) >= (servo_value.wait * 1000))
            {
                Servo_ControlUp(servo_value.servo_numbers);
                servo_value.state = 2;
                servo_value.time_last = millis();
            }
            break;
        case 2:
            if ((millis() - servo_value.time_last) >= (servo_value.keep * 1000))
            {
                servo_value.state = 0;
                if (servo_value.time > 1)
                {
                    DB_DEBUG("Servo_%d Time: %d\n", servo_value.servo_numbers, servo_value.time);
                    servo_value.time--;
                }
                else
                {
                    servo_value.flag = 0;
                    DB_DEBUG("Servo_%d Time End: %d\n", servo_value.servo_numbers, servo_value.time);
                }
            }
            break;
        default:
            break;
        }
    }
}