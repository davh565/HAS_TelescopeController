#ifndef HAS_STP
#define HAS_STP

// #include "../HAS_TelescopeController.h"
#include <Arduino.h>
#include "enums.h"
#include "utils.h"
#include "pulse.h"

struct stepperCalibration{
    double pulsesPerDeg;
    double correctionSlope;
    double correctionIntercept;
};

namespace io{
    class Stepper{
    private:
        bool isEnabled;
        // bool flagResetCount;
        double pulsesPerDeg;
        double correctionSlope;
        double correctionIntercept;
        direction dir;
        int pinDIR;
        uint32_t maxFrequency;
        uint32_t frequency;
        void resetStepCount(){Pulse.resetCount();}
        void setCount(long count){Pulse.setCount(count);}

        void setDirection(direction dir);
        void setTarget(int32_t target){Pulse.setTarget(target);}
        void setEnabled(bool isEnabled);
        int getDirection() {return dir;}
    public:
        PulseGenerator Pulse;
        uint32_t getFrequency() {return frequency;}
        bool getEnabled() {return isEnabled;}
        double getPulsesPerDeg() {return pulsesPerDeg;}
        long getStepCount() {return Pulse.getCount();}
        void setFrequency(uint32_t frequency);
        void runSteps(int32_t steps);
        void runAngle(double angle);
        void runToTarget(int32_t target);
        void run(direction dir, uint32_t frequency);
        void run(direction dir);
        void disable();
        void enable();
        void runSteps(){runSteps(0);}
        void stop();
        void init(int pinDIR, PulsePin pinPUL, uint32_t maxFrequency, stepperCalibration calParams);
    };
}
#endif