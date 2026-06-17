# Arduino Joystick Controlled Fan

A beginner-friendly Arduino project built with the **Arduino Uno R3 Starter Kit**.  
A 2-axis joystick module controls the speed of a small DC fan via PWM:  
push the joystick forward → fan speeds up; pull it back → fan slows down; press the joystick button → toggle the fan on/off.

---

## Table of Contents

1. [Project Overview](#project-overview)
2. [Components Required](#components-required)
3. [Circuit Wiring](#circuit-wiring)
4. [How It Works](#how-it-works)
5. [Setting Up Visual Studio Code](#setting-up-visual-studio-code)
6. [Uploading the Code](#uploading-the-code)
7. [Using the Serial Monitor](#using-the-serial-monitor)
8. [Troubleshooting](#troubleshooting)
9. [License](#license)

---

## Project Overview

| Property | Details |
|---|---|
| Microcontroller | Arduino Uno R3 |
| Language | Arduino C/C++ |
| Difficulty | Beginner |
| Estimated build time | 30–45 minutes |

The sketch (`JoystickFan/JoystickFan.ino`) reads the Y-axis of a joystick module and maps it to a PWM signal on pin 9.  That signal drives the base of an NPN transistor which switches current through the fan motor.

---

## Components Required

All of the following are typically included in a standard Arduino Uno R3 Starter Kit:

| Qty | Component |
|-----|-----------|
| 1 | Arduino Uno R3 |
| 1 | USB-A to USB-B cable (to connect Uno to PC) |
| 1 | 2-axis joystick module (KY-023 or equivalent) |
| 1 | Small 5 V DC fan **or** DC motor + fan blade |
| 1 | NPN transistor (2N2222, PN2222, or TIP120) |
| 1 | 1 kΩ resistor (base resistor for transistor) |
| 1 | 1N4007 flyback diode (protects transistor from motor back-EMF) |
| 1 | Breadboard |
| — | Jumper wires (M-M) |

---

## Circuit Wiring

### Joystick Module → Arduino

| Joystick Pin | Arduino Pin |
|---|---|
| VCC | 5 V |
| GND | GND |
| VRy (Y-axis) | A1 |
| VRx (X-axis) | A0 *(wired but not used for fan)* |
| SW (button) | D2 |

### Fan / Motor Driver → Arduino

```
Arduino D9 (PWM)
        │
      [1 kΩ]
        │
    ┌───┤ Base   (NPN transistor, e.g. 2N2222)
    │   ├─ Collector ──── Fan (−) terminal
    │   └─ Emitter  ──── GND
    │
5 V ────────────────── Fan (+) terminal
              │
           [1N4007 diode]   (cathode toward +5 V, across fan terminals)
              │
            GND
```

> **Note:** If your fan draws more than ~200 mA, use a TIP120 Darlington transistor and power the fan from an external 5 V supply sharing the same GND as the Arduino.

---

## How It Works

1. **Joystick Y-axis** is read as a 10-bit ADC value (0–1023) on pin A1.  
   - Resting centre ≈ 512  
   - Pushed fully forward ≈ 0  
   - Pulled fully back ≈ 1023  
2. Values in the **lower half** (0–512) are mapped to a PWM duty cycle (255–0), running the fan faster when the joystick is pushed further forward.  
3. Values in the **upper half** (512–1023, joystick pulled back) keep the fan off.  
4. A small **deadzone** (±20 counts around centre) prevents the fan from spinning at very low, noisy readings.  
5. The joystick **push-button** (SW) toggles the fan on or off with software debouncing.

---

## Setting Up Visual Studio Code

VS Code with the **Arduino extension** gives you IntelliSense, code navigation, and one-click upload—all without installing the full Arduino IDE.

### Step 1 – Install prerequisites

1. Download and install **[Arduino IDE](https://www.arduino.cc/en/software)** (even if you use VS Code, the IDE installs the compiler toolchain and board packages that the VS Code extension relies on).
2. Download and install **[Visual Studio Code](https://code.visualstudio.com/)**.

### Step 2 – Install the Arduino extension in VS Code

1. Open VS Code.
2. Click the **Extensions** icon in the left sidebar (or press `Ctrl+Shift+X`).
3. Search for **Arduino** and install the extension published by **Microsoft**.
4. Restart VS Code when prompted.

### Step 3 – Configure the extension

1. Open VS Code **Settings** (`Ctrl+,`) and search for `arduino`.
2. Set **Arduino: Path** to the folder where you installed the Arduino IDE  
   - Windows example: `C:\Program Files\Arduino IDE`  
   - macOS example: `/Applications/Arduino IDE.app`  
   - Linux example: `/usr/share/arduino`
3. Open the **Command Palette** (`Ctrl+Shift+P`) and run **Arduino: Board Manager**.  
   Search for **Arduino AVR Boards** and make sure it is installed.

### Step 4 – Open the project

1. Clone or download this repository:
   ```bash
   git clone https://github.com/jjeffrey0022/Arduino-Joystick-Controlled-Fan.git
   ```
2. In VS Code choose **File → Open Folder** and select the cloned folder.
3. Open `JoystickFan/JoystickFan.ino`.

### Step 5 – Select board and port

Open the **Command Palette** (`Ctrl+Shift+P`) and run each of these commands:

| Command | Value to select |
|---|---|
| `Arduino: Select Board` | Arduino Uno |
| `Arduino: Select Serial Port` | The COM port your Uno is plugged into (e.g. `COM3` on Windows, `/dev/ttyACM0` on Linux/macOS) |

The selected board and port are shown in the blue VS Code status bar at the bottom of the window.

---

## Uploading the Code

1. Connect the Arduino Uno to your PC with the USB cable.
2. Make sure the correct board and port are selected (see Step 5 above).
3. Open `JoystickFan/JoystickFan.ino` in VS Code.
4. Open the **Command Palette** (`Ctrl+Shift+P`) and run **Arduino: Upload**,  
   **or** click the **→ Upload** arrow button in the top-right toolbar of the editor.
5. The sketch compiles and uploads. The Uno's built-in LED will blink during transfer.
6. When the output panel shows `Done uploading`, the fan is ready to use.

---

## Using the Serial Monitor

The sketch prints fan speed and button events to the serial port at **9600 baud** for easy debugging:

1. Open the **Command Palette** and run **Arduino: Open Serial Monitor**.
2. Make sure the baud rate is set to **9600**.
3. Move the joystick and press the button — you will see output like:

```
Joystick Fan Ready
Push joystick forward to increase speed.
Press joystick button to toggle fan on/off.
Fan speed: 0
Fan speed: 128
Fan speed: 255
Fan toggled: OFF
Fan OFF (press button to enable)
```

---

## Troubleshooting

| Symptom | Possible cause | Fix |
|---|---|---|
| Fan doesn't spin at all | Wrong pin, loose wire, or transistor installed backwards | Double-check wiring against the diagram above |
| Fan spins at full speed constantly | Joystick Y-axis wired incorrectly | Make sure VRy goes to A1, not A0 |
| Upload fails with "Port not found" | Wrong COM port selected or driver missing | Install the CH340 or FTDI driver for your Uno clone; reselect the port |
| VS Code says "arduino-cli not found" | Arduino path not configured | Set `Arduino: Path` in VS Code settings to your Arduino IDE install folder |
| Fan runs but is very weak | Fan needs more current than the transistor can supply | Use a TIP120 Darlington + external power supply |
| Button doesn't toggle | SW not wired to D2 | Verify joystick SW pin → Arduino D2 |

---

## License

This project is released under the [MIT License](LICENSE).  
Feel free to use, modify, and share it.
