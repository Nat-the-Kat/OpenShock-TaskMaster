## **config.json contents**

The config.json contains the "config" object, which holds some basic configuration information. The best way I can explain each field is by going through the following example file.


```Json
{"config":{"ntp_server":"pool.ntp.org","api_key":"your_api_key","shocker":"your_shocker_id","can_override":true,"override_pin":1,"num_overrides":1,"reset_day": 0,"message_time":5,"reset_time":[0,0,0],"timezone":[-7,0,0]}}
```

Understand everything? Perfect! Nothing left to say then. I'm joking.


```Json
"ntp_server":"pool.ntp.org",
```

Allows specifying the NTP server the pico fetches its time from.


```Json
"api_key":"your_api_key",
```

Your OpenShock api key that will allow the pico to send control requests.


```Json
"shocker":"your_shocker_id",
```

The shocker that the pico will use for warnings/punishments.


```Json
"can_override":true,
"override_pin":1,
"num_overrides":1,
"reset_day": 0,
```

The bool "can_override" allows the daily override functionality. Basically, when enabled, allows for disabling all the tasks for the day if it senses 3.3v on the gpio pin "override_pin". "num_overrides" specifies how many times per week a days tasks can be overridden. the number of overrides used each week resets on "reset_day" at the "reset_time".

Note 1: "reset_day" refers to a day of the week, with sunday being 0, monday being 1, etc.

Note 2: if "can_override" is false, "override_pin", "num_overrides", and "reset_day" don't need to be added to config file, as they will just be ignored.


```Json
"message_time":5,
```

how long messages will stay on the oled, in seconds.


```Json
"reset_time":[0,0,0],
```

This array is used to specify a time (in 24 hour format) when the daily task override resets if it was used. It is also the time on "reset_day" when the week's daily task overrides reset.

Note 3: the values are ordered as follows: [hour,minute,second]


```Json
"timezone":[-7,0,0]
```

This array is used to specify the time offset from UTC. In this case it is set to PST as that is my local timezone. It also has the same format as "reset_time" (hour,minute,second).

Note 4: this offset does not account for daylight saving time, it is used in your part of the world. As far as I can tell, the arduino-pico core has no way to specify timezone.
