// Stick remote with ESP32
// RO 4/2025
// Pins are read in with interrupt-routines
// Interrupt and debounce handling explained here: // https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/
// Keep this in mind: only one interrupt routine per pin is possible. so we have to attachthe falling routine in the rise routine and vice versa -> 
// https://forum.arduino.cc/t/pushbutton-short-long-press-with-interrupt/667457/7


// Inspired on the sketch form Stefly: 

// define press (short press) and hold (long press) functions for each button
// settings in XCSoar default.xci are
// F1 QuickMenu
// F2 Analysis
// F3 Checklist
// F4 FlarmTraffic
// F5 GotoLookup (Select Waypoint)
// F6 Setup Alternates
// F7 Setup Task
// F8 Setup Basic (wingload,bugs, QNH)
// F9
//    Status all
//    Calculator (tasks)
//
// settings in openvario.xci
// V for vario mode
// S for speed to fly mode
// M for vario menu
// Q to quit


// main modifications:
// 1) possibility to switch through all Menues with M Button (Upper Right Hand on Stick)
// 2) Mouse Acceleration for fast and slow mouse-positioning
// 3) automatic switch between mouse and cursor mode when enter the menues
// 4) documentation, comments
// 5) surely i put some errors in the code ;-)

// Description of the button functions:
// Jostick press: Enter or Left Click
// Fn : Switch between mouse or cursor mode
// M  : switch through all screen-button-menues. (1-1, 2-2, 3-3-3, 4-4-4) long press is Qick-Menue (M)
// X  : short press is ESC

// Hardware:  Stefly PCB V3.0 or Selfmade-PCB  and ESP32-S2 (LOLIN S2-mini)
// 4+1 HAT button for Joystick/mouse and three buttons Fn,M and X 
// button layout Stefly Remote as shown on http://www.openvario.org/doku.php?id=projects:remote_00:top 

// One of the buttons (lower left) is used as PTT-button for the radio and is not connected to the Arduino.
// The PCB V3.0 has the possibility to connect 2 more external switches.

// remote control for XCSoar, emulates a keyboard and mouse
// hardware is just pushbuttons connected between pins of uC and Gnd
// for each button press a keystroke or mouse action is sent





#include "USB.h"
#include "USBHIDMouse.h"
#include "USBHIDKeyboard.h"
USBHIDMouse Mouse;
USBHIDKeyboard Keyboard;


struct btn {
    const uint8_t PIN;
    unsigned long time_rise; //variables to keep track of the timing of recent interrupts and debouncing
    unsigned long time_fall; //variables to keep track of the timing of recent interrupts and debouncing
    uint8_t pressed; // 0: not pressed  1:pressed short  2:pressed long 3: pressed very long
};

// btn-object definitions
//  btn_name =  {pin number, time-rise, time_fall, pressed}
btn btn_left =  {18,0,0,0};
btn btn_right = {33,0,0,0};
btn btn_up =    {17,0,0,0};
btn btn_down =  {21,0,0,0};
btn btn_press = {16, 0,0,0};

btn btn_fn =    {34, 0,0,0};
btn btn_m =     {35, 0,0,0};
btn btn_x =     {36, 0,0,0};

uint8_t ausgabe = 0;
uint8_t menu_index = 0;
unsigned long press_time = 0;
unsigned long bounce_delay = 40; //Delay time in ms for debouncing time
unsigned long long_press_delay = 1000; //Delay time in ms for debouncing time
unsigned long very_long_press_delay = 4000; //Delay time in ms for debouncing time

boolean mouse_active = false; // 0: joystick does not control the mouse    1: Joystick controls the mouse
const int mouse_move_distance = 1;  

const byte mouse_delay_initial = 10;
byte mouse_delay ;



//Interrupt routine. will be called from ESP32 if button is pressed
//each time the ISR is executed, it compares the current time returned by the millis() function to the time the ISR was last called.
//If it is within the time given by bounce_delay (say 200ms), the ESP32 ignores the interrupt and immediately goes back to what 
//it was doing. If not, it executes the code within the if statement incrementing the counter and updating the last_button.time variable, 
//so the function has a new value to compare against when it’s triggered in the future.
void IRAM_ATTR isr_btn_left_rising() { //button was released
      btn_left.time_rise = millis(); // actual_time since start of uC (overflow after 50 days)
      press_time = btn_left.time_rise - btn_left.time_fall;
      if (press_time > bounce_delay & press_time < long_press_delay)
      { btn_left.pressed = 1;    }
      if (press_time >= long_press_delay & press_time < very_long_press_delay)
      { btn_left.pressed = 2;    }
      if (press_time >= very_long_press_delay)
      { btn_left.pressed = 3;    }
}
void IRAM_ATTR isr_btn_left_falling() {  //button was pressed
    btn_left.time_fall = millis(); // actual_time since start of uC (overflow after 50 days)
    attachInterrupt(btn_left.PIN, isr_btn_left_rising, RISING); // switch the interrupt to the rising interrupt routine  
}

void IRAM_ATTR isr_btn_right_rising() {
      btn_right.time_rise = millis(); // actual_time since start of uC (overflow after 50 days)
      press_time = btn_right.time_rise - btn_right.time_fall;
      if (press_time > bounce_delay & press_time < long_press_delay)
      { btn_right.pressed = 1;    }
      if (press_time >= long_press_delay & press_time < very_long_press_delay)
      { btn_right.pressed = 2;    }
      if (press_time >= very_long_press_delay)
      { btn_right.pressed = 3;    }
}
void IRAM_ATTR isr_btn_right_falling() {
    btn_right.time_fall = millis(); // actual_time since start of uC (overflow after 50 days)
    attachInterrupt(btn_right.PIN, isr_btn_right_rising, RISING); // switch the interrupt to the rising interrupt routine  
}

void IRAM_ATTR isr_btn_up_rising() {
      btn_up.time_rise = millis(); // actual_time since start of uC (overflow after 50 days)
      press_time = btn_up.time_rise - btn_up.time_fall;
      if (press_time > bounce_delay & press_time < long_press_delay)
      { btn_up.pressed = 1;    }
      if (press_time >= long_press_delay & press_time < very_long_press_delay)
      { btn_up.pressed = 2;    }
      if (press_time >= very_long_press_delay)
      { btn_up.pressed = 3;    }
}
void IRAM_ATTR isr_btn_up_falling() {
    btn_up.time_fall = millis(); // actual_time since start of uC (overflow after 50 days)
    attachInterrupt(btn_up.PIN, isr_btn_up_rising, RISING); // switch the interrupt to the rising interrupt routine  
}

void IRAM_ATTR isr_btn_down_rising() {
      btn_down.time_rise = millis(); // actual_time since start of uC (overflow after 50 days)
      press_time = btn_down.time_rise - btn_down.time_fall;
      if (press_time > bounce_delay & press_time < long_press_delay)
      { btn_down.pressed = 1;    }
      if (press_time >= long_press_delay & press_time < very_long_press_delay)
      { btn_down.pressed = 2;    }
      if (press_time >= very_long_press_delay)
      { btn_down.pressed = 3;    }
}
void IRAM_ATTR isr_btn_down_falling() {
    btn_down.time_fall = millis(); // actual_time since start of uC (overflow after 50 days)
    attachInterrupt(btn_down.PIN, isr_btn_down_rising, RISING); // switch the interrupt to the rising interrupt routine  
}

void IRAM_ATTR isr_btn_press_rising() {
      btn_press.time_rise = millis(); // actual_time since start of uC (overflow after 50 days)
      press_time = btn_press.time_rise - btn_press.time_fall;
      if (press_time > bounce_delay & press_time < long_press_delay)
      { btn_press.pressed = 1;    }
      if (press_time >= long_press_delay & press_time < very_long_press_delay)
      { btn_press.pressed = 2;    }
      if (press_time >= very_long_press_delay)
      { btn_press.pressed = 3;    }
}
void IRAM_ATTR isr_btn_press_falling() {
    btn_press.time_fall = millis(); // actual_time since start of uC (overflow after 50 days)
    attachInterrupt(btn_press.PIN, isr_btn_press_rising, RISING); // switch the interrupt to the rising interrupt routine  
}

void IRAM_ATTR isr_btn_fn_rising() {
      btn_fn.time_rise = millis(); // actual_time since start of uC (overflow after 50 days)
      press_time = btn_fn.time_rise - btn_fn.time_fall;
      if (press_time > bounce_delay & press_time < long_press_delay)
      { btn_fn.pressed = 1;    }
      if (press_time >= long_press_delay & press_time < very_long_press_delay)
      { btn_fn.pressed = 2;    }
      if (press_time >= very_long_press_delay)
      { btn_fn.pressed = 3;    }
}
void IRAM_ATTR isr_btn_fn_falling() {
    btn_fn.time_fall = millis(); // actual_time since start of uC (overflow after 50 days)
    attachInterrupt(btn_fn.PIN, isr_btn_fn_rising, RISING); // switch the interrupt to the rising interrupt routine  
}

void IRAM_ATTR isr_btn_m_rising() {
      btn_m.time_rise = millis(); // actual_time since start of uC (overflow after 50 days)
      press_time = btn_m.time_rise - btn_m.time_fall;
      if (press_time > bounce_delay & press_time < long_press_delay)
      { btn_m.pressed = 1;    }
      if (press_time >= long_press_delay & press_time < very_long_press_delay)
      { btn_m.pressed = 2;    }
      if (press_time >= very_long_press_delay)
      { btn_m.pressed = 3;    }
}
void IRAM_ATTR isr_btn_m_falling() {
    btn_m.time_fall = millis(); // actual_time since start of uC (overflow after 50 days)
    attachInterrupt(btn_m.PIN, isr_btn_m_rising, RISING); // switch the interrupt to the rising interrupt routine  
}

void IRAM_ATTR isr_btn_x_rising() {
      btn_x.time_rise = millis(); // actual_time since start of uC (overflow after 50 days)
      press_time = btn_x.time_rise - btn_x.time_fall;
      if (press_time > bounce_delay & press_time < long_press_delay)
      { btn_x.pressed = 1;    }
      if (press_time >= long_press_delay & press_time < very_long_press_delay)
      { btn_x.pressed = 2;    }
      if (press_time >= very_long_press_delay)
      { btn_x.pressed = 3;    }
}
void IRAM_ATTR isr_btn_x_falling() {
    btn_x.time_fall = millis(); // actual_time since start of uC (overflow after 50 days)
    attachInterrupt(btn_x.PIN, isr_btn_x_rising, RISING); // switch the interrupt to the rising interrupt routine  
}


void setup() {
    Serial.begin(115200);
    Mouse.begin();
    Keyboard.begin();
    USB.begin();

    pinMode(btn_left.PIN, INPUT_PULLUP);
    pinMode(btn_right.PIN, INPUT_PULLUP);
    pinMode(btn_up.PIN, INPUT_PULLUP);
    pinMode(btn_down.PIN, INPUT_PULLUP);
    pinMode(btn_press.PIN, INPUT_PULLUP);
    pinMode(btn_fn.PIN, INPUT_PULLUP);
    pinMode(btn_m.PIN, INPUT_PULLUP);
    pinMode(btn_x.PIN, INPUT_PULLUP);

    attachInterrupt(btn_left.PIN, isr_btn_left_falling, FALLING);
    attachInterrupt(btn_right.PIN, isr_btn_right_falling, FALLING);
    attachInterrupt(btn_up.PIN, isr_btn_up_falling, FALLING);
    attachInterrupt(btn_down.PIN, isr_btn_down_falling, FALLING);
    attachInterrupt(btn_press.PIN, isr_btn_press_falling, FALLING);
    attachInterrupt(btn_fn.PIN, isr_btn_fn_falling, FALLING);
    attachInterrupt(btn_m.PIN, isr_btn_m_falling, FALLING);
    attachInterrupt(btn_x.PIN, isr_btn_x_falling, FALLING);
}

void loop() {
    
    if (mouse_active == true) {
      mouse_delay = mouse_delay_initial;
      while (digitalRead(btn_up.PIN) == 0) {
        Mouse.move(0, -mouse_move_distance);    
        delay(mouse_delay);
        mouse_delay = mouse_delay - 1; //increase mouse speed  
        if (mouse_delay < 1) mouse_delay = 1;  //max speed
      }

      mouse_delay = mouse_delay_initial;
      while (digitalRead(btn_down.PIN) == 0) {
        Mouse.move(0, mouse_move_distance);    
        delay(mouse_delay);
        mouse_delay = mouse_delay - 1; //increase mouse speed  
        if (mouse_delay < 1) mouse_delay = 1;  //max speed
      }
      mouse_delay = mouse_delay_initial;
      while (digitalRead(btn_right.PIN) == 0) {
        Mouse.move(mouse_move_distance, 0);    
        delay(mouse_delay);
        mouse_delay = mouse_delay - 1; //increase mouse speed  
        if (mouse_delay < 1) mouse_delay = 1;  //max speed
      }
      mouse_delay = mouse_delay_initial;
      while (digitalRead(btn_left.PIN) == 0) {
        Mouse.move(-mouse_move_distance, 0);    
        delay(mouse_delay);
        mouse_delay = mouse_delay - 1; //increase mouse speed  
        if (mouse_delay < 1) mouse_delay = 1;  //max speed
      }

      if (btn_press.pressed > 0) {
          if (btn_press.pressed == 1) { //Short press
            Keyboard.releaseAll();
            Mouse.click(MOUSE_LEFT);
          }
        btn_press.pressed = 0;
        attachInterrupt(btn_press.PIN, isr_btn_press_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
      }

    }
    else if (mouse_active == false ) {
      //Serial.printf("mouse inaktiv \n");
      if (btn_left.pressed > 0) {
      // 1: btn pressed short   2: button pressed >1sec  3: button pressed >4sec
        Keyboard.releaseAll();
        Keyboard.write(KEY_LEFT_ARROW);
        btn_left.pressed = 0;
        attachInterrupt(btn_left.PIN, isr_btn_left_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
      }

      if (btn_right.pressed > 0) {
        Keyboard.releaseAll();
        Keyboard.write(KEY_RIGHT_ARROW);
        btn_right.pressed = 0;
        attachInterrupt(btn_right.PIN, isr_btn_right_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
      }

      if (btn_down.pressed > 0) {
        Keyboard.releaseAll();
        Keyboard.write(KEY_DOWN_ARROW);
        btn_down.pressed = 0;
        attachInterrupt(btn_down.PIN, isr_btn_down_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
      }

      if (btn_up.pressed > 0) {
        Keyboard.releaseAll();
        Keyboard.write(KEY_UP_ARROW);
        btn_up.pressed = 0;
        attachInterrupt(btn_up.PIN, isr_btn_up_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
      }

      if (btn_press.pressed > 0) {
        if (btn_press.pressed == 1) { //Short press
          Keyboard.releaseAll();
          Keyboard.write(KEY_RETURN);
        }
        btn_press.pressed = 0;
        attachInterrupt(btn_press.PIN, isr_btn_press_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
      }
    }  //end of else


    if (btn_fn.pressed > 0) {
      //Serial.printf("fn %u \n",btn_fn.pressed);
      btn_fn.pressed = 0;
      mouse_active = !mouse_active;
      attachInterrupt(btn_fn.PIN, isr_btn_fn_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
    }

    if (btn_m.pressed == 1)  {
      //Serial.printf("m %u \n",btn_m.pressed);
      menu_index = menu_index +1;
      if (menu_index > 100) menu_index =0;
      switch (menu_index) {    
        case 1:
          Keyboard.releaseAll();
          Keyboard.press('1');
          mouse_active = true;
          break;
        case 2:
          Keyboard.releaseAll();
          Keyboard.press('1');
          mouse_active = true;
          break;      
        case 3:
          Keyboard.releaseAll();
          Keyboard.press('2');
          mouse_active = true;
          break;
        case 4:
          Keyboard.releaseAll();
          Keyboard.press('2');
          mouse_active = true;
          break;
        case 5:
          Keyboard.releaseAll();
          Keyboard.press('3');
          mouse_active = true;
          break;
        case 6:
          Keyboard.releaseAll();
          Keyboard.press('3');
          mouse_active = true;
          break;
        case 7:
          Keyboard.releaseAll();
          Keyboard.press('3');
          mouse_active = true;
          break;
        case 8:
          Keyboard.releaseAll();
          Keyboard.press('4');
          mouse_active = true;
          break;
        case 9:
          Keyboard.releaseAll();
          Keyboard.press('4');
          mouse_active = true;
          break;      
        case 10:
          Keyboard.releaseAll();
          Keyboard.press('4');
          mouse_active = true;
          break;
        case 11:
          Keyboard.releaseAll();
          Keyboard.press('4');
          mouse_active = true;
          menu_index = 0;  // because its the last entry
          break;
        
        default:
        // if nothing else matches, do the default
        // default is optional
        menu_index = 0;  // because its the last entry
        
        break;
      } //End of Switch

      btn_m.pressed = 0;
      attachInterrupt(btn_m.PIN, isr_btn_m_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
    }

    if (btn_m.pressed == 2)  { // Long pressed M-Button
      Keyboard.releaseAll();
      Keyboard.press(KEY_F1); // Quick Menue
      mouse_active = false;
      btn_m.pressed = 0;
      attachInterrupt(btn_m.PIN, isr_btn_m_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
    }

    if (btn_x.pressed == 1) { // Short pressed X-Button
      //Serial.printf("x %u \n",btn_x.pressed);
      Keyboard.releaseAll();
      Keyboard.press(KEY_ESC);
      btn_x.pressed = 0;
      attachInterrupt(btn_x.PIN, isr_btn_x_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
    }
    Keyboard.releaseAll();

}