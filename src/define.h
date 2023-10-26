#ifndef LIB_DEFINE_h
#define LIB_DEFINE_h

#include <Arduino.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <time.h>

#define string_size 500
#define DB_INFO(message, ...)  Serial.printf(">>> (INFO) : " message , ##__VA_ARGS__)
#define DB_ERROR(message, ...) Serial.printf(">>> (ERROR): " message , ##__VA_ARGS__)
#define DB_DEBUG(message, ...) Serial.printf(">>> (DEBUG): " message , ##__VA_ARGS__)

#endif

