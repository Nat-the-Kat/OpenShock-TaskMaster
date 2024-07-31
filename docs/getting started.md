# **getting started**

## **uploading the firmware**

1. after opening this project in VS Code, in the PlatformIO menu, go to "project tasks>pico>general" and click "Upload". (please note, if you are flashing a brand new pico, make sure to hold down the bootsel button when plugging it in to put it in firmware upload mode)

2. once that is done, in the same menu click "Upload Filesystem Image".

3. you should be good to begin configuring.

## **initial connection and configuring**

1. once the onboard led on the pico turns on (if it has one), a wifi network called "TaskMaster" should appear. connect to that network.

2. after connecting, use a web browser to navigate to http://192.168.42.1

note 1: it is suggested that you start with the wifi list. after entering the details of the wifi network(s) that it will connect to, you can reboot it by simply unplugging it, or use a serial monitor and type in the command "reboot". (make sure you save the network list to flash, if you have a serial monitor open it should say "network list updated")


note 2: after rebooting if it finds a network you will have to navigate to its local ip address to continue configuring. (you can find this by using the command "ip_address" in the serial terminal)

the various configuration pages should be relatively easy to understand what everything does? i think?
 