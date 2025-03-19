
# Remote Unit for Static Test Stand

## Description

The remote unit is part of a static test stand system designed for safe motor testing. It works in conjunction with a base station, which is located near the motor. The base station is responsible for igniting the motor and logging various parameters before, during, and immediately after the burn. The remote unit acts as a wireless head unit, communicating with the base station over WiFi to display statuses, show system messages, and send arm and ignition commands to the base station.

## System Overview

The system consists of two key components:

- **Base Station**: Located next to the motor, it controls the ignition process and collects data during the test.
- **Remote Unit**: This unit serves as a user interface for the base station. It communicates wirelessly with the base station to display information and allow user interaction.

### Primary Functions of the Remote Unit:
- **Display**: The remote unit displays the current state of the base station’s state machine and other status messages.
- **Control**: It receives user inputs for arming the system and triggering ignition, then transmits these commands to the base station.

The remote unit allows users to monitor and control the static test from a safe distance. It acts as a "head unit" that enables the operator to initiate the test, while the actual workflow and testing procedures are executed by the base station.

### Display Information

The remote unit's display provides the following information:
- **Top Left Corner**: Displays the current state of the base station’s state machine.
- **Top Right Corner**: Shows a value representing the quality of the WiFi link between the remote unit and the base station. This indicates whether the remote unit is nearing the edge of its wireless range.
- **Message Area**: The remaining five lines of the display are used to show messages from the base station. These messages scroll in a terminal-style fashion, with the most recent message appearing at the bottom and older messages scrolling upward.

## Hardware Components

The hardware setup consists of the following components (also see `pins.h` for pin assignments):

- **ESP32 Development Board**: The core of the remote unit, running the firmware.
- **Built-in LED**: On the ESP32 development board (controlled by `LED_BUILTIN`).
- **USB 1S LiPo Charger Module**: Charges the battery and powers the ESP32 via an integrated boost converter (5V output).
- **Ignition Button**: A button with an integrated LED ring (controlled by `BUTTON_BUTTON` and `LED_BUTTON` respectively).
- **Arm Switch**: A Single Pole Double Throw switch (controlled by `SWITCH_ARM`).
- **OLED Display**: An SSD1306 OLED display, connected via I²C to show system status and messages.
- **Battery Voltage Divider**: A simple voltage divider circuit to monitor the battery voltage. One side is connected to ground, the other to the battery positive terminal, and the middle is connected to an ESP32 ADC input pin (labeled `VOLT_BAT`). It converts the battery voltage (4.2V - 0V) down to a safe range for the ESP32's ADC (3.3 - 0V).
- **Buzzer**: A buzzer with an integrated oscillator, controlled by a BC547 transistor, connected to an ESP32 output pin (labeled `BUZZER`).

## Code Structure

The codebase is organized into several files, each serving a distinct function:

- **`remote.ino`**: The main Arduino sketch that ties everything together and runs the remote unit’s logic.
- **`RTOS`**: Code responsible for initializing and managing the Real-Time Operating System (RTOS), setting up tasks, and defining task priorities and core assignments.
- **`SSD1306`**: Contains code for controlling the SSD1306 OLED display, which displays system status and messages.
- **`buzzer`**: Code to control the buzzer, used for auditory feedback during operation.
- **`debounce`**: Implements debouncing for the ignition button and arm switch to prevent false triggering due to noisy button presses.
- **`espnow`**: Contains the WiFi communication protocol using ESPNOW for communication between the remote unit and the base station.
- **`outputs`**: Code to control the ESP32 output pins, including toggling and turning pins on and off for various actions.
- **`pins`**: Defines the pin assignments and initialization code for the hardware components.

This structured approach allows for modular updates and easy debugging. Each component of the system has a dedicated section in the code, making it clear and maintainable.

## To do

- Implement reading of battery voltage
- Add state machine to implement logic (reacting to buttons, updating display, reading battery voltage, controlling LEDs and buzzer)
- Implement a "ping-like" protocol which pings the base and will be used to assess wifi link quality (out of range detection)
- Implement protocol to send commands from remote to base
- Implement protocol to send state info and messages from base to remote
