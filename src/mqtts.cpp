#include "mqtts.h"

const char *mqtt_broker = "Mqtt.mysignage.vn";
const char *mqtt_username = "admin";
const char *mqtt_password = "admin";
const int mqtt_port = 1883;
const char *topic = "topic";

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
        if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
        {
            DB_INFO("(MQTTS.c)MQTT Connect: %s\n", mqtt_broker);
            // Once connected, publish an announcement...
            client.publish(topic, "hello world!");
            // ... and resubscribe
            client.subscribe(topic);
        }
        else
        {
            DB_ERROR("(MQTTS.c)MQTT Connect failed, rc=: %s\n", mqtt_broker);
            DB_ERROR("(MQTTS.c)%d\n", client.state());
            DB_ERROR("(MQTTS.c)try again in 5 seconds\n");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void MQTT_Init(void (*callback)(char *, byte *, unsigned int))
{
    client.setServer(mqtt_broker, mqtt_port);
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