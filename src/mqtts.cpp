#include "mqtts.h"

// const char *mqtt_broker = "Mqtt.mysignage.vn";
// const char *mqtt_username = "admin";
// const char *mqtt_password = "admin";
// const int mqtt_port = 1883;
const char *topic = "topic1";
const char *topic_sub_switch = "host\\switch";
const char *topic_pub_switch = "client\\switch";

const char *topic_sub_wireless = "host\\wireless";
const char *topic_pub_wireless = "client\\wireless";
WiFiClient espClient;
PubSubClient client(espClient);

static void MQTT_Reconnect(void)
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        DB_INFO("(MQTTS.c)Attempting MQTT connection...\n");
        // Create a random client ID
        String clientId = "ESP32lient-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str(), eeprom_data.mqtt_username, eeprom_data.mqtt_password))
        {
            DB_INFO("(MQTTS.c)MQTT Connect: %s\n", (char *)eeprom_data.mqtt_broker);
            // Once connected, publish an announcement...
            client.publish(topic, "hello world!");
            // ... and resubscribe
            client.subscribe(topic_sub_switch);
            client.subscribe(topic_sub_wireless);
            // MQTT_Response_GetInfoWireless(5);
            // delay(1000);
            // MQTT_Response_SetConfigWireless(3);
            // delay(1000);
            // MQTT_Response_PowerSwitch(1);
            // delay(1000);
            // MQTT_Response_OutputSwitch(2, 1, 1, 1, 0);
            // delay(1000);
            // MQTT_Response_PressSwitch(3);
            // delay(1000);
        }
        else
        {
            DB_ERROR("(MQTTS.c)MQTT Connect failed, rc=: %s\n", (char *)eeprom_data.mqtt_broker);
            DB_ERROR("(MQTTS.c)%d\n", client.state());
            DB_ERROR("(MQTTS.c)try again in 5 seconds\n");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void MQTT_Init(void (*callback)(char *, byte *, unsigned int))
{
    client.setServer(eeprom_data.mqtt_broker, eeprom_data.mqtt_port);
    client.setCallback(callback);
}

void MQTT_Run(void)
{
    if (!client.connected())
    {
        MQTT_Reconnect();
    }
    client.loop();
}

void MQTT_Response_PowerSwitch(uint8_t switch_id)
{
    StaticJsonDocument<100> doc;
    char jsonChar[100];
    doc["switch"] = switch_id;
    doc["cmd"] = "power";
    // serializeJson(doc, Serial);
    serializeJson(doc, jsonChar);
    client.publish(topic_pub_switch, jsonChar);
}

void MQTT_Response_PressSwitch(uint8_t switch_id)
{
    StaticJsonDocument<100> doc;
    char jsonChar[100];
    doc["switch"] = switch_id;
    doc["cmd"] = "press";
    // serializeJson(doc, Serial);
    serializeJson(doc, jsonChar);
    client.publish(topic_pub_switch, jsonChar);
}

void MQTT_Response_OutputSwitch(uint8_t switch_id, uint8_t out1, uint8_t out2, uint8_t out3, uint8_t out4)
{
    StaticJsonDocument<500> doc;
    char jsonChar[500];
    doc["switch"] = switch_id;
    doc["cmd"] = "output";

    JsonArray data = doc.createNestedArray("status");
    data.add(out1);
    data.add(out2);
    data.add(out3);
    data.add(out4);
    // serializeJson(doc, Serial);
    serializeJson(doc, jsonChar);
    client.publish(topic_pub_switch, jsonChar);
}

void MQTT_Response_SetConfigWireless(uint8_t switch_id)
{
    StaticJsonDocument<100> doc;
    char jsonChar[100];
    doc["switch"] = switch_id;
    doc["cmd"] = "response";
    // serializeJson(doc, Serial);
    serializeJson(doc, jsonChar);
    client.publish(topic_pub_wireless, jsonChar);
}

void MQTT_Response_GetInfoWireless(uint8_t switch_id)
{
    StaticJsonDocument<500> doc;
    char jsonChar[500];
    char ip[30];
    EEPROM_ReadStruct(&eeprom_data);
    doc["switch"] = switch_id;
    doc["cmd"] = "post";
    doc["ssid"] = eeprom_data.wifi_ssid;
    doc["pass"] = eeprom_data.wifi_pass;
    doc["host"] = eeprom_data.mqtt_broker;
    doc["port"] = eeprom_data.mqtt_port;
    doc["username"] = eeprom_data.mqtt_username;
    doc["password"] = eeprom_data.mqtt_password;
    sprintf(ip, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    doc["ip"] = ip;
    serializeJson(doc, Serial);
    serializeJson(doc, jsonChar);
    client.publish(topic_pub_wireless, jsonChar);
}

void MQTT_Receive(char *datain)
{
    StaticJsonDocument<300> doc;
    DeserializationError error = deserializeJson(doc, datain);
    // // Test if parsing succeeds.
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }
    int sensor = doc["switch"];
    String cmd = doc["cmd"];
    if (cmd == "press")
    {
        MQTT_Response_PressSwitch(1);
        DB_DEBUG("Press");
    }
    // else if (!strcmp(cmd, "power"))
    // {
    //     MQTT_Response_PowerSwitch(1);
    // }
    // else if (!strcmp(cmd, "output"))
    // {
    //     MQTT_Response_OutputSwitch(1, 1, 1, 0, 1);
    // }
    DB_DEBUG("SW: %d, CMD: %s\n", sensor, cmd);
}