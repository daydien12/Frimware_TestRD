#include "wifis.h"

int8_t WIFI_Connect(void)
{
    WiFi.mode(WIFI_STA); // Optional
    WiFi.begin(eeprom_data.wifi_ssid, eeprom_data.wifi_pass);
    Serial.println("\nConnecting");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    return 1;
}