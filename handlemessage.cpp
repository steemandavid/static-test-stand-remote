#include "handleMessage.h"
#include "pins.h"
#include "outputs.h"
#include "buzzer.h"
#include "SSD1306.h"

//char* TAG = "handlemessage.cpp";


void handleMessage(MsgStruct message) {
    printf("Received command: %i\n", message.Command);
//    ESP_LOGW(TAG, "Received command %i\n", message.Command);
    switch (message.Command) {
        // ** Action Commands **
        case 1: // Button LED on for x seconds
          startSwitchPinOnForSeconds(LED_BUTTON, atoi(message.Data)); // pin, on time in seconds
          break;
        case 2: // Button LED blink
          startToggleTask(LED_BUTTON, atoi(message.Data)); //void startToggleTask(int pin, int frequency)
          break;
        case 3: // Buzzer on for x seconds
          startSwitchPinOnForSeconds(BUZZER, atoi(message.Data)); // pin, on time in seconds
          break;
        case 4: // Buzzer blink
          beepBuzzer(BUZZER, atoi(message.Data), 150, 50);  // beepBuzzer(int pin, int beepCount, int onTime, int offTime) {
          break;
        case 5: // Built-in LED on for x seconds
          startSwitchPinOnForSeconds(LED_BUILTIN, atoi(message.Data)); // pin, on time in seconds
          break;
        case 6: // Built-in LED blink
          startToggleTask(LED_BUILTIN, atoi(message.Data)); //void startToggleTask(int pin, int frequency)
          break;

        // ** Display Commands **
        case 10: // Clear display
          for (int i = 0; i < 5; i++)  DisplayData.Line[i]=""; // Clear log lines
          printf("Display cleared\n");
          break;
        case 11: // Add display log line
            AddDisplayLine(message.Data);
            break;

        // ** Communication Commands **
        case 20: // Ping - pong
          EspNowSend(&message); // Send ping command back as received
          break;
        case 21: // Comms link issues
          // TODO: Handle communication issues
          break;

        default:
          // Unknown command
          printf("Unknown command\n");
          break;
    }
}
