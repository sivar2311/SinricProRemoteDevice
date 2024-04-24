#include "SinricProRemoteDevice.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

void SinricProRemoteDevice::begin(const char* apiKey, const char* deviceId) {
    this->apiKey   = apiKey;
    this->deviceId = deviceId;
}

int SinricProRemoteDevice::setPowerState(bool state) const {
    return sendText("setPowerState", "state", state ? "On" : "Off");
}

int SinricProRemoteDevice::setPowerLevel(int level) const {
    return sendNumber("setPowerLevel", "powerLevel", level);
}

int SinricProRemoteDevice::adjustPowerLevel(int levelDelta) const {
    return sendNumber("adjustPowerLevel", "powerLevelDelta", levelDelta);
}

int SinricProRemoteDevice::setBrightness(int brightness) const {
    return sendNumber("setBrightness", "brightness", brightness);
}

int SinricProRemoteDevice::adjustBrightness(int brightnessDelta) const {
    return sendNumber("adjustBrightness", "brightnessDelta", brightnessDelta);
}

int SinricProRemoteDevice::setColor(uint8_t r, uint8_t g, uint8_t b) const {
    auto request = createRequest("setColor");
    auto value   = getRequestValue(request);
    auto color   = value["color"].to<JsonObject>();
    color["r"]   = r;
    color["g"]   = g;
    color["b"]   = b;
    return sendRequest(request);
}

int SinricProRemoteDevice::setColorTemperature(int colorTemperature) const {
    return sendNumber("setColorTemperature", "colorTemperature", colorTemperature);
}

int SinricProRemoteDevice::increaseColorTemperature() const {
    auto request = createRequest("increaseColorTemperature");
    return sendRequest(request);
}

int SinricProRemoteDevice::decreaseColorTemperature() const {
    auto request = createRequest("decreaseColorTemperature");
    return sendRequest(request);
}

int SinricProRemoteDevice::setTargetTemperature(int temperature) const {
    return sendNumber("targetTemperature", "temperature", temperature);
}

int SinricProRemoteDevice::adjustTargetTemperature(int temperature) const {
    return sendNumber("adjustTargetTemperature", "temperature", temperature);
}

int SinricProRemoteDevice::setThermostatMode(const String& thermostatMode) const {
    return sendText("setThermostatMode", "thermostatMode", thermostatMode);
}

int SinricProRemoteDevice::setRangeValue(int rangeValue, const String& instanceId) const {
    return sendNumber("setRangeValue", "rangeValue", rangeValue, instanceId);
}

int SinricProRemoteDevice::adjustRangeValue(int rangeValue, const String& instanceId) const {
    return sendNumber("adjustRangeValue", "rangeValue", rangeValue);
}

int SinricProRemoteDevice::setVolume(int volume) const {
    return sendNumber("setVolume", "volume", volume);
}

int SinricProRemoteDevice::adjustVolume(int volume, bool volumeDefault) const {
    auto request           = createRequest("adjustVolume");
    auto value             = getRequestValue(request);
    value["volume"]        = volume;
    value["volumeDefault"] = volumeDefault;
    return sendRequest(request);
}

int SinricProRemoteDevice::selectInput(const String& input) const {
    return sendText("selectInput", "input", input);
}

int SinricProRemoteDevice::mediaControl(const String& mediaControl) const {
    return sendText("mediaControl", "control", mediaControl);
}

int SinricProRemoteDevice::changeChannel(const String& channelName) const {
    auto request    = createRequest("changeChannel");
    auto value      = getRequestValue(request);
    auto channel    = value["channel"].to<JsonObject>();
    channel["name"] = channelName;
    return sendRequest(request);
}

int SinricProRemoteDevice::skipChannels(int channelCount) const {
    return sendNumber("skipChannels", "channelCount", channelCount);
}

int SinricProRemoteDevice::setBands(const String& name, int bandsValue) const {
    auto request   = createRequest("setBands");
    auto value     = getRequestValue(request);
    auto bands     = value["bands"].add<JsonObject>();
    bands["name"]  = name;
    bands["value"] = bandsValue;

    return sendRequest(request);
}

int SinricProRemoteDevice::adjustBands(const String& name, int levelDelta, const String& levelDirection) const {
    auto request            = createRequest("adjustBands");
    auto value              = getRequestValue(request);
    auto bands              = value["bands"].add<JsonObject>();
    bands["levelDelta"]     = levelDelta;
    bands["levelDirection"] = levelDirection;
    bands["name"]           = name;

    return sendRequest(request);
}

int SinricProRemoteDevice::resetBands(const String& name) const {
    auto request = createRequest("resetBands");
    auto value = getRequestValue(request);
    auto bands = value["bands"].add<JsonObject>();
    bands["name"] = name;
    return sendRequest(request);
}

int SinricProRemoteDevice::setMode(const String& mode, const String& instanceId) const {
    return sendText("setMode", "mode", mode, instanceId);
}

int SinricProRemoteDevice::setLockState(bool lock) const {
    return sendText("setLockState", "state", lock ? "lock" : "unlock");
}

int SinricProRemoteDevice::setMute(bool mute) const {
    auto request = createRequest("setMute");
    auto value = getRequestValue(request);
    value["mute"] = mute;
    return sendRequest(request);
}

int SinricProRemoteDevice::setPercentage(int percentage) const {
    return sendNumber("setPercentage", "percentage", percentage);
}

int SinricProRemoteDevice::sendKeystroke(const String& keystroke) const {
    return sendText("sendKeystroke", "keystroke", keystroke);
}

int SinricProRemoteDevice::setToggleState(bool state) const {
    return sendText("setToggleState", "state", state ? "On" : "Off");
}

// protected functions

JsonObject SinricProRemoteDevice::getRequestValue(JsonDocument& doc) const {
    return doc["value"].to<JsonObject>();
}

void SinricProRemoteDevice::setInstanceId(JsonDocument& doc, const String& instanceId) const {
    doc["instanceId"] = instanceId;
}

JsonDocument SinricProRemoteDevice::createRequest(const char* action) const {
    JsonDocument doc;
    doc["api_key"]   = apiKey;
    doc["device_id"] = deviceId;
    doc["action"]    = action;
    return doc;
}

bool SinricProRemoteDevice::credentialsNotSet() const {
    return (apiKey == nullptr) || (deviceId == nullptr);
}

int SinricProRemoteDevice::sendRequest(JsonDocument doc) const {
    if (credentialsNotSet()) return -1;

    HTTPClient       https;
    WiFiClientSecure client;
    client.setInsecure();
    https.begin(client, "https://ifttt.sinric.pro/v1/actions");
    https.addHeader("Content-Type", "application/json");

    String request;
    serializeJson(doc, request);
    Serial.println();
    auto result = https.POST(request);
    return result;
}

int SinricProRemoteDevice::sendNumber(const char* action, const char* varName, int value, const String& instanceId) const {
    auto _request = SinricProRemoteDevice::createRequest(action);
    auto _value   = SinricProRemoteDevice::getRequestValue(_request);
    if (instanceId.length()) setInstanceId(_request, instanceId);
    _value[varName] = value;
    return sendRequest(_request);
}

int SinricProRemoteDevice::sendText(const char* action, const char* varName, const String& value, const String& instanceId) const {
    auto _request = SinricProRemoteDevice::createRequest(action);
    auto _value   = SinricProRemoteDevice::getRequestValue(_request);
    if (instanceId.length()) setInstanceId(_request, instanceId);
    _value[varName] = value;
    return sendRequest(_request);
}