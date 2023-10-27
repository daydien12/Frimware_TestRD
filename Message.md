# Control Servo
### Topic: `client\servo\control`
* switch: switch'order in line (0-255)
* servo: numerical order of servo (0, 1, 2, 3, 10), with servo=10 is control all
* status: touch or non-touch (1 or 0)
```
{
    "switch": 0,
    "servo": 0,
    "status": 1
}
``` 
# Setup rotation of servo
### Topic: `client\servo\setup`
* switch: switch'order in line (0-255)
* servo: numerical order of servo 0, 1, 2, 3
* on: rotation when touch 0 - 180
* off: rotation when non-touch 0 - 180
```
{
    "switch": 0,
    "servo": 0,
    "on": 60,
    "off": 100
}
```
# Power 
### Topic: `client\switch\power`
* switch: switch'order in line (0-255)
* power: supply AC for switch 0-off, 1-on
```
{
    "switch": 0,
    "power": 0
}
```

# Reading Output
### Topic: `host\switch\output`
* switch: switch'order in line (0-255)
* output: output'order of switch (0, 1, 2, 3)
* status: digital status of AC output (0-off, 1-on)
```
{
    "switch":0,
    "output":0,
    "status":1
}
```

# Changing MQTT
### Topic: `client\wireless\mqtt`
* switch: switch'order in line (0-255)
```
{
    "switch": 0,
    "host": "192.168.1.100",
    "port": 1883,
    "username": "RD",
    "password": "1"
}
```
### Topic: `host\wireless\mqtt`
Response for Changing mqtt message
```
{
    "switch": 0
}
```
# Changing Wifi
### Topic: `client\wireless\wifi`
* switch: switch'order in line (0-255)
```
{
    "switch": 0,
    "ssid": "Rang Dong Lab",
    "password": "12345678"
}
```
### Topic: `host\wireless\wifi`
Response for Changing wifi message
```
{
    "switch": 0
}
```
# Post Configure
### Topic: `host\wireless\configure`
```
{
    "switch": 0,
    "ssid": "Rang Dong Lab",
    "password": "12345678",
    "host": "192.168.1.100",
    "port": 1883,
    "username": "RD",
    "password": "1"
}
```
# Post IP Address
```
{
    "switch": 0,
    "ip": "192.168.1.10"
}
```