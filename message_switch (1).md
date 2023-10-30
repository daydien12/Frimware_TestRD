
# Topic `host\switch`
* switch: switch'order in line (0-255)
```
{
    "switch": 0,
    "cmd":"press",
    "servo":[0,1,1,1],
    "time":2,
    "wait":2,
    "keep":2,
}
```
```
{
    "switch": 0,
    "cmd":"power",
    "status": "on"
}
```
# Topic `client\switch`
```
{
    "switch": 0,
    "cmd":"power",
    "status":[0,1,1,1]
}
```


# Topic: `host\wireless`
```
{
    "switch": 0,
    "cmd": "set"
    "ssid": "Rang Dong Lab",
    "password": "12345678",
    "host": "192.168.1.100",
    "port": 1883,
    "username": "RD",
    "password": "1"
}
```

```
{
    "switch": 0,
    "cmd": "get"
}
```
# Topic: `client\wireless`
### Response for command "set"
```
{
    "switch": 0
    "cmd": "response"
}
```
### Response for command "get"
```
{
    "switch": 0,
    "cmd": "post"
    "ssid": "Rang Dong Lab",
    "password": "12345678",
    "host": "192.168.1.100",
    "port": 1883,
    "username": "RD",
    "password": "1",
    "ip": "192.168.1.10"
}
```
