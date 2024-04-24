#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

class SinricProRemoteDevice {
  public:
    void begin(const char* apiKey, const char* deviceId);

    int setPowerState(bool state) const;
    int setPowerLevel(int level) const;
    int adjustPowerLevel(int levelDelta) const;
    int setBrightness(int brightness) const;
    int adjustBrightness(int brightnessDelta) const;
    int setColor(uint8_t r, uint8_t g, uint8_t b) const;
    int setColorTemperature(int colorTemperature) const;
    int increaseColorTemperature() const;
    int decreaseColorTemperature() const;
    int setTargetTemperature(int temperature) const;
    int adjustTargetTemperature(int temperature) const;
    int setThermostatMode(const String& thermostatMode) const;
    int setRangeValue(int rangeValue, const String& instanceId = "") const;
    int adjustRangeValue(int rangeValue, const String& instanceId = "") const;
    int setVolume(int volume) const;
    int adjustVolume(int volume, bool volumeDefault = false) const;
    int selectInput(const String& input) const;
    int mediaControl(const String& mediaControl) const;
    int changeChannel(const String& channelName) const;
    int skipChannels(int channelCount) const;
    int setBands(const String& name, int bandsValue) const;
    int adjustBands(const String& name, int levelDelta, const String& levelDirection) const;
    int resetBands(const String& name) const;
    int setMode(const String& mode, const String& instanceId = "") const;
    int setLockState(bool lock) const;
    int setMute(bool mute) const;
    int setPercentage(int percentage) const;
    int sendKeystroke(const String& keystroke) const;
    int setToggleState(bool state) const;

  protected:
    const char* apiKey   = nullptr;
    const char* deviceId = nullptr;

  protected:
    bool         credentialsNotSet() const;
    JsonDocument createRequest(const char* action) const;
    JsonObject   getRequestValue(JsonDocument& doc) const;
    void         setInstanceId(JsonDocument& doc, const String& instanceId) const;
    int          sendRequest(JsonDocument doc) const;
    int          sendNumber(const char* action, const char* varName, int value, const String& instanceId = "") const;
    int          sendText(const char* action, const char* varName, const String& value, const String& instanceId = "") const;
};