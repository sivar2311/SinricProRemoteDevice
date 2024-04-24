#include <Arduino.h>
#include <SinricProRemoteDevice.h>
#include <WiFi.h>

const char* WIFI_SSID = "";
const char* WIFI_PASS = "";

const char* apiKey   = "";
const char* deviceId = "";

bool state = false;

SinricProRemoteDevice remoteDevice;

void setupWiFi() {
    Serial.print("Connecting Wifi");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println("connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void setupRemoteDevice() {
    remoteDevice.begin(apiKey, deviceId);
}

void toggleRemoteDevice() {
    Serial.printf("Toggle device %s\r\n", state ? "on" : "off");
    remoteDevice.setPowerState(state);
    state = !state;
}

void setup() {
    Serial.begin(115200);
    setupWiFi();
    setupRemoteDevice();
}

void loop() {
    toggleRemoteDevice();
    delay(10000);
}