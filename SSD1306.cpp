#include <Arduino.h>
#include "SSD1306.h"
#include "RTOS.h"

// Define the display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define task handle
//TaskHandle_t xUpdateDisplayTaskHandle = NULL;


extern UpdateDisplayStruct DisplayData = {0, 0, 99, "", "", "", "", ""};

void AddDisplayLine(char data[21]) {
    for (int i = 0; i < 4; i++) {
      DisplayData.Line[i] = DisplayData.Line[i+1]; // shift lines up by one line
    }
    DisplayData.Line[4] = data; // add newly received data to the bottom/last line
}


void UpdateDisplayTask(void *pvParameters) {
  while (1) {
    display.clearDisplay();

    // Draw horizontal line
    display.drawLine(0, 16, SCREEN_WIDTH, 16, WHITE);  
    display.drawLine(0, 17, SCREEN_WIDTH, 17, WHITE);  

    // Set text size and print BaseState and RSSI
    display.setTextSize(2);
    display.setCursor(0, 0); 
    display.print(DisplayData.BaseState);

    display.setCursor(SCREEN_WIDTH-(3*12), 0); // x = 3 characters x 12 pixels each from the right
    if (DisplayData.RSSI < -98 || DisplayData.RSSI > 98) { // check out of bounds
      display.print("***");
    } else {
      display.print(DisplayData.RSSI);
    }

    // Set smaller text size and display lines 2-6
    display.setTextSize(1); 
    for (int i = 0; i < 5; i++) {
      display.setCursor(0, 11 + 9 * (i + 1));
      display.print(DisplayData.Line[i]);
    }

    // Refresh the display
    display.display();

    // Task delay to prevent high CPU usage
    vTaskDelay(DISPLAY_REFRESHRATE / portTICK_PERIOD_MS);  // Adjust delay time as needed
  }
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