#include <ArduinoJson.h>
#include "FS.h"
#include <LittleFS.h>

String loadConfig() {
  File configFile = LittleFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open wifi config file");
    return "[]";
  }
  
  size_t size = configFile.size();
  if (size > 1024) {Serial.println(" wifi config file size is too large");}
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  return buf.get();
}

String saveConfig(
  String ssid,
  String password,
  String ip,
  String gateway,
  String subnet,
  String set_temp,
  String www_username,
  String www_password,
  String calibrate_temp
) {
  const size_t capacity = 1024;
  DynamicJsonDocument doc(capacity);

  doc["ssid"] = ssid;
  doc["password"] = password;
  doc["ip"] = ip;
  doc["gateway"] = gateway;
  doc["subnet"] = subnet;
  doc["set_temp"] = set_temp;
  doc["calibrate_temp"] = calibrate_temp;
  doc["www_username"] = www_username;
  doc["www_password"] = www_password;

  File configFile = LittleFS.open(F("/config.json"), "w");
  if (!configFile) {
    return "<h3>Якась помилка</h3>";
  }
  serializeJson(doc, configFile);
  return "<h3>Конфігурацію збережено</h3>";
}

void configSetup() {
  Serial.println("Mounting FS...");

  if (!LittleFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
}
