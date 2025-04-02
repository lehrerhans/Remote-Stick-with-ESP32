# Remote-Stick-with-ESP32
Remote Stick working as HID (Keybaord and mouse) for Open Vario / Open Soar.
Can also used for DIY Keyboards and Mouse Emulation with Keys.


<b> Work in Progress!!! </b> 

uC ESP32-S2
uC Board: Lolin S2 mini

The buttons are read in with Interrupt-Routines. The ESP32-S2 has one core, with 32 interrupts
The buttons cann be pressed short, long (1sec) or very long (4sec)
The pressing times are customizable.

Key            mode Jostick    mode mouse
HAT-Key Up     KEY_UP_ARROW    move mouse up
HAT-Key Down   KEY_DOWN_ARROW  move mouse down
HAT-Key Left   KEY_LEFT_ARROW  move mouse left
HAT-Key Right  KEY_RIGHT_ARROW move mouse right
HAT-Key press  KEY_Enter       mouse left click 

Fn            
M
x  

<img src="Hardware.JPG" width="600px" />

<img src="Schematic.JPG" width="600px" />

