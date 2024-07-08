## **config.json contents**

the config.json contains the "config" object, which holds some basic configuration information. the best way i can explain each field is by going through the following example file.


```Json
{"config":{"ntp_server":"pool.ntp.org", "os_config":{"server":"api.openshock.app","api_key":"your_api_key","shocker":"your_shocker_id"},"can_override":true,"override_pin":1,"num_overrides":1,"reset_day": 0,"message_time":5,"reset_time":[0,0,0],"timezone":[-7,0,0]}}
```

understand everything? perfect! nothing left to say then. i'm joking.


```Json
"ntp_server":"pool.ntp.org",
```

allows specifying the ntp server the pico fetches its time from.


```Json
"os_config":{
  "server":"api.openshock.app",
  "api_key":"your_api_key",
  "shocker":"your_shocker_id"
  }
```

"server" allows you to specify a custom OpenShock implementation, if you wish. 
"api_key" is your OpenShock api key that will allow the pico to send control requests.
"shocker_id" is the shocker that the pico will use for warnings/punishments. currently only supports 1.


```Json
"can_override":true,
"override_pin":1,
"num_overrides":1,
"reset_day": 0,
```

the bool "can_override" allows the daily override functionality. basically, when enabled, allows for disabling all the tasks for the day if it senses 3.3v on the gpio pin "override_pin". "num_overrides" specifies how many times per week a days tasks can be overridden. the number of overrides used each week resets on "reset_day" at the "reset_time".

note 1: "reset_day" refers to a day of the week, with sunday being 0, monday being 1, etc.

note 2: if "can_override" is false, "override_pin", "num_overrides", and "reset_day" don't need to be added to config file, as they will just be ignored.


```Json
"message_time":5,
```

how long messages will stay on the oled, in seconds.


```Json
"reset_time":[0,0,0],
```

this array is used to specify a time (in 24 hour format) when the daily task override resets if it was used. it is also the time on "reset_day" when the week's daily task overrides reset.

note 3: the values are ordered as follows: [hour,minute,second]


```Json
"timezone":[-7,0,0]
```

this array is used to specify the time offset from utc. in this case it is set to pst as that is my local timezone. it also has the same format as "reset_time" (hour,minute,second).

note 4: this offset does not account for daylight saving time, it is used in your part of the world. as far as I can tell, the arduino-pico core has no way to specify timezone.
