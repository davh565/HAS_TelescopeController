#ifndef HAS_UTILS
#define HAS_UTILS

#include <Arduino.h>

/// @brief Software-based Pulse Generator. Reccomend using hardware timers for
///        more accurate timing.
class PulseGeneratorSoft{
    public:
        PulseGeneratorSoft(unsigned int frequency=1000);
        bool getPulseState(){return output;}
        bool pulse();
        long getStepCount(){return pulseCount;}
        unsigned int getFrequency(){return frequency;}
        void resetStepCount(){pulseCount = 0;}
        void setFrequency(unsigned int frequency){this->frequency = frequency;}
        void __SET_PULSE_COUNT(unsigned long pulseCount){this->pulseCount = pulseCount;}
    private:
        unsigned long curMicros;
        unsigned long prevMicros;
        unsigned int frequency;
        bool output;
        long pulseCount;
};

double wrap90(double input);
double wrap180(double input);
double wrap360(double input);

double sind(double input);
double cosd(double input);
double tand(double input);
double asind(double input);
double acosd(double input);
double atand2(double y, double x);


#endif // HAS_UTILS