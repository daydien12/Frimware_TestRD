#include "mqtts.h"

// const char *mqtt_broker = "Mqtt.mysignage.vn";
// const char *mqtt_username = "admin";
// const char *mqtt_password = "admin";
// const int mqtt_port = 1883;
/*client\switch*/
/*client\wireless*/

const char *topic = "topic1";
const char *topic_sub_switch = "host\\switch";
const char *topic_pub_switch = "client\\switch";

const char *topic_sub_wireless = "host\\wireless";
const char *topic_pub_wireless = "client\\wireless";
WiFiClient espClient;
PubSubClient client(espClient);

bool mqtt_flag_config = false;
mqttSwitchPress_t mqtt_switch_press;
mqttSwitchPower_t mqtt_switch_power;
mqttSwitchType_t mqtt_switch_type;
mqttSwitchPinout_t mqtt_switch_pinout;

/*switch sub*/
static void MQTT_GetMsgPress(JsonDocument &doc);
static void MQTT_GetMsgPower(JsonDocument &doc);
static void MQTT_GetMsgType(JsonDocument &doc);
static void MQTT_GetMsgPinout(JsonDocument &doc);
/*switch pub*/
static void MQTT_Response_Switch(uint8_t switch_id, char *cmd);

/*switch sub*/
static void MQTT_GetInfoWireless(JsonDocument &doc);
static void MQTT_SetConfigWireless(JsonDocument &doc);

/*wireless pub*/
static void MQTT_Response_GetInfoWireless(uint8_t switch_id);
static void MQTT_Response_SetConfigWireless(uint8_t switch_id);

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

void MQTT_Response_Switch(uint8_t switch_id, char *cmd)
{
    StaticJsonDocument<100> doc;
    char jsonChar[100];
    doc["switch"] = switch_id;
    doc["cmd"] = cmd;
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
    // serializeJsonPretty(doc, Serial);
    serializeJson(doc, jsonChar);
    client.publish(topic_pub_wireless, jsonChar);
}

void MQTT_Receive(char *datain, char *topic)
{
    StaticJsonDocument<300> doc;
    DeserializationError error = deserializeJson(doc, datain);

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    int switch_number = doc["switch"];
    String cmd = doc["cmd"];
    String topic_value = String(topic);
    if (topic_value == String(topic_sub_switch))
    {
        if (cmd == "press")
        {
            MQTT_GetMsgPress(doc);
            MQTT_Response_Switch(1, "press");
            DB_DEBUG("(MQTTS.c) MQTT Pub to topic %s: Press\n", topic_pub_switch);
        }
        else if (cmd == "power")
        {
            MQTT_GetMsgPower(doc);
            MQTT_Response_Switch(1, "power");
            DB_DEBUG("(MQTTS.c) MQTT Pub to topic %s: Power\n", topic_pub_switch);
        }
        else if (cmd == "type")
        {
            MQTT_GetMsgType(doc);
            MQTT_Response_Switch(1, "type");
            DB_DEBUG("(MQTTS.c) MQTT Pub to topic %s: Type\n", topic_pub_switch);
        }
        else if (cmd == "pinout")
        {
            MQTT_GetMsgPinout(doc);
        }
    }
    else if (topic_value == String(topic_sub_wireless))
    {
        if (cmd == "set")
        {
            MQTT_GetInfoWireless(doc);
            MQTT_Response_SetConfigWireless(1);
            DB_DEBUG("(MQTTS.c) MQTT Pub to topic %s: Set Config Wireless\n", topic_pub_wireless);
        }
        else if (cmd == "get")
        {
            MQTT_Response_GetInfoWireless(1);
            DB_DEBUG("(MQTTS.c) MQTT Pub to topic %s: Get Info Wireless\n", topic_pub_wireless);
        }
    }

    DB_DEBUG("SW: %d, CMD: %s, TOPIC: %s\n", switch_number, cmd, topic_value);
}

static void MQTT_GetMsgPress(JsonDocument &doc)
{
    int arraySize = doc["servo"].size();
    for (int i = 0; i < arraySize; i++)
    {
        mqtt_switch_press.servo[i] = doc["servo"][i];
    }
    mqtt_switch_press.time = doc["time"];
    mqtt_switch_press.wait = doc["wait"];
    mqtt_switch_press.keep = doc["keep"];
    mqtt_switch_press.flag = 1;
}

static void MQTT_GetMsgPower(JsonDocument &doc)
{
    String cmd = doc["status"];
    if (cmd == "on")
    {
        mqtt_switch_power.status = 1;
    }
    else if (cmd == "off")
    {
        mqtt_switch_power.status = 0;
    }
    mqtt_switch_power.flag = 1;
}

static void MQTT_GetMsgType(JsonDocument &doc)
{
    mqtt_switch_type.type = doc["type"];
    mqtt_switch_type.flag = 1;
}

static void MQTT_GetMsgPinout(JsonDocument &doc)
{
    int arraySize = doc["pinout"].size();
    for (int i = 0; i < arraySize; i++)
    {
        mqtt_switch_pinout.pinout[i] = doc["pinout"][i];
    }
    mqtt_switch_pinout.flag = 1;
}

static void MQTT_GetInfoWireless(JsonDocument &doc)
{
    eeprom_data.mqtt_port = doc["port"];
    for (int i = 0; i < sizeof(eeprom_data.wifi_ssid); i++)
    {
        eeprom_data.wifi_ssid[i] = 0;
        eeprom_data.wifi_pass[i] = 0;
        eeprom_data.mqtt_broker[i] = 0;
        eeprom_data.mqtt_username[i] = 0;
        eeprom_data.mqtt_password[i] = 0;
    }
    String data_ssid = doc["ssid"];
    memcpy(eeprom_data.wifi_ssid, data_ssid.c_str(), data_ssid.length());

    String data_pass = doc["pass"];
    memcpy(eeprom_data.wifi_pass, data_pass.c_str(), data_pass.length());

    String data_broker = doc["host"];
    memcpy(eeprom_data.mqtt_broker, data_broker.c_str(), data_broker.length());

    String data_username = doc["username"];
    memcpy(eeprom_data.mqtt_username, data_username.c_str(), data_username.length());

    String data_password = doc["password"];
    memcpy(eeprom_data.mqtt_password, data_password.c_str(), data_password.length());
    mqtt_flag_config = true;
}