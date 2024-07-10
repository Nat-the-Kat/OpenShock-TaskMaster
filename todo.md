## stuff left to do before merge v0.01

* ~~design webpages for configuring.~~
* ~~send configuration data from website back to pico.~~
* save config data from portal.
* ~~add simple error checking of config portal data (null values, doubled up io pins, tasks without shocks, tasks with same name).~~
* ~~add the ability for the pico to create its own wifi network when none are found.~~
* ~~find and fix whatever is causing the screen to not clear properly.~~
* find and fix the reset time freezing issue.
* update documentation (ughhhhh).

## future things

* finish date part of the tod library.
* add support for multiple shockers, and assigning different tasks to different shockers
* add a webpage for requesting an OpenShock api key.
* ability to set certain tasks to only run on certain days of the week/certain days of the month
* support for daylight saving time (if applicable in your part of the world)
* multiple tasks using same io pin.
* custom pcb and 3d printed enclosure. no more protoboard.

## far future things

* google calendar support (need to look into more).
* either port to a different platform (like esp32), or make the jump to fully using the pico sdk (limits portability). either way ditch arduino. 