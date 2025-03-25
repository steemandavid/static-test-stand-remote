#include <Arduino.h>
#include "espnow.h"
#include "SSD1306.h"


uint8_t broadcastAddress[] = {0xE4, 0x65, 0xB8, 0x25, 0x8A, 0xA0}; // base station
//uint8_t broadcastAddress[] = {0x10, 0x97, 0xBD, 0xCC, 0xED, 0xBC}; // remote
esp_now_peer_info_t peerInfo;

//extern MsgStruct MessageReceived;
//extern MsgStruct MessageToSend;
extern bool MessageReceivedFlag = false;


void SetupEspNow() {
  Serial.println("Starting SetupEspNow()...");
  
  EspNowInit();

  Serial.print("MAC Address for peer: ");
  Serial.println(WiFi.macAddress());
}

bool EspNowInit(void) {
  // Ensure WiFi is properly initialized
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
  } else {
      Serial.println("Peer added successfully");
  }

  return true;
}

void EspNowSend(struct MsgStruct *Msg) {
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)Msg, sizeof(*Msg));
    if (result != ESP_OK) {
        Serial.println("Send error");
    } else {
        printf("Message sent: %i, %i, %s\n", Msg->BaseState, Msg->Command, Msg->Data);
    }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status != ESP_NOW_SEND_SUCCESS) {
        Serial.println("OnDataSent failed");
    }
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  DisplayData.RSSI = info->rx_ctrl->rssi;   // Extract RSSI directly
  memcpy(&MessageReceived, incomingData, sizeof(MessageReceived));
  MessageReceivedFlag = true;   // signal that a new message is available
  printf("Recv (OnDataRecv): %i, %i, %s\n", 
          MessageReceived.BaseState, MessageReceived.Command, MessageReceived.Data);
}
