#include <Arduino.h>
#include "espnow.h"

uint8_t broadcastAddress[] = {0xE4, 0x65, 0xB8, 0x25, 0x8A, 0xA0}; // base station
//uint8_t broadcastAddress[] = {0x10, 0x97, 0xBD, 0xCC, 0xED, 0xBC}; // remote
esp_now_peer_info_t peerInfo;

extern MsgStruct MessageReceived;
extern MsgStruct MessageToSend;


void SetupEspNow() {
  // ESPNOW ///////////////////////////////////////
  Serial.println("Starting SetupEspNow()...");
  Serial.print("Broadcast Address of peer: ");
  for (int i = 0; i < 6; i++) {
      Serial.printf("%02X", broadcastAddress[i]);
      if (i < 5) Serial.print(":");
  }
  Serial.println("");

  if (!EspNowInit()) {
      Serial.println("EspNow init failed");
      return;
  }
}


bool EspNowInit(void) {
    WiFi.mode(WIFI_STA);

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
    }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status != ESP_NOW_SEND_SUCCESS) {
        Serial.println("OnDataSent failed");
    }
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&MessageReceived, incomingData, sizeof(MessageReceived));
  printf("Recv (OnDataRecv): %i %i %i %i %i %i --\n", 
          MessageReceived.BaseState, MessageReceived.VbatRemote, 
          MessageReceived.Button_Button, MessageReceived.Switch_Arm,
          MessageReceived.Button_LED, MessageReceived.Buzzer);
}
