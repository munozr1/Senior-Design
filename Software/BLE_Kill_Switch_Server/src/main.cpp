#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

//BLE server name
#define bleServerName "Bit-Bangers-Robot"

bool deviceConnected = false;

bool ledState = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "88a89505-51d9-41ee-848f-b119b850e548"

void writeBlueLED(bool enabled) {
    if (enabled) {
        neopixelWrite(RGB_BUILTIN,0,0,1);
    }
    else {
        neopixelWrite(RGB_BUILTIN,0,0,0);How to send and receive data with ESP32 over BLE?
Stack Overflow
https://stackoverflow.com › questions › how-to-send-a...
Nov 19, 2019 — Define the function and the logic to se
    }
}

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        writeBlueLED(true);
        deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

void setup() {
    Serial.begin(115200); // set baud rate

    BLEDevice::init(bleServerName); // create BLE device

    // create BLE server
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *bmeService = pServer->createService(SERVICE_UUID); // create BLE service

    // begin advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pServer->getAdvertising()->start();
    Serial.println("BLE start advertising...");
}

void loop() {
    if (deviceConnected) {
        // Logic to enable builtin LED 
    }
    else { // toggles blue LED while searching for client
    ledState = !ledState;
    writeBlueLED(ledState);
    Serial.println("Waiting for a client connection to notify...");
    delay(1000);
    }
}