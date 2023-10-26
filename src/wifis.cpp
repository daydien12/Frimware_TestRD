#include "wifis.h"

int8_t WIFI_Connect(void)
{
    uint16_t count_timeout = 0;
    WiFi.mode(WIFI_STA);
    WiFi.begin(eeprom_data.wifi_ssid, eeprom_data.wifi_pass);
    DB_DEBUG("Connecting to: %s, timeout: %d S\n", eeprom_data.wifi_ssid, eeprom_data.wifi_timeout / 10);
    while (WiFi.status() != WL_CONNECTED)
    {
        if (count_timeout >= eeprom_data.wifi_timeout)
        {
            count_timeout = 0;
            break;
        }
        else
        {
            DB_DEBUG(".\n");
            count_timeout += 1;
        }
        delay(100);
    }
    if (count_timeout < eeprom_data.wifi_timeout)
    {
        DB_DEBUG("Connected to: %s, IP: %d.%d.%d.%d\n", eeprom_data.wifi_ssid, WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    }
    else
    {
        DB_ERROR("Connected failed! \n");
        return 0;
    }

    return 1;
}