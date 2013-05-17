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

#define VERSION "ScriptDuino 0.1 [2013.05.17.00]"

#define BOARD "Arduino Mega 2560"
//#define BOARD "Arduino BT 328"
#define BAUD_RATE 9600
//#define BAUD_RATE 115200
#define MAX_INPUT_SIZE 512
#define NUM_FUNCTIONS 5

boolean echo;

class Function {
  public:
    explicit Function(const String& name)
      : name_(name) {
    }
    
    virtual ~Function() {
    }
    
    const String& name() const {
      return name_;
    }

    virtual boolean exec(String* args) const = 0;
    
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
    
    static void popInteger(String* args, int* result) {
      String str;
      popToken(args, &str);
      *result = str.toInt();
    }
    
  private:
    String name_;
};

class Ping : public Function {
  public:
    Ping()
      : Function("ping") {
    }
    
    virtual boolean exec(String* args) const {
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

class Echo : public Function {
  public:
    Echo()
      : Function("echo") {
    }
    
    virtual boolean exec(String* args) const {
      int value = 0;
      popInteger(args, &value);
      echo = value == 1;
      return true;
    }
};

class Reset : public Function {
  public:
    Reset()
      : Function("reset") {
    }
    
    virtual boolean exec(String* args) const {
      for (int pin = 0; pin < NUM_DIGITAL_PINS; ++pin) {
        digitalWrite(pin, LOW);
      }
      return true;
    }
};

class DigitalWrite : public Function {
  public:
    DigitalWrite()
      : Function("dwrite") {
    }
    
    virtual boolean exec(String* args) const {
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

class Pwm : public Function {
  public:
    Pwm()
      : Function("pwm") {
    }
    
    virtual boolean exec(String* args) const {
      int pin = 0;
      int value = 0;
      popInteger(args, &pin);
      popInteger(args, &value);
      if (pin < 0 || pin >= NUM_DIGITAL_PINS) return false;
      analogWrite(pin, value);
      return true;
    }
};

String input;
Function* functions[NUM_FUNCTIONS];

void setup() {
  TCCR1B = 1;

  for (int pin = 0; pin < NUM_DIGITAL_PINS; ++pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  input = "";
  functions[0] = new DigitalWrite();
  functions[1] = new Pwm();
  functions[2] = new Ping();
  functions[3] = new Echo();
  functions[4] = new Reset();
  echo = false;
  
  Serial.begin(BAUD_RATE);
  while (!Serial);
  Serial2.begin(115200);
  while (!Serial2);
}

boolean exec(const String& func_name, String* args) {
  for (int i = 0; i < NUM_FUNCTIONS; ++i) {
    if (functions[i]->name() == func_name) {
      return functions[i]->exec(args);
    }
  }
  return false;
}

void loop() {
  if (input.length() >= MAX_INPUT_SIZE) input = "";
  int index = input.indexOf(';');
  if (index >= 0) {
    if (echo) Serial.println(input);
    String func_str = input.substring(0, index);
    index++;
    if (input.length() > index) {
      input = input.substring(index);
    } else {
      input = "";
    }
    if (func_str.length() >= 3 && func_str[func_str.length() - 1] == ')') {
      index = func_str.indexOf('(');
      if (index > 1) {
        String func_name = func_str.substring(0, index);
        String args = func_str.substring(index + 1, func_str.length() - 1);
        boolean success = exec(func_name, &args);
        if (echo) {
          Serial.println(success ? "ok" : "error");
        }
      }
    }
  }
}

void serialEvent() {
  while (Serial.available() && input.length() < MAX_INPUT_SIZE) {
    char ch = (char) Serial.read();
    input += ch;
  }
}

void serialEvent1() {
  while (Serial1.available() && input.length() < MAX_INPUT_SIZE) {
    char ch = (char) Serial1.read();
    input += ch;
  }
}

void serialEvent2() {
  while (Serial2.available() && input.length() < MAX_INPUT_SIZE) {
    char ch = (char) Serial2.read();
    input += ch;
  }
}

void serialEvent3() {
  while (Serial3.available() && input.length() < MAX_INPUT_SIZE) {
    char ch = (char) Serial3.read();
    input += ch;
  }
}

