#include "src/pulse.h"
#include "src/config.h"
PulseGenerator raPulse;

    double testFreq = 25000;
void setup(){
    pinMode(DI_MODE, INPUT_PULLUP);
    Serial.begin(9600);
    Serial.println("Hello World!");
    raPulse.init(PWM_RA_STP_PUL, testFreq);
//         // raPulse.setRunMode(CONSTANT);
//         // raPulse.setFrequency(testFreq);

}
void loop(){
    static long target = 50000;
    static bool prevState = false;
    bool currState = digitalRead(DI_MODE);
    if(currState != prevState){
        raPulse.setFrequency(testFreq);
        raPulse.setTarget(target);
        raPulse.enable();
        target += 50000;
        prevState=currState;
        delay(2000);

        
    }

}