#include <Arduino.h>
#include "utils.h"

bool PulseGeneratorSoft::pulse(){
    curMicros = micros();
    double period = 1000000/frequency;
    if (2*(curMicros - prevMicros) >= period){
        output = !output;
        if (output) pulseCount++;
        prevMicros = curMicros;
    }
    return output;
}

PulseGeneratorSoft::PulseGeneratorSoft(unsigned int frequency=1000){
    prevMicros = micros();
    output = false;
    pulseCount = 0;
}

double wrap90(double input){
    while (input > 90) input -= 180;
    while (input <= -90) input += 180;
    return input;
}

double wrap180(double input){
    while (input > 180.0) input -= 360.0;
    while (input <= -180.0) input += 360.0;
    return input;
}
double wrap360(double input){
    while (input >= 360.0)  input -= 360.0;
    while (input < 0.0)  input += 360.0;
    return input;
}

double sind(double input){
    return sin(input * PI / 180);
}

double cosd(double input){
    return cos(input * PI / 180);
}

double tand(double input){
    return tan(input * PI / 180);
}

double asind(double input){
    return asin(input) * 180 / PI;
}

double acosd(double input){
    return acos(input) * 180 / PI;
}

double atand2(double y, double x){
    return atan2(y, x) * 180 / PI;
}
