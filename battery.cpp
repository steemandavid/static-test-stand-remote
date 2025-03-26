#define THISFILENAME "battery.cpp"
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include <esp_log.h>
#include "battery.h"
#include "RTOS.h"


#define VOLT_BAT_PIN 33
#define VOLTAGE_DIVIDER_RATIO (2.4543/1)  // Adjust based on your resistor values (27k , 100k)
#define ADC_MAX_VALUE 4095.0
#define ADC_REF_VOLTAGE 3.3
#define NUM_SAMPLES 20
#define SAMPLE_RATE 500 // take one sample every x ms

float BatteryVoltage = 0.0;
float samples[NUM_SAMPLES];
int sampleIndex = 0;
float sampleSum = 0;


void batteryTask(void *pvParameters) {
    pinMode(VOLT_BAT_PIN, INPUT);

    // Seed the filter with actual readings
    float initialVoltage = (analogRead(VOLT_BAT_PIN) / ADC_MAX_VALUE) * ADC_REF_VOLTAGE * VOLTAGE_DIVIDER_RATIO;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        samples[i] = initialVoltage;
        sampleSum += initialVoltage;
    }

    BatteryVoltage = initialVoltage;

    while (1) {
        // Read ADC value and convert to voltage
        int adcValue = analogRead(VOLT_BAT_PIN);
        float voltage = (adcValue / ADC_MAX_VALUE) * ADC_REF_VOLTAGE * VOLTAGE_DIVIDER_RATIO;

        // Update moving average filter
        sampleSum -= samples[sampleIndex];
        samples[sampleIndex] = voltage;
        sampleSum += voltage;
        sampleIndex = (sampleIndex + 1) % NUM_SAMPLES;

        // Compute filtered value
        BatteryVoltage = sampleSum / NUM_SAMPLES;

        // Wait for next sample period
        vTaskDelay(pdMS_TO_TICKS(SAMPLE_RATE));
    }
}

void SetupBatteryMonitor() {
    xTaskCreatePinnedToCore(
        batteryTask,
        "Battery Monitor",
        2048,                // Stack size
        NULL,                // Parameters
        BATTERYTASKPRIORITY, // Priority
        NULL,                // Task handle
        BATTERYTASKCORE      // Core ID
    );
}