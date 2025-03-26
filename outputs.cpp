#define THISFILENAME "outputs.cpp"
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include <esp_log.h>
#include "outputs.h"
#include "RTOS.h"

static const char *TAG = THISFILENAME;

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
                ESP_LOGI(TAG, "Stopped toggle task for pin %d", pin);
            }
        }
        if (switchPinMapping[i] == pin) {
            if (switchTaskHandle[i] != NULL) {
                vTaskDelete(switchTaskHandle[i]);
                switchTaskHandle[i] = NULL;
                switchPinMapping[i] = -1;
                ESP_LOGI(TAG, "Stopped switch task for pin %d", pin);
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

    ESP_LOGI(TAG, "Toggling pin %d at %.2f Hz", pin, frequency);

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
                    ESP_LOGI(TAG, "Started toggle task for pin %d at %.2f Hz", pin, frequency);
                } else {
                    free(params);
                    ESP_LOGE(TAG, "Failed to create toggle task.");
                }
            } else {
                ESP_LOGE(TAG, "Failed to allocate memory for toggle task.");
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

    ESP_LOGI(TAG, "Switching pin %d ON for %d seconds", pin, seconds);

    digitalWrite(pin, HIGH);
    vTaskDelay(pdMS_TO_TICKS(seconds * 1000));
    digitalWrite(pin, LOW);

    ESP_LOGI(TAG, "Switching pin %d OFF", pin);

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
                    ESP_LOGI(TAG, "Started switch task for pin %d for %d seconds", pin, seconds);
                } else {
                    free(params);
                    ESP_LOGE(TAG, "Failed to create switch task.");
                }
            } else {
                ESP_LOGE(TAG, "Failed to allocate memory for switch task.");
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
