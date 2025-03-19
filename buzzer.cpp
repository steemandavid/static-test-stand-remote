#include "buzzer.h"
#include "tasksettings.h"



void beepBuzzer(int pin, int beepCount, int onTime, int offTime) {
    xTaskCreatePinnedToCore([](void* pvParameters) {
        int* params = (int*)pvParameters;
        int beepCount = params[0];
        int pin = params[1];
        int onTime = params[2];
        int offTime = params[3];

        for (int i = 0; i < beepCount; i++) {
            digitalWrite(pin, HIGH); // Turn buzzer ON
            vTaskDelay(pdMS_TO_TICKS(onTime));
            digitalWrite(pin, LOW); // Turn buzzer OFF
            vTaskDelay(pdMS_TO_TICKS(offTime));
        }

        delete[] params; // Free allocated memory
        vTaskDelete(NULL);  // Delete the task when done
    },
    "BuzzerBeepTask",                 // Task name
    2048,                             // Stack size
    new int[4]{beepCount, pin, onTime, offTime}, // Task parameters
    BUZZERBEEPTASKPRIORITY,           // Task priority
    NULL,                             // Task handle
    BUZZERBEEPTASKCORE                // Pin to Core 
    );
}
