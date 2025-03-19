#include <Arduino.h>
#include "SSD1306.h"

// Define the display object (ensure the screen dimensions and reset pin are correct)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void UpdateDisplay() {
  display.clearDisplay();
  display.drawLine(0, 16, SCREEN_WIDTH, 16, WHITE);  // Horizontal line at the top of the blue area of the screen 
  display.drawLine(0, 17, SCREEN_WIDTH, 17, WHITE);  // Horizontal line at the top of the blue area of the screen 
  display.setTextSize(2); // set the font size, supports sizes from 1 to 8
  display.setCursor(0, 0);
  display.print(random(0, 999));
  display.setTextSize(1); // set the font size, supports sizes from 1 to 8
  for (int i=0; i<5; i++) { // Display line 2 through 6
    display.setCursor(0, 11+9*(i+1)); // put cursor on the correct y position for the respective line
    display.print(random(0, 999));
    delay(10);
  }
  display.display();
}

void SetupDisplay() {
  // SSD1306 OLED DISPLAY ///////////////////////////////////////
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  Serial.println("Starting SetupDisplay()...");
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
    // Add error handling here xxx todo
  } else {
    Serial.println(F("SSD1306 allocation succesfull"));
  }
  vTaskDelay( 100 / portTICK_PERIOD_MS ); // give the OLED some time to initialize
  display.setFont(); // Use default display font
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2); // set the font size, supports sizes from 1 to 8
  vTaskDelay( 100 / portTICK_PERIOD_MS ); // give the OLED some time to initialize
  display.clearDisplay();
}