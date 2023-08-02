#include <ArduinoJson.h>
#include "FS.h"
#include "FFat.h"

String loadConfig() {
  File configFile = FFat.open(F("/config.json"), "r");
  if (!configFile) {
    return "[]";
  }
  
  size_t size = configFile.size();
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
  String calibrate_temp,
  String temp_delta
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
  doc["temp_delta"] = temp_delta;
  doc["www_username"] = www_username;
  doc["www_password"] = www_password;

  File configFile = FFat.open(F("/config.json"), "w");
  if (!configFile) {
    return "<h3>Якась помилка</h3>";
  }
  serializeJson(doc, configFile);
  return "<h3>Конфігурацію збережено</h3>";
}

void configSetup() {
  if (!FFat.begin(true)) {
    return;
  }
}
