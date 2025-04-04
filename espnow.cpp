#define THISFILENAME "espnow.cpp"
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include <esp_log.h>
#include "espnow.h"
#include "SSD1306.h"

static const char *TAG = THISFILENAME;

uint8_t broadcastAddress[] = {0xE4, 0x65, 0xB8, 0x25, 0x8A, 0xA0}; // base station address
esp_now_peer_info_t peerInfo;

MsgStruct MessageReceived;
MsgStruct MessageToSend;
bool MessageReceivedFlag = false;

// RTOS queues for sending and receiving messages
QueueHandle_t xSendQueue;
QueueHandle_t xRecvQueue;

void SetupEspNow() {
    ESP_LOGI(TAG, "Starting SetupEspNow...");

    if (!EspNowInit()) {
        ESP_LOGE(TAG, "ESP-NOW Init failed");
        return;
    }

    // Create RTOS queues
    xSendQueue = xQueueCreate(10, sizeof(MsgStruct));
    xRecvQueue = xQueueCreate(10, sizeof(MsgStruct));
    
    if (xSendQueue == NULL || xRecvQueue == NULL) {
        ESP_LOGE(TAG, "Failed to create queues");
        return;
    }

    // Create the task for sending ESPNOW messages
    xTaskCreate(
        EspNowSendTask,
        "EspNowSendTask",
        2048,
        NULL,
        1,
        NULL
    );

    // Create the task for receiving ESPNOW messages
    xTaskCreate(
        EspNowRecvTask,
        "EspNowRecvTask",
        2048,
        NULL,
        1,
        NULL
    );

    ESP_LOGI(TAG, "MAC Address for peer: %s", WiFi.macAddress().c_str());
}

bool EspNowInit(void) {
    WiFi.disconnect(true);
    delay(100);
    WiFi.mode(WIFI_STA);
    delay(100);

    if (esp_now_init() != ESP_OK) {
        ESP_LOGE(TAG, "esp_now_init() failed");
        return false;
    }

    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);

    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add peer");
        return false;
    }

    ESP_LOGI(TAG, "Peer added successfully");
    return true;
}

void EspNowSend(struct MsgStruct *Msg) {
    // Send message to the queue
    if (xQueueSend(xSendQueue, Msg, pdMS_TO_TICKS(100)) != pdTRUE) {
        ESP_LOGW(TAG, "Failed to enqueue message for sending");
    }
}

void EspNowSendTask(void *parameter) {
    MsgStruct msg;

    while (true) {
        // Wait for a message to be available in the queue
        if (xQueueReceive(xSendQueue, &msg, portMAX_DELAY) == pdTRUE) {
            esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&msg, sizeof(msg));
            if (result != ESP_OK) {
                ESP_LOGE(TAG, "Send error");
            } else {
                ESP_LOGI(TAG, "Message sent: %i, %i, %s", msg.BaseState, msg.Command, msg.Data);
            }
        }
    }
}

void EspNowRecvTask(void *parameter) {
    MsgStruct msg;

    while (true) {
        // Wait for a received message to be available in the queue
        if (xQueueReceive(xRecvQueue, &msg, portMAX_DELAY) == pdTRUE) {
            // Handle the received message
            MessageReceived = msg;
            MessageReceivedFlag = true;

            ESP_LOGI(TAG, "Message received: %i, %i, %s", msg.BaseState, msg.Command, msg.Data);
        }
    }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status != ESP_NOW_SEND_SUCCESS) {
        ESP_LOGE(TAG, "OnDataSent failed");
    } else {
        ESP_LOGI(TAG, "OnDataSent success");
    }
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
    if (len == sizeof(MsgStruct)) {
        MsgStruct msg;
        memcpy(&msg, incomingData, sizeof(MsgStruct));
        
        // Add received message to the receive queue
        if (xQueueSend(xRecvQueue, &msg, pdMS_TO_TICKS(100)) != pdTRUE) {
            ESP_LOGW(TAG, "Failed to enqueue received message");
        }
    }
}
