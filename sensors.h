const int analogInPin = 2; // Пин, к которому подключен делитель напряжения с NTC
// Задаємо значення резистора, який підключений паралельно до датчика (в Омах)
const float R1 = 100000.0; // Резистор NTC з опором 10 кОм при 25°C

// Значення, яке повертає АЦП при підключенні максимального напруги (5V)
const int ADC_MAX_VALUE = 4095;

//https://github.com/MarlinFirmware/Marlin/blob/bugfix-2.1.x/Marlin/src/module/thermistor/thermistors.h

// R25 = 100 kOhm, beta25 = 3950 K, 10 kOhm pull-up, NTCS0603E3104FHT

const int temperatureTable[][2] = {
    {704, 1},
    {216, 54},
    {175, 107},
    {152, 160},
    {137, 213},
    {125, 266},
    {115, 319},
    {106, 372},
    {99, 425},
    {91, 478},
    {85, 531},
    {78, 584},
    {71, 637},
    {65, 690},
    {58, 743},
    {50, 796},
    {42, 849},
    {31, 902},
    {17, 955},
    {0, 1008}
};
// R25 = 100 kOhm, beta25 = 3950 K, 4.7 kOhm pull-up, QU-BD silicone bed QWG-104F-3950 thermistor
/*
const int temperatureTable[][2] = {
    {938, 1},
    {314, 31},
    {290, 41},
    {272, 51},
    {258, 61},
    {247, 71},
    {237, 81},
    {229, 91},
    {221, 101},
    {215, 111},
    {209, 121},
    {204, 131},
    {199, 141},
    {195, 151},
    {190, 161},
    {187, 171},
    {183, 181},
    {179, 191},
    {176, 201},
    {170, 221},
    {165, 241},
    {160, 261},
    {155, 281},
    {150, 301},
    {144, 331},
    {139, 361},
    {133, 391},
    {128, 421},
    {123, 451},
    {117, 491},
    {111, 531},
    {105, 571},
    {100, 611},
    {95, 641},
    {90, 681},
    {85, 711},
    {79, 751},
    {72, 791},
    {69, 811},
    {65, 831},
    {57, 871},
    {55, 881},
    {51, 901},
    {45, 921},
    {39, 941},
    {28, 971},
    {23, 981},
    {17, 991},
    {9, 1001},
    {-27, 1021}
};
*/
// R25 = 100 kOhm, beta25 = 3950 K, 4.7 kOhm pull-up,
// Maker's Tool Works Kapton Bed Thermistor
// ./createTemperatureLookup.py --r0=100000 --t0=25 --r1=0 --r2=4700 --beta=3950
// r0: 100000
// t0: 25
// r1: 0 (parallel with rTherm)
// r2: 4700 (series with rTherm)
// beta: 3950
// min adc: 1 at 0.0048828125 V
// max adc: 1023 at 4.9951171875 V
/*
const int temperatureTable[][2] = {
    {272, 51},
    {258, 61},
    {247, 71},
    {237, 81},
    {229, 91},
    {221, 101},
    {204, 131},
    {190, 161},
    {179, 191},
    {167, 231},
    {157, 271},
    {148, 311},
    {140, 351},
    {135, 381},
    {130, 411},
    {125, 441},
    {123, 451},
    {122, 461},
    {120, 471},
    {119, 481},
    {117, 491},
    {116, 501},
    {114, 511},
    {113, 521},
    {111, 531},
    {110, 541},
    {108, 551},
    {107, 561},
    {105, 571},
    {104, 581},
    {102, 591},
    {101, 601},
    {100, 611},
    {98, 621},
    {97, 631},
    {95, 641},
    {94, 651},
    {92, 661},
    {91, 671},
    {90, 681},
    {88, 691},
    {87, 701},
    {85, 711},
    {84, 721},
    {82, 731},
    {81, 741},
    {79, 751},
    {77, 761},
    {76, 771},
    {74, 781},
    {72, 791},
    {71, 801},
    {69, 811},
    {67, 821},
    {65, 831},
    {63, 841},
    {62, 851},
    {60, 861},
    {57, 871},
    {55, 881},
    {53, 891},
    {51, 901},
    {48, 911},
    {45, 921},
    {42, 931},
    {39, 941},
    {36, 951},
    {32, 961},
    {23, 981},
    {17, 991},
    {9, 1001},
    {0, 1008}
};
*/
// R25 = 100 kOhm, beta25 = 3950 K, 4.7 kOhm pull-up,
// Formbot / Vivedino high temp 100k thermistor
// 100KR13950181203
// Generated with modified version of https://www.thingiverse.com/thing:103668
// Using table 1 with datasheet values
// Resistance         100k Ohms at 25deg. C
// Resistance Tolerance     + / -1%
// B Value             3950K at 25/50 deg. C
// B Value Tolerance         + / - 1%
/*
const int temperatureTable[][2] = {
    {420, 2.00}, // Guestimate to ensure we don't lose a reading and drop temps to -50 when over
    {350, 12.07},
    {345, 12.79},
    {340, 13.59},
    {335, 14.44},
    {330, 15.37},
    {325, 16.38},
    {320, 17.46},
    {315, 18.63},
    {310, 19.91},
    {305, 21.29},
    {300, 22.79},
    {295, 24.43},
    {290, 26.21},
    {285, 28.15},
    {280, 30.27},
    {275, 32.58},
    {270, 35.10},
    {265, 38.44},
    {260, 40.89},
    {255, 44.19},
    {250, 47.83},
    {245, 51.80},
    {240, 56.20},
    {235, 61.00},
    {230, 66.30},
    {225, 72.11},
    {220, 78.51},
    {215, 85.57},
    {210, 93.34},
    {205, 101.91},
    {200, 111.34},
    {195, 121.73},
    {190, 133.17},
    {185, 145.74},
    {180, 159.57},
    {175, 174.73},
    {170, 191.35},
    {165, 209.53},
    {160, 229.35},
    {155, 250.90},
    {150, 274.25},
    {145, 299.46},
    {140, 326.52},
    {135, 355.44},
    {130, 386.15},
    {125, 418.53},
    {120, 452.43},
    {115, 487.62},
    {110, 523.82},
    {105, 560.70},
    {100, 597.88},
    {95, 634.97},
    {90, 671.55},
    {85, 707.21},
    {80, 741.54},
    {75, 779.65},
    {70, 809.57},
    {65, 833.40},
    {60, 859.55},
    {55, 883.27},
    {50, 904.53},
    {45, 923.38},
    {40, 939.91},
    {35, 954.26},
    {30, 966.59},
    {25, 977.08},
    {20, 985.92},
    {15, 993.39},
    {10, 999.42},
    {5, 1004.43},
    {0, 1008.51},
    {-5, 1011.79},
    {-10, 1014.40},
    {-15, 1016.48},
    {-20, 1018.10},
    {-25, 1019.35},
    {-30, 1020.32},
    {-35, 1021.05},
    {-40, 1021.60},
    {-45, 1022.01},
    {-50, 1022.31}
};
*/

// Функція для обчислення температури в градусах Цельсія
float getTemperature(int resistance)
{
  int size = sizeof(temperatureTable) / sizeof(temperatureTable[0]);

  // Перевіряємо діапазон опору NTC в таблиці
  if (resistance < temperatureTable[0][0])
  {
    return temperatureTable[0][1];
  }
  if (resistance >= temperatureTable[size - 1][0])
  {
    return temperatureTable[size - 1][1];
  }

  // Інтерполяція значення температури для заданого опору NTC
  for (int i = 1; i < size; i++)
  {
    if (resistance < temperatureTable[i][0])
    {
      float resistance1 = temperatureTable[i - 1][0];
      float resistance2 = temperatureTable[i][0];
      float temperature1 = temperatureTable[i - 1][1];
      float temperature2 = temperatureTable[i][1];
      return temperature1 + (temperature2 - temperature1) * (resistance - resistance1) / (resistance2 - resistance1);
    }
  }
}

float numLimit(float num,int lim){
  float resp = round((num)*pow(10,lim))/pow(10,lim);
  return resp;
}

String getData(float calibrate_temp){
  int adcValue = analogRead(analogInPin);                        // Зчитуємо значення АЦП
  float resistance = R1 * (ADC_MAX_VALUE / (float)adcValue - 1); // Обчислюємо опір NTC
  float temperature = getTemperature(resistance);                // Отримуємо температуру в градусах Цельсія

  StaticJsonDocument<256> doc;
  JsonObject object = doc.to<JsonObject>();

  object["temperature"] = numLimit(temperature + calibrate_temp,2);

  String output;
  serializeJson(object, output);
  return output;
}

void sensorsSetup() {
  pinMode(analogInPin, INPUT);
}
