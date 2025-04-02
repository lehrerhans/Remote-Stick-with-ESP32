// Stick remote with ESP32
// RO 4/2025
// Pins are read in with interrupt-routines
// Interrupt and debounce handling explained here: // https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/
// Keep this in mind: only one interrupt routine per pin is possible. so we have to attachthe falling routine in the rise routine and vice versa -> 
// https://forum.arduino.cc/t/pushbutton-short-long-press-with-interrupt/667457/7


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
unsigned long press_time = 0;
unsigned long bounce_delay = 20; //Delay time in ms for debouncing time
unsigned long long_press_delay = 1000; //Delay time in ms for debouncing time
unsigned long very_long_press_delay = 4000; //Delay time in ms for debouncing time

//Interrupt routine. will be called from ESP32 if button is pressed
//each time the ISR is executed, it compares the current time returned by the millis() function to the time the ISR was last called.
//If it is within the time given by bounce_delay (say 200ms), the ESP32 ignores the interrupt and immediately goes back to what 
//it was doing. If not, it executes the code within the if statement incrementing the counter and updating the last_button.time variable, 
//so the function has a new value to compare against when it’s triggered in the future.
void IRAM_ATTR isr_btn_left_rising() {
      btn_left.time_rise = millis(); // actual_time since start of uC (overflow after 50 days)
      press_time = btn_left.time_rise - btn_left.time_fall;
      if (press_time > bounce_delay & press_time < long_press_delay)
      { btn_left.pressed = 1;    }
      if (press_time >= long_press_delay & press_time < very_long_press_delay)
      { btn_left.pressed = 2;    }
      if (press_time >= very_long_press_delay)
      { btn_left.pressed = 3;    }
}
void IRAM_ATTR isr_btn_left_falling() {
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
    if (btn_left.pressed > 0) {
      Serial.printf("left %u \n",btn_left.pressed);
      btn_left.pressed = 0;
      attachInterrupt(btn_left.PIN, isr_btn_left_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
    }
    if (btn_right.pressed > 0) {
      Serial.printf("right %u \n",btn_right.pressed);
      btn_right.pressed = 0;
      attachInterrupt(btn_right.PIN, isr_btn_right_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
    }
    if (btn_down.pressed > 0) {
      Serial.printf("down %u \n",btn_down.pressed);
      btn_down.pressed = 0;
      attachInterrupt(btn_down.PIN, isr_btn_down_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
    }
    if (btn_up.pressed > 0) {
      Serial.printf("up %u \n",btn_up.pressed);
      btn_up.pressed = 0;
      attachInterrupt(btn_up.PIN, isr_btn_up_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
    }

    if (btn_press.pressed > 0) {
      Serial.printf("press %u \n",btn_press.pressed);
      btn_press.pressed = 0;
      attachInterrupt(btn_press.PIN, isr_btn_press_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
    }

    if (btn_fn.pressed > 0) {
      Serial.printf("fn %u \n",btn_fn.pressed);
      btn_fn.pressed = 0;
      attachInterrupt(btn_fn.PIN, isr_btn_fn_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
    }

    if (btn_m.pressed > 0) {
      Serial.printf("m %u \n",btn_m.pressed);
      btn_m.pressed = 0;
      attachInterrupt(btn_m.PIN, isr_btn_m_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
    }
    if (btn_x.pressed > 0) {
      Serial.printf("x %u \n",btn_x.pressed);
      btn_x.pressed = 0;
      attachInterrupt(btn_x.PIN, isr_btn_x_falling, FALLING); // switch the interrupt to the FALLING interrupt routine
    }
}