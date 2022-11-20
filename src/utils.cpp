#include <Arduino.h>
#include "utils.h"

/// @brief Software based pulse generator. Reccomend use of hardware based pulse generator
/// instead for higher precision and lower jitter.
/// @return true if pulse is high, false if pulse is low
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

/// @brief Constructor for the software based pulse generator.
/// @param frequency The frequency of the pulse output in Hz.
PulseGeneratorSoft::PulseGeneratorSoft(unsigned int frequency=1000){
    prevMicros = micros();
    output = false;
    pulseCount = 0;
}

/// @brief wrap values to plus or minus 90 degrees.
/// @param input The input value in degrees.
/// @return The wrapped value in degrees.
double wrap90(double input){
    while (input > 90) input -= 180;
    while (input <= -90) input += 180;
    return input;
}

/// @brief wrap values to plus or minus 180 degrees.
/// @param input The input value in degrees.
/// @return The wrapped value in degrees.
double wrap180(double input){
    while (input > 180.0) input -= 360.0;
    while (input <= -180.0) input += 360.0;
    return input;
}

/// @brief wrap values from 0 to 360 degrees.
/// @param input The input value in degrees.
/// @return The wrapped value in degrees.
double wrap360(double input){
    while (input >= 360.0)  input -= 360.0;
    while (input < 0.0)  input += 360.0;
    return input;
}

/// @brief  sine function with input in degrees
/// @param input input angle in degrees
/// @return sine of input
double sind(double input){
    return sin(input * PI / 180);
}

/// @brief  cosine function with input in degrees
/// @param input input angle in degrees
/// @return cosine of input
double cosd(double input){
    return cos(input * PI / 180);
}

/// @brief  tangent function with input in degrees
/// @param input input angle in degrees
/// @return tangent of input
double tand(double input){
    return tan(input * PI / 180);
}

/// @brief  arcsine function with output in degrees
/// @param input input value
/// @return arcsine of input in degrees
double asind(double input){
    return asin(input) * 180 / PI;
}

/// @brief  arccosine function with output in degrees
/// @param input input value
/// @return arccosine of input in degrees
double acosd(double input){
    return acos(input) * 180 / PI;
}

/// @brief  arctangent2 function with output in degrees
/// @param input input value
/// @return arctangent2 of input in degrees
double atand2(double y, double x){
    return atan2(y, x) * 180 / PI;
}
