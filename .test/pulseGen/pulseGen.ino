#include "pulse.h"
PulseGenerator p1;
PulseGenerator p2;
void setup(){
    Serial.begin(9600);
    // p1.init(11, 1000);
    // p3.init(5, 1000);
    p1.init(5, 2000);
    p2.init(6, 2000);
    // p4.setTarget(4);
    p1.enable();
    p2.enable();
}

void loop(){
    // static bool dir = FORWARD;
    // // static int frq = 100;
    // // p1.setFrequency(frq);
    // // p3.setFrequency(frq);
    // // p4.setFrequency(frq);
    // // p5.setFrequency(frq);
    // // frq +=1;
    // // if(frq > 2000) frq = 5;
    
    // Serial.println(p4.getCount());


}