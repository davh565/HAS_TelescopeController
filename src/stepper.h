#ifndef HAS_STP
#define HAS_STP

// #include "../HAS_TelescopeController.h"
#include <Arduino.h>
#include "enums.h"
#include "utils.h"
#include "pulse.h"

//TODO remove correction factors. Stepper should not run in region when missing steps are likely
struct stepperCalibration{
    double pulsesPerDeg;
    double correctionSlope;
    double correctionIntercept;
};

namespace io{
    /// @brief Stepper motor class.
    class Stepper{
    private:
        bool isEnabled;
        // bool flagResetCount;
        bool enableLimits;
        double pulsesPerDeg;
        double correctionSlope;
        double correctionIntercept;
        direction dir;
        int pinDIR;
        uint32_t maxFrequency;
        uint32_t frequency;
        void resetPulseCount(){Pulse.resetCount();}
        void setCount(long count){Pulse.setCount(count);}

        void setDirection(direction dir);
        void setTarget(int32_t target){Pulse.setTarget(target);}
        void setEnabled(bool isEnabled);
    public:
        PulseGenerator Pulse;
        int getDirection(){return dir;}
        uint32_t getFrequency() {return frequency;}
        uint32_t getMaxFrequency() {return maxFrequency;}
        bool getEnabled() {return isEnabled;}
        double getPulsesPerDeg() {return pulsesPerDeg;}
        long getPulseCount() {return Pulse.getCount();}
        void setFrequency(uint32_t frequency);
        void runPulses(int32_t steps);
        void runAngle(double angle);
        void runToTarget(int32_t target);
        void run(direction dir, uint32_t frequency);
        void run(direction dir);
        void disable();
        void enable();
        void runPulses(){runPulses(0);}
        void stop();
        void init(int pinDIR, PulsePin pinPUL, uint32_t maxFrequency, bool enableLimits, stepperCalibration calParams);
    };
}
#endif