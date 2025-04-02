# Remote-Stick-with-ESP32
Remote Stick working as HID (Keybaord and Mouse) for Open Vario / Open Soar.
Can also used for DIY Keyboards and Mouse Emulation with Keys.


<b> Work in Progress!!! </b> 

uC ESP32-S2
uC Board: Lolin S2 mini

The buttons are read in with Interrupt-Routines. The ESP32-S2 has one core, with 32 interrupts. </br>
The buttons cann be pressed short, long (1sec) or very long (4sec), </br>
The pressing times are customizable.


| Button        | Pin | Mode: Joystick   | Mode: Mouse      | Remark  
| ------------- | --- | -------------    | -------------    | ------------- 
| HAT-Key Up    | 17  |KEY_UP_ARROW      | move mouse up    | -
| HAT-Key Down  | 21  |KEY_DOWN_ARROW    | move mouse down  | -
| HAT-Key Left  | 18  |KEY_LEFT_ARROW    | move mouse left  | -
| HAT-Key Right | 33  |KEY_RIGHT_ARROW   | move mouse right | -
| HAT-Key Press | 16  |KEY_RETURN        | mouse left click | -
| Fn            | 34  |Switch Mode       | -                | -
| M             | 35  |1 1 3 3 3 4 4 4 4 | -                | cycle through Menue and switch Mode to Mouse
| X             | 36  |KEY_ESC           | -                | -

</b>

  * <b>1 (Fn)</b> : switches between mouse and keyboard emulation, **a long pressure ( > 1 Sec) sends a 'E' - could be managed as PilotEvent**
     in the XCSoar/OpenSoar - or to another user defined event in the xci-file
  * <b>2 (M)</b> : call the QuickMenu ('F1'), long pressure (> 1 sec) Audio/Vario-Menu ('M')
  * <b>3 (X)</b> : is ESC, **a long pressure calls the Quit program command ('Q'), and a very long pressure ( > 4 sec) sends a 'X' - can be used as exit command or for a shutdown event**
  * <b>4 (PTT)</b> : is an additional button connected to the PTT (not managed by the RemoteControl itself)
  * <b>5</b> (up), <b>6</b> (down), <b>7</b> (left), <b>8</b> (right) :  are arrow keys **with an accelerated movement after 1 second of continuous pressure**
  * <b>0</b> : is Enter, **a long pressure (> 1 sec) should reset the RemoteStick!**
  * <b>9 (STF)</b> (out of this layout): is a button (backside of stick) for toggling between Vario and STF (Speed-to-Fly) mode

<img src="Hardware.JPG" width="600px" />

<img src="Hardware.JPG" width="600px" />

<img src="Schematic.JPG" width="600px" />

