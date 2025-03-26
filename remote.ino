#define THISFILENAME "espnow.cpp"
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

static const char *TAG = THISFILENAME;

#include <Arduino.h>
#include <driver/ledc.h>  // for PWM functions

#include "espnow.h"
#include "SSD1306.h"
#include "pins.h"
#include "RTOS.h"
#include "debounce.h"
#include "outputs.h"
#include "buzzer.h"
#include "battery.h"
#include "handlemessage.h"

// MsgStruct MessageReceived, MessageReceivedPrev, MessageToSend;

void setup(void) {
  ESP_LOGI(THISFILENAME, "Initializing...");

  SetupEspNow();  // Initialize ESPNOW communication
  SetupPins();    // Initialize I/O pins
  SetupDisplay();  // Initialize SSD1306 OLED DISPLAY
  SetupButtons();  // Initialize button detection
  SetupRTOSTasks(); // Start RTOS tasks
  SetupBatteryMonitor(); // Start battery monitor task

  MessageToSend.BaseState = 0;

  ESP_LOGI(THISFILENAME, "Current firmware file: %s", __FILE__);
  ESP_LOGI(THISFILENAME, "\nSetup() complete\n");

  startToggleTask(LED_BUILTIN, 5.0); // Toggle pin according to frequency in Hz
  // startToggleTask(LED_BUTTON, 0.5); // Toggle pin according to frequency in Hz

  beepBuzzer(BUZZER, 1, 100, 500); // Beep 5 times, 100ms on, 500ms off
  // delay(1000);                      // Wait for 5 seconds
}

void loop(void) {
//  ESP_LOGI(THISFILENAME, "BatteryVoltage: %f", BatteryVoltage);
  vTaskDelay(1000 / portTICK_PERIOD_MS);   // xxx debug
}



void ClearContents(struct MsgStruct *p) {
  memset(p, 0, sizeof(MsgStruct));
}
