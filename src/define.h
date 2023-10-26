#ifndef LIB_DEFINE_h
#define LIB_DEFINE_h

#include <Arduino.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <time.h>
#define SERVO_CONTROL_UP 1
#define SERVO_CONTROL_DOWN 0
#define string_size 500

#define DB_WIFI 0
#define DB_GET_MESSAGE 0

#define DB_FUNC_ERROR (-1)

#define DB_INFO(message, ...) Serial.printf(">>> (INFO) : " message, ##__VA_ARGS__)
#define DB_ERROR(message, ...) Serial.printf(">>> (ERROR): " message, ##__VA_ARGS__)
#define DB_DEBUG(message, ...) Serial.printf(">>> (DEBUG): " message, ##__VA_ARGS__)

#endif
