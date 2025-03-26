#include "espnow.h"
#include "SSD1306.h"


uint8_t broadcastAddress[] = {0xE4, 0x65, 0xB8, 0x25, 0x8A, 0xA0}; // base station address
esp_now_peer_info_t peerInfo;

MsgStruct MessageReceived;
MsgStruct MessageToSend;
bool MessageReceivedFlag = false;

QueueHandle_t xQueue;


// Forward declaration for the task
void EspNowSendTask(void *parameter);

void SetupEspNow() {
    Serial.println("Starting SetupEspNow...");

    if (!EspNowInit()) {
        Serial.println("ESP-NOW Init failed");
        return;
    }

    // Create RTOS queue for message handling
    xQueue = xQueueCreate(10, sizeof(MsgStruct));
    if (xQueue == NULL) {
        Serial.println("Failed to create queue");
        return;
    }

    // Create the task for handling ESPNOW sending
    xTaskCreate(
        EspNowSendTask,
        "EspNowSendTask",
        2048,
        NULL,
        1,
        NULL
    );

    Serial.print("MAC Address for peer: ");
    Serial.println(WiFi.macAddress());
}

bool EspNowInit(void) {
    WiFi.disconnect(true);
    delay(100);
    WiFi.mode(WIFI_STA);
    delay(100);

    if (esp_now_init() != ESP_OK) {
        Serial.println("esp_now_init() failed");
        return false;
    }

    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);

    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return false;
    }

    Serial.println("Peer added successfully");
    return true;
}

void EspNowSend(struct MsgStruct *Msg) {
    // Send message to the queue
    if (xQueueSend(xQueue, Msg, pdMS_TO_TICKS(100)) != pdTRUE) {
        Serial.println("Failed to enqueue message");
    }
}

void EspNowSendTask(void *parameter) {
    MsgStruct msg;

    while (true) {
        // Wait for a message to be available in the queue
        if (xQueueReceive(xQueue, &msg, portMAX_DELAY) == pdTRUE) {
            esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&msg, sizeof(msg));
            if (result != ESP_OK) {
                Serial.println("Send error");
            } else {
                printf("Message sent: %i, %i, %s\n", msg.BaseState, msg.Command, msg.Data);
            }
        }
    }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status != ESP_NOW_SEND_SUCCESS) {
        Serial.println("OnDataSent failed");
    } else {
        Serial.println("OnDataSent success");
    }
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
    if (len == sizeof(MessageReceived)) {
        memcpy(&MessageReceived, incomingData, sizeof(MessageReceived));
        MessageReceivedFlag = true;

        printf("Recv (OnDataRecv): %i, %i, %s\n", 
               MessageReceived.BaseState, MessageReceived.Command, MessageReceived.Data);
    }
}
