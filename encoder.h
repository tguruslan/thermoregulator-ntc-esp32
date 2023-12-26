#define SW 0
#define CLK 16
#define DT 13
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
        if(set_temp > 400){set_temp=20;}
        if(set_temp < 1){set_temp=400;}
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