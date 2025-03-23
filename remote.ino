#include <Arduino.h>
#include <driver/ledc.h>  // for PWM functions

#include "espnow.h"
#include "SSD1306.h"
#include "pins.h"
#include "RTOS.h"
#include "debounce.h"
#include "outputs.h"
#include "buzzer.h"


MsgStruct MessageReceived, MessageReceivedPrev, MessageToSend;


void setup(void) {
  Serial.begin(115200);

  SetupEspNow();  // Initialize ESPNOW communication
  SetupPins();    // Initialize I/O pins
  SetupDisplay();  // Initialize SSD1306 OLED DISPLAY
  SetupButtons();  // Initialize button detection
  SetupRTOSTasks(); // Start RTOS tasks

  MessageToSend.BaseState = 0;

  Serial.print("Current firmware file: ");
  Serial.println(__FILE__);
  Serial.println("\nSetup() complete\n");

  startToggleTask(LED_BUILTIN, 3.0); // Toggle pin according to frequency in Hz
  startToggleTask(LED_BUTTON, 0.5); // Toggle pin according to frequency in Hz

//  beepBuzzer(BUZZER, 5, 200, 500); // Beep 5 times, 200ms on, 200ms off
//  delay(1000);                      // Wait for 5 seconds
}


void loop(void) {
//  Serial.println("*");
//  vTaskDelay( 1000 / portTICK_PERIOD_MS );   //xxx debug
}



void ClearContents(struct MsgStruct *p) {
    memset(p, 0, sizeof(MsgStruct));
}
