#ifndef PINS_H
#define PINS_H

// I/O defines
//#define LED1 15
//#define LED2 2
//#define LED3 4
#define BUTTON_BUTTON 16
#define LED_BUTTON 2
#define SWITCH_ARM 27
#define LED_BUILTIN 32
#define BUZZER 25
#define VOLT_BAT 33 

#define I2C_SDA 4 // standard ESP32 is 21
#define I2C_SCL 5 // standard ESP32 is 22


void SetupPins();


#endif