#ifndef ESPNOW_H
#define ESPNOW_H

#include <esp_now.h>
#include <WiFi.h>

typedef struct __attribute__((packed)) MsgStruct {  // "__attribute__((packed))" ensure that the compiler does not add padding
    uint8_t BaseState;
    uint16_t TxRxFails;
    uint8_t VbatRemote;
    bool Button_Button;
    bool Switch_Arm;
    bool Button_LED;
    bool Buzzer;
    char DisplayLine[10];
} MsgStruct;

extern uint8_t broadcastAddress[6]; // Fix: Make broadcastAddress available globally

bool EspNowInit(void);
void EspNowSend(struct MsgStruct *Msg);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len);
void SetupEspNow();

#endif
