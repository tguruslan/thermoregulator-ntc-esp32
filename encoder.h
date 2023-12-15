#define SW 38
#define CLK 39
#define DT 40
int currentStateCLK;
int lastStateCLK;

int buttonData(){
    return digitalRead(SW);
}

float encoderData(float set_temp){
    currentStateCLK = digitalRead(CLK);
    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {
        if (digitalRead(DT) != currentStateCLK) {
            set_temp ++;
        } else {
            set_temp --;
        }
    }
    lastStateCLK = currentStateCLK;
    return set_temp;
}

void encoderSetup() {
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(SW, INPUT);
    lastStateCLK = digitalRead(CLK);
}