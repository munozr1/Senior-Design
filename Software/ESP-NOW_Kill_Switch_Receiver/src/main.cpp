/*
  Bit Bangers ESP-NOW "Kill Switch" Receiver Code
  github.com/munozr1/Senior-Design

  Original Code:
  
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

/*
 ----------------------------------- Libraries ---------------------------------
*/
#include <esp_now.h>
#include <WiFi.h>

/*
------------------------------------- Defines ----------------------------------
*/

/*
------------------------------------ Structures --------------------------------
*/
typedef struct struct_message {
    char a[32];
} struct_message;

/*
--------------------------------  Global Variables -----------------------------
*/
bool toggleLED = false;
struct_message myData;

/*
------------------------------------ Functions ---------------------------------
*/
// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  if (!strcmp(myData.a, "kill")) {
    // This code simply turns on the builtin LED
    // Todo: Power off all peripherals and ESP32 here
    if (toggleLED) {
      digitalWrite(LED_BUILTIN, LOW);
      toggleLED = false;
    }
    else {
      digitalWrite(LED_BUILTIN, HIGH);
      toggleLED = true;

    }
  }
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}