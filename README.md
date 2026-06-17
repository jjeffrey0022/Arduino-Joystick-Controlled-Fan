# Arduino-Joystick-Controlled-Fan
A simple Arduino Uno R3 project that uses a joystick module to control the speed of a DC fan. The joystick's analog input is read by the Arduino and mapped to a PWM signal that adjusts the fan speed in real time.

## Overview

This project demonstrates:

- Analog sensor input using a joystick module
- Pulse Width Modulation (PWM)
- DC motor/fan control
- Arduino programming
- Basic embedded systems design

## Components

- Arduino Uno R3
- Joystick Module
- DC Motor / Fan
- Motor Driver (L293D, L298N, or transistor circuit)
- Breadboard
- Jumper Wires
- External power source (optional)

## Wiring

### Joystick

| Joystick Pin | Arduino Pin |
|-------------|-------------|
| VCC | 5V |
| GND | GND |
| VRx | A0 |
| VRy | A1 |
| SW | D2 (Optional) |

### Motor Driver

| Driver Pin | Arduino Pin |
|------------|------------|
| Enable | D10 (PWM) |
| Input 1 | D9 |
| GND | GND |

## How It Works

The Arduino continuously reads the joystick position through the analog input pin.

- Center position keeps the fan off.
- Moving the joystick increases the PWM duty cycle.
- Higher PWM values result in higher fan speeds.

The fan speed is controlled using Arduino's `analogWrite()` function.

## Code

```cpp
const int joystickX = A0;
const int motorPWM = 10;
const int motorDir = 9;

void setup() {
  pinMode(motorPWM, OUTPUT);
  pinMode(motorDir, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int xValue = analogRead(joystickX);

  int speedValue;

  if (xValue > 550) {
    digitalWrite(motorDir, HIGH);
    speedValue = map(xValue, 550, 1023, 0, 255);
    analogWrite(motorPWM, speedValue);
  }
  else {
    analogWrite(motorPWM, 0);
  }

  delay(20);
}
```

## Future Improvements

- Add OLED display for speed percentage
- Add bidirectional motor control
- Implement multiple speed modes
- Add temperature-based automatic control
- Design and 3D print an enclosure

## Skills Demonstrated

- Arduino Programming
- Embedded Systems
- PWM Motor Control
- Sensor Interfacing
- Circuit Design
- Hardware Debugging

## Author

Jeffrey Joseph

UCLA Electrical Engineering Student
