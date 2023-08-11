#define CLK 39
#define DT 40
int currentStateCLK;
int lastStateCLK;

DynamicJsonDocument cnf_settings(1024);

int encoderData(){
    int open_settings = 0;
    currentStateCLK = digitalRead(CLK);
    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {
        deserializeJson(cnf_settings, loadConfig());
        float set_temp = cnf_settings["set_temp"].as<float>();

        if (digitalRead(DT) != currentStateCLK) {
            set_temp ++;
        } else {
            set_temp --;
        }
        
        saveConfig(
            cnf_settings["ssid"].as<String>(),
            cnf_settings["password"].as<String>(),
            cnf_settings["ip"].as<String>(),
            cnf_settings["gateway"].as<String>(),
            cnf_settings["subnet"].as<String>(),
            String(set_temp),
            cnf_settings["www_username"].as<String>(),
            cnf_settings["www_password"].as<String>(),
            cnf_settings["calibrate_temp"].as<String>()
        );
        open_settings = 1;
    }
    lastStateCLK = currentStateCLK;
    return open_settings;
}

void encoderSetup() {
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    lastStateCLK = digitalRead(CLK);
}