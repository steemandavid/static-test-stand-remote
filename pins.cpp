#include <Arduino.h>
#include <Wire.h>
#include "pins.h"



void SetupPins() {
  Serial.println("Starting SetupPins()...");

  // Set up IO pins
  pinMode(BUTTON_BUTTON, INPUT_PULLUP); // Button input
  pinMode(SWITCH_ARM, INPUT); // Arm switch
  pinMode(VOLT_BAT, INPUT); // Analog voltage

  pinMode(LED_BUTTON, OUTPUT);  // LED ring around the button
  pinMode(LED_BUILTIN, OUTPUT); // ESP32 module built-in LED
  pinMode(BUZZER, OUTPUT); // Buzzer

  // Set up I²C interface
  Wire.setPins(I2C_SDA, I2C_SCL);
  Wire.begin(); // Start I²C bus
}