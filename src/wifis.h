#ifndef LIB_WIFI_h
#define LIB_WIFI_h
#include "define.h"
#include "eeproms.h"
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>



int8_t WIFI_Connect(void);
void WIFI_Ota_Init(void);
void WIFI_Ota_Run(void);
#endif
