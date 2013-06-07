/* Robot Control System
 * Copyright (C) 2013 Tuna Oezer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Scriptduino version.
#define VERSION "ScriptDuino 0.2 [2013.06.06.00]"

// Borad identifiers. Must set for each board.
#define BOARD "Arduino Mega 2560"

// Number of serial ports. Depends on the board and configuration.
#define NUM_SERIAL_PORTS 1

// Baud rates for the serial ports.
#define BAUD_RATE_PORT  115200  // default port
#define BAUD_RATE_PORT1 115200  // extended port 1
#define BAUD_RATE_PORT2 115200  // extended port 2
#define BAUD_RATE_PORT3   9600  // extended port 3

// Maximum size of serial input.
#define MAX_SERIAL_INPUT_SIZE 512

// Logging level
#define LOG_NONE 0
#define LOG_ERROR 1
#define LOG_WARNING 2
#define LOG_INFO 3
int log_level;

// Defaults
#define INIT_ON_STARTUP true  // Run init() on startup
#define ENABLE_RADIO_ON_STARTUP true  // Enable radio on startup

// Gyro
#define USE_GYRO true
#define GYRO_MIN_THRESHOLD -15.0f
#define GYRO_MAX_THRESHOLD 15.0f
#define GYRO_MAX_DIRECTION_ADJUSTMENT 0.3f
#define GYRO_DIRECTION_ADJUSTMENT_DELTA 0.0002f

#if USE_GYRO
#include <Wire.h>
#include <L3G.h>
L3G gyro;
#endif

// True if radio control is on.
boolean radio_control = ENABLE_RADIO_ON_STARTUP;

// Abstract base class for all functions.
// All functions must derive from this class and implement exec(String);
class Function {
  public:
    // Creates a function with the specified name.
    explicit Function(const String& name)
      : name_(name) {
    }

    virtual ~Function() {
    }

    // Returns the name of the function.
    const String& name() const {
      return name_;
    }

    // Derived classes must implement this method.
    // args is a comma separated list of function arguments. The pop* methods should be used by
    // derived classes to obtain individual typed arguments.
    // Returns true if execution was successful.
    virtual boolean exec(String* args) = 0;

    // Pops the next argument from the argument list and returns it as a string in result.
    // Returns the empty string if there are no more arguments left.
    // The popped argument is removed from args.
    static void popToken(String* args, String* result) {
      int index = args->indexOf(',');
      if (index < 0) {
        *result = *args;
        *args = "";
      } else {
        *result = args->substring(0, index);
        *args = args->substring(index + 1);
      }
    }

    // Pops the next argument from the argument list and returns it as an integer.
    // Returns 0 on error.
    static void popInteger(String* args, int* result) {
      String str;
      popToken(args, &str);
      *result = str.toInt();
    }

    // Pops the next argument from the argument list and returns it as a float.
    // Returns 0.0 on error.
    static void popFloat(String* args, float* result) {
      String str;
      popToken(args, &str);
      char buf[9];
      str.toCharArray(buf, 8);
      buf[8] = 0;
      *result = atof(buf);
    }

  private:
    String name_;
};

// ------------------------------------------------------------------------------
// Standard functions
// ------------------------------------------------------------------------------

// ping([flags]);
// ping() returns the Scriptduino version.
// ping(board) returns the board identifier.
class Ping : public Function {
  public:
    explicit Ping() : Function("ping") {}

    virtual boolean exec(String* args) {
      String arg;
      popToken(args, &arg);
      if (arg == "board") {
        Serial.println(BOARD);
        return true;
      }
      Serial.println(VERSION);
      return true;
    }
};

// logLevel(int value);
// Sets the logging level.
// logLevel(0) turns off logging.
// logLevel(x) where x > 0 set logging to level x.
class LogLevel : public Function {
  public:
    explicit LogLevel() : Function("logLevel") {}
    
    virtual boolean exec(String* args) {
      popInteger(args, &log_level);
      return true;
    }
};

// reset();
// reset() sets all output pins to low voltage and turns off
// pull up resistors on input pins.
class Reset : public Function {
  public:
    explicit Reset() : Function("reset") {}

    virtual boolean exec(String* args) {
      for (int pin = 0; pin < NUM_DIGITAL_PINS; ++pin) {
        digitalWrite(pin, LOW);
      }
      return true;
    }
};

// pinMode(int pin, int mode);
// Sets the mode for the specified pin.
// pinMode(pin_x, input) set pin_x to input mode.
// pinMode(pin_x, output) set pin_x to output mode.
class PinMode : public Function {
  public:
    explicit PinMode() : Function("pinMode") {}
    
    virtual boolean exec(String* args) {
      int pin = 0;
      String value = 0;
      popInteger(args, &pin);
      popToken(args, &value);
      if (pin < 0 || pin >= NUM_DIGITAL_PINS) return false;
      if (value == "output") {
        pinMode(pin, OUTPUT);
      } else if (value == "input") {
        pinMode(pin, INPUT);
      } else {
        return false;
      }
      return true;
    }
};

// dwrite(int pin, int value);
// Set the value of an output pin to high or low voltage.
// Turns on or off the pull up resistor of an input pin.
// The pin number must be a valid pin.
// dwrite(pin_x,1) set pin_x to high or turn on resistor
// dwrite(pin_x,1) set pin_x to low or turn off resistor
class DigitalWrite : public Function {
  public:
    explicit DigitalWrite() : Function("dwrite") {}

    virtual boolean exec(String* args) {
      int pin = 0;
      int value = 0;
      popInteger(args, &pin);
      popInteger(args, &value);
      if (pin < 0 || pin >= NUM_DIGITAL_PINS) return false;
      switch (value) {
        case 0: digitalWrite(pin, LOW); break;
        case 1: digitalWrite(pin, HIGH); break;
        default: return false;
      }
      return true;
    }
};

// pwm(int pin, int duty_cycle);
// Sets a PWM signal at the specified pin. The duty_cycle must be between
// 0 (off) and 255 (=100%).
class Pwm : public Function {
  public:
    explicit Pwm() : Function("pwm") {}

    virtual boolean exec(String* args) {
      int pin = 0;
      int value = 0;
      popInteger(args, &pin);
      popInteger(args, &value);
      if (pin < 0 || pin >= NUM_DIGITAL_PINS) return false;
      if (value < 0 || value > 255) return false;
      analogWrite(pin, value);
      return true;
    }
};


// ------------------------------------------------------------------------------
// Specialized functions for robot control
// ------------------------------------------------------------------------------

#define ENABLE_PIN 28
#define MOTOR_RIGHT_FORWARD_PIN 22
#define MOTOR_RIGHT_BACKWARD_PIN 24
#define MOTOR_RIGHT_SPEED_PIN 11
#define MOTOR_LEFT_FORWARD_PIN 32
#define MOTOR_LEFT_BACKWARD_PIN 30
#define MOTOR_LEFT_SPEED_PIN 12

// for radio control
#define RADIO_JOYSTICK_X_PIN 3  // movement of joystick in x direction
#define RADIO_JOYSTICK_Y_PIN 2  // movement of joystick in y direction
#define RADIO_MIN_HIGH_DURATION_MICROS 100  // minimum high signal duration (radio is off below this value)
#define RADIO_NEGATIVE_START_MICROS 1500    // signals below this value are interpreted as negative
#define RADIO_NEGATIVE_RANGE_MICROS 350.0f  // difference between highest and lowest value in negative range
#define RADIO_POSITIVE_START_MICROS 1550    // signals above this value are interpreted as positive
#define RADIO_POSITIVE_RANGE_MICROS 350.0f  // difference between highest and lowest value in positive range
#define RADIO_X_ZERO_RANGE 0.2f  // x values with smaller absolute values are interpreted as zero
#define RADIO_XY_FACTOR 1.2f  // Effect of y on x scaling

// Represents a motor.
class Motor {
  public:
    Motor(int forward_pin, int backward_pin, int speed_pin)
     : forward_pin_(forward_pin),
       backward_pin_(backward_pin),
       speed_pin_(speed_pin),
       speed_(0) {
    }
    
    // Turns off the motor. Does not actively break.
    void stop() {
      digitalWrite(speed_pin_, 0);
      digitalWrite(forward_pin_, 0);
      digitalWrite(backward_pin_, 0);
      speed_ = 0;
    }
    
    // Sets the motor to move at the specified speed.
    // The speed value must be between -255 and +255.
    // A positive value moves the motor in the forward direction.
    // A negative value moves it in the backward direction.
    // This method handles switching direction in flight.
    void move(int speed) {
      if (speed == 0) {
        stop();
        return;
      }
      if (speed > 0) {
        if (speed_ < 0) {
          stop();
        }
        if (speed > 255) speed = 255;
        digitalWrite(forward_pin_, HIGH);
        analogWrite(speed_pin_, speed);
      } else {
        if (speed_ > 0) {
          stop();
        }
        if (speed < -255) speed = -255;
        digitalWrite(backward_pin_, HIGH);
        analogWrite(speed_pin_, -speed);
      }
      speed_ = speed;
    }
  
  private:
    int forward_pin_;
    int backward_pin_;
    int speed_pin_;
    int speed_;  // current speed
};

Motor left_motor(MOTOR_LEFT_FORWARD_PIN, MOTOR_LEFT_BACKWARD_PIN, MOTOR_LEFT_SPEED_PIN);
Motor right_motor(MOTOR_RIGHT_FORWARD_PIN, MOTOR_RIGHT_BACKWARD_PIN, MOTOR_RIGHT_SPEED_PIN);

// RadioInterruptHandler can be configuered to respond to radio signal interrupts at the specified pin.
class RadioInterruptHandler {
  public:
    // Interrupts must be supported on the specified pins.
    // See attach() method for supported pins.
    explicit RadioInterruptHandler(int pin)
      : pin_(pin),
        state_(0),
        high_micros_(0),
        high_duration_micros_(0) {
    }

    // Returns the interrupt number for the specified pin.
    // Returns -1 the pin does not support interrupts.
    // This is configured for the Arduino MEGA 2560. Different boards may have different mappings.
    static int InterruptNumber(int pin) {
      switch (pin) {
        case 2: return 0;
        case 3: return 1;
        default: return -1;
      }
    }

    // Tries to attach the interrupt handler and returns true on success.
    boolean attach() {
      int interrupt_number = InterruptNumber(pin_);
      switch (interrupt_number) {
        case 0: attachInterrupt(interrupt_number, interruptHandler0, CHANGE); break;
        case 1: attachInterrupt(interrupt_number, interruptHandler1, CHANGE); break;
        default: return false;
      }
      return true;
    }

    // Tries to detach the interrupt handler and returns true on success.
    boolean detach() {
      int interrupt_number = InterruptNumber(pin_);
      if (interrupt_number < 0) return false;
      detachInterrupt(interrupt_number);
      return true;
    }

    // Returns the signal value in the range -1.0 .. +1.0.
    // This corresponds to the displacement of the joystick in the respective direction.
    float getJoystickPosition() {
      if (high_duration_micros_ < RADIO_MIN_HIGH_DURATION_MICROS) return 0.0f;
      float position;
      if (high_duration_micros_ > RADIO_NEGATIVE_START_MICROS && high_duration_micros_ < RADIO_POSITIVE_START_MICROS) {
        position = 0.0f;
      } else if (high_duration_micros_ >= RADIO_POSITIVE_START_MICROS) {
        position = (high_duration_micros_ - RADIO_POSITIVE_START_MICROS) / RADIO_POSITIVE_RANGE_MICROS;
        if (position > 1.0f) position = 1.0f;
      } else {
        // high_duration <= RADIO_NEGATIVE_START
        position = (RADIO_NEGATIVE_START_MICROS - high_duration_micros_) / -RADIO_NEGATIVE_RANGE_MICROS;
        if (position < -1.0f) position = -1.0f;
      }
      return position;
    }

    // Handles speed interrupts.
    void handle() {
      switch (digitalRead(pin_)) {
        case HIGH:
          if (state_ == 0) {
            high_micros_ = micros();
            state_ = 1;
          }
          break;
        case LOW:
          if (state_ == 1) {
            high_duration_micros_ = micros() - high_micros_;
            if (high_duration_micros_ < 0) {
              // clock overflow
              high_duration_micros_ = micros() + (4294967295 - high_micros_);
            }
            state_ = 0;
          }
          break;
      }
    }

  private:
    int pin_;
    int state_;
    unsigned long high_micros_;  // time when the signal goes HIGH in micros
    unsigned long high_duration_micros_;  // duration of high signal
};

RadioInterruptHandler radio_joystick_x_handler(RADIO_JOYSTICK_X_PIN);
RadioInterruptHandler radio_joystick_y_handler(RADIO_JOYSTICK_Y_PIN);

// Interrupt handler for interrupt 0.
void interruptHandler0() {
  radio_joystick_y_handler.handle();
}

// Interrupt handler for interrupt 1.
void interruptHandler1() {
  radio_joystick_x_handler.handle();
}
// stop()
// Stops the robot.
class Stop : public Function {
  public:
    explicit Stop() : Function("stop") {}

    virtual boolean exec(String* args) {
      stop();
      return true;
    }

    static void stop() {
      left_motor.stop();
      right_motor.stop();
    }
};
// init();
// Initializes the board for robot control.
class Init : public Function {
  public:
    explicit Init() : Function("init") {}

    virtual boolean exec(String* args) {
      init();
      return true;
    }
    
    static void init() {
      // Adjust the PWM frequency on timer 1 (pins 11 and 12) to achieve quiter motor operation.
      TCCR1B = TCCR1B & 0b11111000 | 0x01;  // 31250 Hz
      
      Stop::stop();
      pinMode(MOTOR_LEFT_SPEED_PIN, OUTPUT);
      pinMode(MOTOR_RIGHT_SPEED_PIN, OUTPUT);
      pinMode(MOTOR_LEFT_FORWARD_PIN, OUTPUT);
      pinMode(MOTOR_LEFT_BACKWARD_PIN, OUTPUT);
      pinMode(MOTOR_RIGHT_FORWARD_PIN, OUTPUT);
      pinMode(MOTOR_RIGHT_BACKWARD_PIN, OUTPUT);
      
      pinMode(RADIO_JOYSTICK_X_PIN, INPUT);
      pinMode(RADIO_JOYSTICK_Y_PIN, INPUT);
      radio_joystick_x_handler.attach();
      radio_joystick_y_handler.attach();

      // Set enable bit for motors.
      digitalWrite(ENABLE_PIN, HIGH);
    }
};

// move(float speed, float direction)
// Move the robot at the specified speed and direction.
// speed must be a value between -1.0 and +1.0. 0 stops the robot.
// +1.0 is full speed forwards. -1.0 is full speed backwards.
// direction must be a value between -1.0 and +1.0.
// 0 is straight ahead. -1.0 is full turn towards left.
// +1.0 is full turn towards right.
class Move : public Function {
  public:
    explicit Move() : Function("move") {}

    virtual boolean exec(String* args) {
      float speed;
      float direction;
      popFloat(args, &speed);
      popFloat(args, &direction);
      move(speed, direction);
      return true;
    }
    
    static void move(float speed, float direction) {
      float right_speed = speed;
      float left_speed = speed;
      
      #if USE_GYRO
      if (abs(speed) > 0.02f && direction > -0.02f && direction < 0.02f) {
        if (last_speed * speed < 0.0f) {
          // reversal
          direction_adjustment = 0.0f;
        }
        gyro.read();
        if (gyro.g.z > GYRO_MAX_THRESHOLD) {
          if (direction_adjustment < GYRO_MAX_DIRECTION_ADJUSTMENT) {
            direction_adjustment += GYRO_DIRECTION_ADJUSTMENT_DELTA;
          }
        } else if (gyro.g.z < GYRO_MIN_THRESHOLD) {
          if (direction_adjustment > -GYRO_MAX_DIRECTION_ADJUSTMENT) {
            direction_adjustment -= GYRO_DIRECTION_ADJUSTMENT_DELTA;
          }
        }
        if (speed > 0.0f) {
          direction = direction_adjustment;
        } else {
          direction = -direction_adjustment;
        }
      } else {
        direction_adjustment = 0.0f;
      }
      #endif
      
      if (direction > 0.02f) {
        right_speed = speed * (1.0f - 2.0f * direction);
      } else if (direction < -0.02f) {
        left_speed = speed * (1.0f + 2.0f * direction);
      }
      left_motor.move((int)(255 * left_speed));
      right_motor.move((int)(255 * right_speed));
      last_speed = speed;
    }
    
  private:
    static float last_speed;
    static float direction_adjustment;
};
float Move::last_speed(0.0f);
float Move::direction_adjustment(0.0f);

// radio(on|off);
// Turns radio control on or off.
class Radio : public Function {
  public:
    explicit Radio() : Function("radio") {}
    
    virtual boolean exec(String* args) {
      String arg;
      popToken(args, &arg);
      if (arg == "on") {
        radio_control = true;
      } else {
        radio_control = false;
      }
      return true;
    }
};

// ------------------------------------------------------------------------------
// Main program
// ------------------------------------------------------------------------------

// serial_queue[i] represents the current unprocessed input from the respective
// serial port.
String serial_queue[NUM_SERIAL_PORTS];

// Number of functions.
#define NUM_FUNCTIONS 10
Function* functions[NUM_FUNCTIONS];

void setup() {
  for (int pin = 0; pin < NUM_DIGITAL_PINS; ++pin) {
    digitalWrite(pin, LOW);
  }

  log_level = 0;
  for (int i = 0; i < NUM_SERIAL_PORTS; ++i) {
    serial_queue[i] = "";
  }

  functions[0] = new Reset();
  functions[1] = new PinMode();
  functions[2] = new DigitalWrite();
  functions[3] = new Pwm();
  functions[4] = new Ping();
  functions[5] = new LogLevel();
  functions[6] = new Init();
  functions[7] = new Stop();
  functions[8] = new Move();
  functions[9] = new Radio();

  Serial.begin(BAUD_RATE_PORT);
  while (!Serial);

#if NUM_SERIAL_PORTS > 1
  Serial1.begin(BAUD_RATE_PORT1);
  while (!Serial1);
#endif
#if NUM_SERIAL_PORTS > 2
  Serial2.begin(BAUD_RATE_PORT2);
  while (!Serial2);
#endif
#if NUM_SERIAL_PORTS > 3
  Serial3.begin(BAUD_RATE_PORT3);
  while (!Serial3);
#endif

#if USE_GYRO
  Wire.begin();
  if (gyro.init()) {
    // normal power mode, all axes enabled, 100 Hz
    // Low byte, bit 3 = power state, bits 0,1,2 = Y,X,Z axis enables
    gyro.writeReg(L3G_CTRL_REG1, 0x0F); 
    // 2000 dps full scale
    // High byte: (00: 250 dps; 01: 500 dps; 10: 2000 dps; 11: 2000 dps)
    gyro.writeReg(L3G_CTRL_REG4, 0x20);
    gyro.enableDefault();
  }
#endif

#if INIT_ON_STARTUP
  Init::init();
#endif
}

// Executes the function with the specified name and the comma separated list of arguments.
boolean exec(const String& func_name, String* args) {
  for (int i = 0; i < NUM_FUNCTIONS; ++i) {
    if (functions[i]->name() == func_name) {
      return functions[i]->exec(args);
    }
  }
  return false;
}

// True if the robot is currently radio controlled.
boolean radio_controlled = false;

// Handles commands received via radio.
void handleRadioControl() {
    float x = radio_joystick_x_handler.getJoystickPosition();
    float y = radio_joystick_y_handler.getJoystickPosition();
    float abs_x = abs(x);
    float abs_y = abs(y);
    // Ignore radio commands on idle position.
    // This allows USB or Bluetooth control in conjunction with radio control.
    if (abs_x + abs_y < 0.05f) {
      if (radio_controlled) {
        Stop::stop();
      }
      radio_controlled = false;
      return;
    }
    float speed = max(abs_x, abs_y);
    if (y < -0.1f) speed = -speed;
    float direction = 0.0f;
    if (abs_x > RADIO_X_ZERO_RANGE) {
      direction = 1.0f - abs_y / RADIO_XY_FACTOR;
      if (x < -RADIO_X_ZERO_RANGE) direction = -direction;
    }
    Move::move(speed, direction);
    radio_controlled = true;
}

void loop() {
  for (int i = 0; i < NUM_SERIAL_PORTS; ++i) {
    // Ignore long inputs.
    if (serial_queue[i].length() >= MAX_SERIAL_INPUT_SIZE)
     serial_queue[i] = "";
     
    // Pop next function call.
    int index = serial_queue[i].indexOf(';');
    if (index >= 0) {
      // Logging is always done on the default serial port.
      if (log_level >= LOG_INFO) Serial.println(serial_queue[i]);
 
      // Parse and execute function.
      String func_str = serial_queue[i].substring(0, index);
      index++;
      if (serial_queue[i].length() > index) {
        serial_queue[i] = serial_queue[i].substring(index);
      } else {
        serial_queue[i] = "";
      }
      if (func_str.length() >= 3 && func_str[func_str.length() - 1] == ')') {
        index = func_str.indexOf('(');
        if (index > 1) {
          String func_name = func_str.substring(0, index);
          String args = func_str.substring(index + 1, func_str.length() - 1);
          boolean success = exec(func_name, &args);
          if (log_level >= LOG_ERROR && !success) {
            Serial.println("error");
          }
        }
      }
    }
  }
  if (radio_control) {
    handleRadioControl();
  }
}

void serialEvent() {
  while (Serial.available() && serial_queue[0].length() < MAX_SERIAL_INPUT_SIZE) {
    char ch = (char) Serial.read();
    serial_queue[0] += ch;
  }
}

#if NUM_SERIAL_PORTS > 1
void serialEvent1() {
  while (Serial1.available() && serial_queue[1].length() < MAX_SERIAL_INPUT_SIZE) {
    char ch = (char) Serial1.read();
    serial_queue[1] += ch;
  }
}
#endif

#if NUM_SERIAL_PORTS > 2
void serialEvent2() {
  while (Serial2.available() && serial_queue[2].length() < MAX_SERIAL_INPUT_SIZE) {
    char ch = (char) Serial2.read();
    serial_queue[2] += ch;
  }
}
#endif

#if NUM_SERIAL_PORTS > 3
void serialEvent3() {
  while (Serial3.available() && serial_queue[3].length() < MAX_SERIAL_INPUT_SIZE) {
    char ch = (char) Serial3.read();
    serial_queue[3] += ch;
  }
}
#endif

