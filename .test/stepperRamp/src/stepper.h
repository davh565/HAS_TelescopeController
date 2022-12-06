#ifndef HAS_STP
#define HAS_STP

// #include "../HAS_TelescopeController.h"
#include <Arduino.h>
#include "enums.h"
#include "utils.h"
#include "pulse.h"

struct StepperParams{
    double pulsesPerDeg;
    double rampUpTime;
    double rampDownTime;
    uint32_t minRampFrequency;
    uint32_t maxFrequency;
};

namespace io{
    class Stepper{
    private:
        PulseGenerator Pulse;
        bool isEnabled;
        // bool flagResetCount;
        double pulsesPerDeg;
        double rampUpTime;
        double rampDownTime;
        uint32_t rampUpPulses;
        uint32_t rampDownPulses;

        uint32_t minRampFrequency;
        direction dir;
        int pinDIR;
        uint32_t maxFrequency;
        uint32_t frequency;
        void resetStepCount(){Pulse.resetCount();}
        void setCount(long count){Pulse.setCount(count);}

        void disable();
        void enable();
        void setDirection(direction dir);
        void setTarget(int32_t target){Pulse.setTarget(target);}
        void setEnabled(bool isEnabled);
        int getDirection() {return dir;}
    public:
        uint32_t getFrequency() {return frequency;}
        bool getEnabled() {return isEnabled;}
        // double getStepsPerDeg() {return stepsPerDeg;}
        long getStepCount() {return Pulse.getCount();}
        void setFrequency(uint32_t frequency);
        void runPulses(int32_t steps);
        void runAngle(double angle);
        void runToAngle(double angle);
        void runToTarget(int32_t target);
        void run(direction dir, uint32_t frequency);
        void run(direction dir);
        void stop();
        void rampStop();
        void init(int pinDIR, PulsePin pinPUL, StepperParams params);
    };
}
#endif