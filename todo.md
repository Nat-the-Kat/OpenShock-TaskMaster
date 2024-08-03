## stuff left to do before merge v0.2.0

* ability to set certain tasks to only run on certain days of the week/certain days of the month
* completely purge the tod library. it was a dumb idea.
* custom pcb and 3d printed enclosure. no more protoboard.

## problems i can't figure out

* find and fix whatever is causing the screen to not clear properly. idk why i cant figure this problem out. it seems to just be happening randomly...

## future things

* add support for multiple shockers, and assigning different tasks to different shockers
* add a webpage for requesting an OpenShock api key.
* compress webpage files to save space in flash.
* multiple tasks using same io pin.
* 

## far future things

* google calendar support (need to look into more).
* either port to a different platform (like esp32), or make the jump to fully using the pico sdk (limits portability). either way ditch arduino. 
* a small interpreter (probably lua, although if i want to be really weird i could do BASIC...) to allow for programming task triggers (i.e. only count a task as complete if a sensor reads a certain value) 
* create a file format for more compactly storing config and task data than json.
