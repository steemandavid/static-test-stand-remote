#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#define THISFILENAME "debounce.h"

#include <Arduino.h>

#define DEBOUNCE_TIME_MS 50     // 50 ms for debouncing
#define HOLD_THRESHOLD_MS 500   // 500 ms for detecting a long press

struct Button {
  int pin;
  bool lastState;
  bool currentState;
  bool debouncedState;
  unsigned long lastDebounceTime;
  unsigned long pressStartTime;
  bool eventFired;
};

// External declaration of buttons array and its size
extern Button buttons[];
extern const int numButtons;

// Function declarations
void SetupButtons();
bool debounceButton(Button &button);
int checkButtonEvent(Button &button);
void buttonTask(void *pvParameters); // Task function

#endif
