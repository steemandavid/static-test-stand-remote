#define THISFILENAME "debounce.cpp"
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include <esp_log.h>
#include "debounce.h"
#include "pins.h"
#include "RTOS.h"

static const char *TAG = THISFILENAME;

Button buttons[] = {
  {BUTTON_BUTTON, HIGH, HIGH, HIGH, 0, 0, false},  // BUTTON_BUTTON
  {SWITCH_ARM, HIGH, HIGH, HIGH, 0, 0, false}      // SWITCH_ARM
};

const int numButtons = sizeof(buttons) / sizeof(buttons[0]);

void SetupButtons() {
  // Configure the button pins
  for (int i = 0; i < numButtons; ++i) {
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }
  // Create and start the button detection task
  //  xTaskCreate(buttonTask, "ButtonTask", 2048, NULL, BUTTONTASKPRIORITY, NULL);
  xTaskCreatePinnedToCore(
    buttonTask,        // Task function
    "buttonTask",      // Task name
    2048,              // Stack size
    NULL,              // Task parameters
    BUTTONTASKPRIORITY,// Priority
    NULL,              // Task handle
    BUTTONTASKCORE     // Pin to Core
  );
}


// Debounce function definition
bool debounceButton(Button &button) {
  bool currentState = digitalRead(button.pin);
  
  if (currentState != button.lastState) {
    button.lastDebounceTime = millis();  // Reset debounce timer
  }

  if ((millis() - button.lastDebounceTime) > DEBOUNCE_TIME_MS) {
    if (currentState != button.debouncedState) {
      button.debouncedState = currentState;
      
      if (button.debouncedState == LOW) {
        // Button was just pressed down
        button.pressStartTime = millis();
      } else if (button.debouncedState == HIGH) {
        // Button was just released — handle event here
        if (!button.eventFired) {
          button.eventFired = true;
          return true;  // Trigger event only after release
        }
      }
    }
  }

  button.lastState = currentState;
  return false;
}

// Check if button event occurred (returns 0 = no press, 1 = short press, 2 = long press)
int checkButtonEvent(Button &button) {
  if (debounceButton(button)) {
    unsigned long pressDuration = millis() - button.pressStartTime;

    if (pressDuration >= HOLD_THRESHOLD_MS) {
      ESP_LOGI(TAG, "Button on pin %d Long Press Detected!", button.pin);
      
      button.eventFired = false;
      return 2; // Long press
    } else {
      ESP_LOGI(TAG, "Button on pin %d Short Press Detected!", button.pin);
      
      button.eventFired = false;
      return 1; // Short press
    }
  }

  return 0; // No press
}

// Task to handle button events
void buttonTask(void *pvParameters) {
  while (true) {
    for (int i = 0; i < numButtons; ++i) {
      int event = checkButtonEvent(buttons[i]);
      if (event == 1) {
        ESP_LOGI(TAG, "Short press detected on button %d", buttons[i].pin);
      } else if (event == 2) {
        ESP_LOGI(TAG, "Long press detected on button %d", buttons[i].pin);
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
