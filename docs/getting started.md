# **Getting started**

1. after uploading the firmware, plug the device into a free usb port and open up a serial monitor on whatever port the pico is using. (i used [this one](https://serial.huhn.me))

2. after opening the serial monitor you should be greeted with several messages telling you that its trying to connect, before finally spitting out "ready to receive commands..."
it is now ready to be configured.

3. type in "add_networks" and hit enter.

4. it should say "waiting for json string..."
now you can paste in the contents of a wifi.json file (see [this](wifi.md) for more details)

note 1: for this document (and all json documents used to configure this project) need to be minified, ie no newline characters or it will complain that its an invalid document. spaces are ok.

5. type in the command "write_networks". this writes the configuration to flash.

repeat steps 3,4 & 5 with the commands "edit_config" ("write_config" to save) and "add_tasks" ("write_tasks" to save). refer to [this for info on making a config file](config.md) and [this for info on making a tasks file](tasks.md)

Note 2: you can use the view commands ("view_config","view_tasks", and "view_networks") to check your work. you can remove a bad network config using "delete_network 'ssid'" (ssid doesn't need to be in single quotes). badly configured tasks can be removed using "remove_task 'task'" (task is the task_name, it doesn't need to be in single quotes). the config can only be edited with "edit_config"
