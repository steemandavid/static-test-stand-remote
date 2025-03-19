# remote

**Static test stand - remote unit code**

The static test stand system consists of a base station and a remote unit. The base station is near the motor and serves to ignite the motor and measure different parameters before, during, and directly after the burn. The remote unit communicates over WiFi with the base station, displaying states and status messages while transmitting arm and ignition signals to the base station.

## Basic functionality

The remote unit serves two purposes:
- display the state and messages of/from the base station
- read Arm and Ignition user input and transmit it to the base station

The remote merely functions as a wireless "head unit" for the base station. It allows monitoring from a safe distance, as well as initiating the static test. All workflow for a test is ran by the base station.

The remote unit display the following:
- Top line of the display: the current state of the state machine running on the base station
- Top line of the display, right-hand side: a value representing the quality of the wifi link between remote and base station. This shows if the remote is (nearly) out of range
- 5 lines of messages from the base station. These lines scroll in a "terminal-style"; the most recent line is added to the bottom and pushes the other lines up.


## Hardware

The hardware consists of (also see pins.h):
- an ESP32 development board
- a built)in LED on the ESP32 dev board (LED_BUILTIN)
- an USB 1S LiPo charger module with integrated boost converter to power the ESP32 board (5V)
- An ignition button (BUTTON_BUTTON) with integrated LED ring around the button (LED_BUTTON)
- An arm switch, SPDT (SWITCH_ARM)
- An SSD1306 OLED display connected over I²C
- A voltage divider with one side towards ground, the other side towards battery +, and the middle connected to an ESP32 ADC input pin (VOLT_BAT)
- a buzzer with integrated oscillator, connected through a BC547 transistor to an ESP32 output pin (BUZZER)

## Code Structure

The code consists of the following files:

- **remote.ino**: The main sketch
- **RTOS**: Contains code to initialize the RTOS, some RTOS tasks, and settings for task priorities and which core to run on
- **SSD1306**: SSD1306 OLED display code
- **buzzer**: Contains code to beep the buzzer
- **debounce**: Button debounce code, used for ignition button and arm switch
- **espnow**: WiFi communication protocol code
- **outputs**: Code to toggle and switch on and off output pins
- **pins**: Pin definitions and initialization code
