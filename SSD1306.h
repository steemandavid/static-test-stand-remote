
// SSD1306 OLED display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif12pt7b.h>


// SSD1306 OLED display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define DISPLAY_REFRESHRATE 500 // in ms

//TaskHandle_t xUpdateDisplayTaskHandle;


extern Adafruit_SSD1306 display;

struct UpdateDisplayStruct {
  uint8_t BaseState;
  uint8_t Command;
  int8_t RSSI;
  String Line[5];
};

extern UpdateDisplayStruct DisplayData;


void UpdateDisplayTask(void *pvParameters);
void SetupDisplay();
void AddDisplayLine(char data[21]);
