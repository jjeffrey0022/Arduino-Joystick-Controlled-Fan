/*
 * JoystickFan.ino
 *
 * Arduino Uno R3 – Joystick Controlled Fan
 *
 * Description:
 *   Reads the Y-axis of a 2-axis joystick module and maps the reading to a
 *   PWM duty cycle that controls a small 5 V DC fan (or motor) through a
 *   transistor driver.  Pushing the joystick fully forward runs the fan at
 *   full speed; pulling it back slows or stops the fan.  The joystick button
 *   (SW) is used as a kill-switch: press it to toggle the fan on/off.
 *
 * Wiring:
 *   Joystick module
 *     VCC  -> 5 V
 *     GND  -> GND
 *     VRy  -> A1   (Y-axis analog output)
 *     VRx  -> A0   (X-axis analog output – unused but wired for completeness)
 *     SW   -> D2   (button, active-low with internal pull-up)
 *
 *   Fan / motor driver (e.g. NPN transistor such as 2N2222 or TIP120)
 *     Arduino D9 (PWM) -> 1 kΩ resistor -> transistor Base
 *     Transistor Collector -> fan negative terminal
 *     Fan positive terminal -> 5 V (or external supply if fan needs >200 mA)
 *     Transistor Emitter -> GND
 *     Flyback diode across fan terminals (cathode toward +5 V)
 *
 * Author : jjeffrey0022
 * Date   : 2026
 * License: MIT
 */

// ── Pin definitions ──────────────────────────────────────────────────────────
const int PIN_JOY_X  = A0;  // Joystick X-axis  (not used for fan control)
const int PIN_JOY_Y  = A1;  // Joystick Y-axis  (controls fan speed)
const int PIN_JOY_SW = 2;   // Joystick button  (toggle fan on/off)
const int PIN_FAN    = 9;   // PWM output to fan transistor base

// ── Tunable constants ────────────────────────────────────────────────────────
const int  DEADZONE        = 20;   // ADC counts around centre (512) to ignore
const int  CENTER          = 512;  // Expected mid-point of 10-bit ADC
const long DEBOUNCE_MS     = 50;   // Button debounce time in milliseconds

// ── State variables ──────────────────────────────────────────────────────────
bool fanEnabled      = true;   // Master on/off flag toggled by SW button
bool lastButtonState = HIGH;   // Previous debounced button reading
long lastDebounceTime = 0;     // Time of last button-state change

// ── setup() ──────────────────────────────────────────────────────────────────
void setup() {
  pinMode(PIN_FAN,    OUTPUT);
  pinMode(PIN_JOY_SW, INPUT_PULLUP);  // Button pulls pin LOW when pressed

  analogWrite(PIN_FAN, 0);            // Fan off at startup

  Serial.begin(9600);
  Serial.println(F("Joystick Fan Ready"));
  Serial.println(F("Push joystick forward to increase speed."));
  Serial.println(F("Press joystick button to toggle fan on/off."));
}

// ── loop() ───────────────────────────────────────────────────────────────────
void loop() {
  handleButton();

  if (fanEnabled) {
    int speed = readFanSpeed();
    analogWrite(PIN_FAN, speed);

    // Print speed to Serial Monitor for debugging
    Serial.print(F("Fan speed: "));
    Serial.println(speed);
  } else {
    analogWrite(PIN_FAN, 0);
    Serial.println(F("Fan OFF (press button to enable)"));
  }

  delay(100);  // Small delay to avoid flooding Serial Monitor
}

// ── readFanSpeed() ────────────────────────────────────────────────────────────
// Reads the Y-axis and returns a PWM value (0-255).
// Joystick resting at centre (≈512) → fan stopped.
// Joystick pushed forward (low raw value toward 0) → full speed.
int readFanSpeed() {
  int raw = analogRead(PIN_JOY_Y);

  // Apply deadzone around centre
  if (abs(raw - CENTER) < DEADZONE) {
    return 0;
  }

  // Map lower half of joystick range to fan speed (0-255)
  // raw = 0   → speed 255 (full speed)
  // raw = CENTER-DEADZONE → speed 0 (stopped)
  if (raw < CENTER) {
    return map(raw, 0, CENTER - DEADZONE, 255, 0);
  }

  // Upper half of joystick (pulled back) → fan off
  return 0;
}

// ── handleButton() ────────────────────────────────────────────────────────────
// Debounced read of the joystick push-button.  Toggles fanEnabled on each press.
void handleButton() {
  bool reading = digitalRead(PIN_JOY_SW);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    // Stable LOW means button is pressed (active-low with pull-up)
    if (reading == LOW && lastButtonState == HIGH) {
      fanEnabled = !fanEnabled;
      Serial.print(F("Fan toggled: "));
      Serial.println(fanEnabled ? F("ON") : F("OFF"));
    }
  }

  lastButtonState = reading;
}
