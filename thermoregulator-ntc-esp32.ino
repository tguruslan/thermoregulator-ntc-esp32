#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "ota.h"
#include "sensors.h"
#include "config.h"

#ifndef APSSID
#define APSSID "TempSensor"
#define APPSK  "TempSensor"
#endif

const char* www_username;
const char* www_password;

const char *ap_ssid = APSSID;
const char *ap_password = APPSK;

long lastUpdateTime = 0, previousMillis = 0, interval = 1000;
int open_settings = 1, run_app=0;
float temp_delta = 1; // якщо температура відрізняється від заданої на данне число виконати дію

const size_t capacity = 1024;
DynamicJsonDocument config_settings(capacity);
StaticJsonDocument<256> temp_data;
String json_data;

IPAddress ip_fin, gateway_fin, subnet_fin; 

WebServer server(80);

String getHeader(String title){
  String html = "<!DOCTYPE html>";
  html += "<header>";
  html += "<meta charset='utf-8'>";
  html += "<title>"+title+"</title>";
  html += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/css/materialize.min.css'>";
  html += "<script src='https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/js/materialize.min.js'></script>";
  html += "</header>";
  html += "<body>";
  html += "<div class='container'>";
  html += "<center><h1>"+title+"</h1></center>";
  return html;
}


String getFooter(){
  String html = "</div>";
  html += "</body>";
  html += "</html>";
  return html;
}

String getTitle(String name){
  StaticJsonDocument<512> doc;
  JsonObject object = doc.to<JsonObject>();

  object["ssid"]="Назва мережі";
  object["password"]="Пароль";
  object["ip"]="IP адреса";
  object["gateway"]="Основний шлюз";
  object["subnet"]="Маска підмережі";
  object["set_temp"]="Задана температура";
  object["temp_delta"]="Різниця температур для дій";
  object["calibrate_temp"]="Калібрування температури";
  object["www_username"]="Логін для веб налаштувань";
  object["www_password"]="Пароль для веб налаштувань";
  object["temperature"]="Температура";
  return object[name];
}

String getUnit(String name){
  StaticJsonDocument<128> doc;
  JsonObject object = doc.to<JsonObject>();
  object["temperature"]="°C";
  return object[name];
}

void handleNotFound() {
  String content = "Not Found\n\n";
  content += "URI: ";
  content += server.uri();
  content += "\nMethod: ";
  content += (server.method() == HTTP_GET) ? "GET" : "POST";
  content += "\nArguments: ";
  content += server.args();
  content += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    content += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", content);
}

void String_to_IP(String str,IPAddress& IP_addr)
{
  int c1 = str.indexOf('.');
  int c2 = str.indexOf('.',c1+1);
  int c3 = str.indexOf('.',c2+1);
  int ln = str.length();
  IP_addr[0] = str.substring(0,c1).toInt();
  IP_addr[1] = str.substring(c1+1,c2).toInt();
  IP_addr[2] = str.substring(c2+1,c3).toInt();
  IP_addr[3] = str.substring(c3+1,ln).toInt();
}

String genInput(String name, String val){
  String out;
         out += "<div class='input-field col s6'>";
         out += "<label for='"+name+"'>"+getTitle(name)+"</label>";
         out += "<input type='text' name='"+name+"' value='"+val+"' /><br>";
         out += "</div>";
  return out;
}

void setup(void) {
  Serial.begin(115200);
  configSetup();
  sensorsSetup();

  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);

  if (open_settings == 1) {
    deserializeJson(config_settings, loadConfig());
    open_settings = 0;
  }

  if (config_settings["ssid"].as<String>() != "null" && config_settings["ssid"].as<String>() != "") {
    WiFi.mode(WIFI_STA);
    WiFi.begin(config_settings["ssid"].as<String>(), config_settings["password"].as<String>());

    if (config_settings["ip"].as<String>() != "null" && config_settings["ip"].as<String>() != "") {
      String_to_IP(config_settings["ip"].as<String>(), ip_fin);
      String_to_IP(config_settings["gateway"].as<String>(), gateway_fin);
      String_to_IP(config_settings["subnet"].as<String>(), subnet_fin);

      WiFi.config(ip_fin, gateway_fin, subnet_fin);
    }


    int boot_try_count = 0;
    while (WiFi.status() != WL_CONNECTED && run_app == 0) {
      delay(500);
      boot_try_count++;
      if (boot_try_count > 20) {
        run_app = 1;
      }
    }

  }else{
    run_app = 1;
  }
  if (run_app == 1) {
    WiFi.softAP(ap_ssid, ap_password);
  }
  otaSetup();
  www_username = config_settings["www_username"];
  www_password = config_settings["www_password"];

  server.on("/", []() {
    json_data = getData(config_settings["calibrate_temp"].as<float>());
    server.send(200, "application/json", json_data);
  });

  server.on("/preview", []() {
    json_data = getData(config_settings["calibrate_temp"].as<float>());
    deserializeJson(temp_data, json_data);
    JsonObject root = temp_data.as<JsonObject>();
    String content;
    content += getHeader("Данні датчиків");
    content += "<table class='responsive-table'><thead>";
    content += "<tr><td>Величина</td><td>Значення</td><td>Одиниці виміру</td></tr>";
    content += "</thead><tbody>";

    for (JsonPair kv : root) {
        content += "<tr>";
        content += "<td>"+getTitle(kv.key().c_str())+"</td>";
        content += "<td>"+kv.value().as<String>()+"</td>";
        content += "<td>"+getUnit(kv.key().c_str())+"</td>";
        content += "</tr>";
    }
    content += "</table></tbody>";
    content += getFooter();

    server.send(200, "text/html", content);
  });

  server.on("/config", []() {
    if (config_settings["www_password"].as<String>() != "null" && config_settings["www_password"].as<String>() != "")
    {
      if (!server.authenticate(www_username, www_password)) {return server.requestAuthentication();}
    }
    String content;
    content += getHeader("Налаштування");

    if (server.arg("ssid") != "") {
      content += saveConfig(
        server.arg("ssid"),
        server.arg("password"),
        server.arg("ip"),
        server.arg("gateway"),
        server.arg("subnet"),
        server.arg("set_temp"),
        server.arg("www_username"),
        server.arg("www_password"),
        server.arg("calibrate_temp"),
        server.arg("temp_delta")
      );
      open_settings = 1;
    }


    content += "<form>";
    deserializeJson(config_settings, loadConfig());
    content += "<div class='row'>";
    content += "<h3>Налаштування мережі</h3>";
    for(String param : {
      "ssid",
      "password",
      "ip",
      "gateway",
      "subnet"
    })
    {
        content += genInput(param,config_settings[param]);
    }
    content += "</div>";
    content += "<div class='row'>";
    content += "<h3>Інші налаштування</h3>";
    for(String param : {
      "www_username",
      "www_password",
      "set_temp",
      "calibrate_temp",
      "temp_delta"
    })
    {
        content += genInput(param,config_settings[param]);
    }
    content += "</div>";
    content += "<div class='row'>";
    content += "<button class='btn waves-effect waves-light' type='submit'>Зберегти</button>";
    content += "</div>";
    content += "</form>";
    content += getFooter();

    server.send(200, "text/html", content);
  });

  server.onNotFound(handleNotFound);

  server.begin();
}

void loop(void) {
  ArduinoOTA.handle();
  if (open_settings == 1) {
    deserializeJson(config_settings, loadConfig());
    open_settings = 0;
  }

  if (lastUpdateTime == 0 || (millis() - lastUpdateTime > interval))
  {
    lastUpdateTime = millis();

    if (WiFi.status() != WL_CONNECTED && run_app == 0){
      ESP.restart();
    }

    json_data = getData(config_settings["calibrate_temp"].as<float>());
    deserializeJson(temp_data, json_data);

 
    if (config_settings["temp_delta"].as<String>() != "null" && config_settings["temp_delta"].as<String>() != ""){
      temp_delta=config_settings["temp_delta"].as<float>();
    }

    if(temp_data["temperature"].as<float>() < (config_settings["set_temp"].as<float>() - temp_delta)){
      digitalWrite(3, HIGH);
    }
    if (temp_data["temperature"].as<float>() > (config_settings["set_temp"].as<float>() + temp_delta)) {
      digitalWrite(3, LOW);
    }
  }
  server.handleClient();
}