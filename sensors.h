const int analogInPin = 2; // пін до якого підключано NTC
const float supplyVoltage = 3.3; // Напруга живлення ESP32
const float R_divider = 100000.0; // Опір резистора в дільнику напруги (100K)
const float R_ntc_at_25C = 100000.0; // Опір NTC при 25 °C (100K)
const float B = 3950.0; // Коефіцієнт Бета


float numLimit(float num,int lim){
  float resp = round((num)*pow(10,lim))/pow(10,lim);
  return resp;
}

String getData(float calibrate_temp){
  int rawValue = analogRead(analogInPin);
  float voltage = rawValue * supplyVoltage / 4095.0; // Перетворення в напругу (12 бітне розширення)

  // Розраховуємо опір NTC на основі відомого опору і напруги
  float R_ntc = R_divider * (supplyVoltage / voltage - 1.0);

  // розраховуємо температуру за допомогою рівняння Стейнхарта-Харт:
  // T = 1 / (1 / (T0 + 273.15) + (1 / B) * ln(Rntc / R25))
  float T0 = 25.0; // Опорна температура (зазвичай 25 °C)
  float temperature = 1.0 / (1.0 / (T0 + 273.15) + (1.0 / B) * log(R_ntc / R_ntc_at_25C)) - 273.15;
  
  StaticJsonDocument<256> doc;
  JsonObject object = doc.to<JsonObject>();

  object["temperature"] = numLimit(temperature + calibrate_temp,2);

  String output;
  serializeJson(object, output);
  return output;
}

void sensorsSetup() {
  analogReadResolution(12);
  pinMode(analogInPin, INPUT);
}