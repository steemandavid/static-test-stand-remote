#include "outputs.h"
#include "RTOS.h"

#define MAX_TASKS 10

TaskHandle_t toggleTaskHandle[MAX_TASKS] = { NULL };
int togglePinMapping[MAX_TASKS] = { -1 };

TaskHandle_t switchTaskHandle[MAX_TASKS] = { NULL };
int switchPinMapping[MAX_TASKS] = { -1 };


// debug : there's still a bug in the code which causes tasks to restart when calling other tasks, and ESP32 sometimes crashing after repeatedly calling the same function

void stopTask(int pin) {
    // Stop toggle task if running
    for (int i = 0; i < MAX_TASKS; i++) {
        if (togglePinMapping[i] == pin) {
            if (toggleTaskHandle[i] != NULL) {
                vTaskDelete(toggleTaskHandle[i]);
                toggleTaskHandle[i] = NULL;
                togglePinMapping[i] = -1;
                Serial.print("Stopped toggle task for pin ");
                Serial.println(pin);
            }
        }
        if (switchPinMapping[i] == pin) {
            if (switchTaskHandle[i] != NULL) {
                vTaskDelete(switchTaskHandle[i]);
                switchTaskHandle[i] = NULL;
                switchPinMapping[i] = -1;
                Serial.print("Stopped switch task for pin ");
                Serial.println(pin);
            }
        }
    }
}

// === TOGGLE TASK ===
void toggleTask(void *param) {
    int pin = *((int *)param);
    float frequency = *((float *)((int *)param + 1));
    int delayMs = (int)(1000.0 / (2.0 * frequency));

    free(param);

    if (pin < 0) {
        vTaskDelete(NULL);
        return;
    }

    Serial.print("Toggling pin ");
    Serial.print(pin);
    Serial.print(" at ");
    Serial.print(frequency);
    Serial.println(" Hz");

    while (1) {
        toggleOutput(pin);
        vTaskDelay(pdMS_TO_TICKS(delayMs));
    }
}

void startToggleTask(int pin, float frequency) {
    // Stop any existing task for the pin
    stopTask(pin);

    if (frequency == 0) {
        return; // Do nothing if frequency is zero
    }

    for (int i = 0; i < MAX_TASKS; i++) {
        if (toggleTaskHandle[i] == NULL) {
            void *params = malloc(sizeof(int) + sizeof(float));
            if (params != NULL) {
                *((int *)params) = pin;
                *((float *)((int *)params + 1)) = frequency;

                if (xTaskCreatePinnedToCore(
                        toggleTask,
                        "ToggleTask",
                        4096,
                        params,
                        1,
                        &toggleTaskHandle[i],
                        1) == pdPASS) {
                    togglePinMapping[i] = pin;
                    Serial.print("Started toggle task for pin ");
                    Serial.print(pin);
                    Serial.print(" at ");
                    Serial.print(frequency);
                    Serial.println(" Hz");
                } else {
                    free(params);
                    Serial.println("Failed to create toggle task.");
                }
            } else {
                Serial.println("Failed to allocate memory for toggle task.");
            }
            break;
        }
    }
}

// === SWITCH TASK ===
void switchPinOnForSeconds(void *param) {
    int pin = *((int *)param);
    int seconds = *((int *)param + 1);

    free(param);

    if (pin < 0) {
        vTaskDelete(NULL);
        return;
    }

    Serial.print("Switching pin ");
    Serial.print(pin);
    Serial.print(" ON for ");
    Serial.print(seconds);
    Serial.println(" seconds");

    digitalWrite(pin, HIGH);
    vTaskDelay(pdMS_TO_TICKS(seconds * 1000));
    digitalWrite(pin, LOW);

    Serial.print("Switching pin ");
    Serial.print(pin);
    Serial.println(" OFF");

    vTaskDelete(NULL);
}

void startSwitchPinOnForSeconds(int pin, int seconds) {
    // Stop any existing task for the pin
    stopTask(pin);

    if (seconds == 0) {
        return; // Do nothing if seconds is zero
    }

    for (int i = 0; i < MAX_TASKS; i++) {
        if (switchTaskHandle[i] == NULL) {
            int *params = (int *)malloc(2 * sizeof(int));
            if (params != NULL) {
                params[0] = pin;
                params[1] = seconds;

                if (xTaskCreatePinnedToCore(
                        switchPinOnForSeconds,
                        "SwitchPinOnForSeconds",
                        4096,
                        params,
                        1,
                        &switchTaskHandle[i],
                        1) == pdPASS) {
                    switchPinMapping[i] = pin;
                    Serial.print("Started switch task for pin ");
                    Serial.print(pin);
                    Serial.print(" for ");
                    Serial.print(seconds);
                    Serial.println(" seconds");
                } else {
                    free(params);
                    Serial.println("Failed to create switch task.");
                }
            } else {
                Serial.println("Failed to allocate memory for switch task.");
            }
            break;
        }
    }
}

// === OUTPUT CONTROL ===
void toggleOutput(int pin) {
    digitalWrite(pin, !digitalRead(pin));
}

void switchOutputOn(int pin) {
    digitalWrite(pin, HIGH);
}

void switchOutputOff(int pin) {
    digitalWrite(pin, LOW);
}
