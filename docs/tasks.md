# **tasks.json contents**


Tasks.json contains the "tasks" array. For simplicity the example document will only have one task. The following is the example.


```Json
{"tasks":[{"name":"wake up","type":1,"can_punish":true,"can_warn":true,"can_reward":true,"punish_time":[8,45,0],"warn_time":[8,30,0],"punishment":{"intensity":100,"type":"Shock","duration":1000,"message":"You really should have gotten up by now..."},"warning":{"intensity":100,"type":"Vibrate","duration":1000,"message":"Its time to wake up"},"reward_message":"Good morning!","gpio":3}]}
```
lets break it apart.

```Json
"name":"wake up",
```

The name of the task. This is only used for housekeeping (deleting a task from the list).


```Json
"type":1,
```

This is used to specify what type of task this json object is referring to, and what (if any) extra fields are in the object. For simplicity I'll use the base task in this example. More about different task types and their extra fields later in this document.


```Json
"can_punish":true,
"can_warn": true,
"can_reward":true,
```

"can_punish" is used to signify if there is a "punishment" object attached to this task. if it is false, "punishment" and "punish_time" does not need to be added as they will just be ignored.

"can_warn" is used to signify if there is a "warning" object attached to this task. if it is false, "warning" and "warn_time" does not need to be added as they will just be ignored. 

"can_reward" is used to signify if there is a "reward_message" that will appear if this task is completed before the warning or punishment, whatever comes first. if it is false, "reward_message" does not  need to be added as it will just be ignored.

```Json
"punish_time":[8,45,0],
"warn_time":[8,30,0],
```
"punish_time" is when the "punishment" will be sent to the OpenShock API.

"warn_time" is the same, only "warning" will be sent instead.

to keep this document relatively short, we will only look at "punishment". just know that "warning" is formatted the same way.

```Json
"punishment":{
"intensity":100,
"type":"Shock",
"duration":1000,
"message":"you really should've gotten up by now..."
},
```
"intensity" is how strong the shock will be. it has a valid range of 0-100.

"type" is what type of control request will be sent. valid values are: "Sound","Shock","Vibrate"

"duration" is how long the shock will last, in milliseconds. it has a valid range of 300 - 30000.

"message" is what will be written to the screen when administering the shock. it will also appear in the shockers log.

Note 1: message should 96 characters long, anything longer will go off screen.

```Json
"reward_message":"Good morning!",
```
message that is put on the oled if the task is completed before the first shock.

```Json
"gpio":3
```
the gpio pin used to signal that the task has been completed.


## different task types

there are 3 different task types.


1. once a day, at a specified time
2. once a day, with a specified time and a time window for when it can be completed
3. multiple times per day, with a specified start and stop time, plus a time for how often the task should be performed

type 1 we have already looked at. the other types just add extra fields to type 1.

### type 2

type 2 tasks only have one extra field.
```Json
"window":[0,30,0],
```

"window" specifies how long before the warning shock is sent that a task can be completed (or the punishment, if there is no warning). anything on the gpio pin assigned to this task will be ignored outside this time window.

the following example task has a time window of 30 minutes, meaning it can only be completed as early as 12:00
```Json
{"tasks":[{"name":"eat food","type":2,"can_punish":true,"can_warn":true,"can_reward":true,"punish_time":[12,45,0],"warn_time":[12,30,0],"window":[0,30,0],"punishment":{"intensity":40,"type":"Shock","duration":1000,"message":"Eat something. Its good for you."},"warning":{"intensity":50,"type":"Vibrate","duration":1000,"message":"its lunch time, you should have something to eat!"},"reward_message":"Good job","gpio":4}]}
```

### type 3

type 3 tasks three extra fields compared to type 1:
```Json
"start":[9,0,0],
"end":[23,0,0],
"interval":[1,0,0],
```
"start" is when the task starts each day.

"end" is when the task is done for the day.

"interval" is how often the task needs to be done (for this example its once per hour).

in addition to 3 extra fields, type 3 has 2 values that are used differently:

```Json
"punish_time":[0,55,0],
"warn_time":[0,45,0],
```

"punish_time" specifies how long after the start of each interval a punishment will be administered.

"warn_time" is the same as "punish_time" only it for the warning.

the following example task starts at 9:00, runs every hour, and ends at 23:00. the first warning will be at 9:45, and the last punishment will be at 22:55.

```Json
{"tasks":[{"name":"hydrate","type":3,"can_punish":true,"can_warn":true,"can_reward":true,"start":[9,0,0],"end":[23,0,0],"interval":[1,0,0],"punish_time":[0,55,0],"warn_time":[0,45,0],"punishment":{"intensity":40,"type":"Shock","duration":1000,"message":"Drink some water. Its good for you."},"warning":{"intensity":50,"type":"Vibrate","duration":1000,"message":"Its time to drink some water."},"reward_message":"Good job","gpio":5}]}
```



