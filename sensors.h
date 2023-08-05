#include "max6675.h"

int thermoMISO = 37;
int thermoSC = 34;
int thermoCLK = 36;

MAX6675 thermocouple(thermoCLK, thermoSC, thermoMISO);

float numLimit(float num,int lim){
  float resp = round((num)*pow(10,lim))/pow(10,lim);
  return resp;
}

String getData(float calibrate_temp){
  StaticJsonDocument<256> doc;
  JsonObject object = doc.to<JsonObject>();

  object["temperature"] = numLimit(thermocouple.readCelsius() + calibrate_temp,2);

  String output;
  serializeJson(object, output);
  return output;
}

void sensorsSetup() {
}
