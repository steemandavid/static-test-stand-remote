# remote

Static test stand - remote unit code

The static test stand system consists of a base station and a remote unit. The base station is near the motor and serves to ignite the motor and measure different parameters before, during, and directly after the burn. The remote unit communicates over WiFi with the base station, displaying states and status messages while transmitting arm and ignition signals to the base station.

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
