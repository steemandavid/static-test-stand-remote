#include "outputs.h"
#include "tasksettings.h"


TaskHandle_t toggleTaskHandle[10]; // Array to hold task handles (supports 10 tasks maximum)
int taskCount = 0; // Count of currently active tasks


// RTOS task for toggling output at specified frequency
void toggleTask(void *param) {
    int pin = *((int *)param);
    float frequency = *((float *)param + 1);
    int delayMs = (int)(1000.0 / (2.0 * frequency)); // Half-period in ms

    while (1) {
        toggleOutput(pin);
        vTaskDelay(pdMS_TO_TICKS(delayMs));
    }
}

void startToggleTask(int pin, float frequency) {
    // Ensure we don't exceed the maximum number of tasks
    if (taskCount < 10) {
        // Dynamically allocate memory for parameters
        int *params = (int *)malloc(2 * sizeof(int)); 
        if (params != NULL) {
            params[0] = pin;
            *((float *)(&params[1])) = frequency;  // Store frequency as a float

            // Create the task
            xTaskCreatePinnedToCore(
                toggleTask, 
                "ToggleTask", 
                2048, 
                params, 
                TOGGLETASKPRIORITY, 
                &toggleTaskHandle[taskCount], 
                TOGGLETASKCORE
            );
            taskCount++; // Increment the task count
        } else {
            Serial.println("Failed to allocate memory for task parameters.");
        }
    } else {
        Serial.println("Maximum number of tasks reached.");
    }
}

void stopToggleTask(int pin) {
    for (int i = 0; i < taskCount; i++) {
        // Check if the task's pin matches the one we want to stop
        if (*((int *)toggleTaskHandle[i]) == pin) {
            vTaskDelete(toggleTaskHandle[i]);
            toggleTaskHandle[i] = NULL;
            taskCount--; // Decrease the task count
            break;
        }
    }
}

// Toggle output state
void toggleOutput(int pin) {
    digitalWrite(pin, !digitalRead(pin));
}

// Switch output on
void switchOutputOn(int pin) {
    digitalWrite(pin, HIGH);
}

// Switch output off
void switchOutputOff(int pin) {
    digitalWrite(pin, LOW);
}
