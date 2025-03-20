
# Remote Unit for rocket motor Static Test Stand

## Description

The remote unit is part of a static test stand system designed for amateur rocket motor testing. It works in conjunction with a base station, which is located near the motor. The base station is responsible for igniting the motor and logging various parameters before, during, and immediately after the burn. The remote unit acts as a wireless head unit, communicating with the base station over WiFi to display statuses, show system messages, and send arm and ignition commands to the base station.

## System Overview

The system consists of two key components:

- **Base Station**: Located next to the motor, it controls the ignition process and collects data during the test.
- **Remote Unit**: This unit serves as a user interface for the base station. It communicates wirelessly with the base station to display information and allow user interaction.

This repository is for the remote unit only. The base station has a separate repository (not yet available). 

### Primary Functions of the Remote Unit:
- **Display**: The remote unit displays the current state of the base station’s state machine and other status messages.
- **Control**: It receives user inputs for arming the system and triggering ignition, then transmits these commands to the base station.

The remote unit allows users to monitor and control the static test from a safe distance. It acts as a "head unit" that enables the operator to initiate the test, while the actual workflow and testing procedures are executed by the base station.

### The display

The remote unit's display provides the following information:
- **Top Left Corner**: Displays the current state of the base station’s state machine.
- **Top Right Corner**: Shows a value representing the quality of the WiFi link between the remote unit and the base station. This indicates whether the remote unit is nearing the edge of its wireless range.
- **Message Area**: The remaining five lines of the display are used to show messages from the base station. These messages scroll in a terminal-style fashion, with the most recent message appearing at the bottom and older messages scrolling upward.

### The Arm switch

The Arm switch is a single pole double throw switch. The Arm switch is used to arm the test stand (going from safe mode to unsafe pre-ignition mode). To do this, the switch needs to be flipped up for at least 500ms and then flipped down again. The system will now enter arm mode (= the remote will send an Arm command to the base station). A protective cover prevents accidental flipping of the switch. 

### The Ignition Button

The ignite button is a round button with an itegrated LED ring around it. When the system is in Armed mode, the LED ring around the button flashes. When the user pushes the Ignition button, a command is sent to the base station which then fires the ignition charge.


## Hardware Components

The hardware setup consists of the following components (also see `pins.h` for pin assignments):

- **ESP32 Development Board**: The core of the remote unit, running the firmware. The test unit is a NodeMCU-32S dev board, the production unit will be a custom board based on ESP-WROOM-32D
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

## Real Time Operating System

A Real Time Operating System (RTOS) is used to concurrently run different tasks. It allows assigning tasks to one of the 2 cores, and assigning priorities so that critical tasks have a better chance of getting processing cycles assigned to them.

### The State Machine

The logic of the remote is managed by a State Machine. It moves between the following states:

#### Init

Initialization of peripherals, tasks, variables and functions.
Afterwards, the state is set to Idle.

#### Idle

Give a low voltage warning and a critical voltage warning.
Check wifi connection quality and react to connection issues.
Check for Arm switch user input. 
- If the Arm switch is flipped on, the buzzer will emit short beeps.
- If the Arm switch was flipped on for 5 seconds, the state is set to Arm. 
- If the Arm switch was flipped shorter than that time, the buzzer is set to off and the state machines stays in the Idle state.

#### Arm

The system is in Arm mode, waiting for the user to push the Ignition button. An Arm command is sent to the base station. The buzzer is switched on to indicate the danger of imminent ignition. 

If the Ignition button is pushed (short push), an Ignition command is sent to the base station.
If the Ignition isn't pushed with 10 seconds, the Arm mode times out and the system returns to idle state.

#### Ignite

An Arm command is sent to the base station. The buzzer is switched off.
Afterwards, the state is set to End.

#### End

The static test is over. The remote stays locked in this state until it is powered off.

### Background tasks

The following RTOS tasks run in the background. They can be enabled and suspended as required.

#### Wifi communications

Handles the exchange of messages between the remote unit and the base station over ESPNOW wifi protocol. Functions:
- receive messages
- send messages
- send an empty message every second, and update wifi connection quality variable according to packet send errors (shift success or fail bit into the left of a 32 bit variable).

#### Update Display

- Checks if any of the variables to display has changed. If not, do nothing.
- Clear the screen, draw the dividing line, display state and wifi connection quality information and the 5 message lines.

#### Input - Output

- Debounce inputs, check for long and short presses.
- Avoid reporting repeated keypresses
- Control the LEDs and buzzer

#### Battery voltage

- Regularly measure the battery voltage.
- Filter the read values using a simple filter.
- Report low battery voltage to the state machine. 


## To do

- Implement protocol to send commands from remote to base
- Implement protocol to send state info and messages from base to remote
