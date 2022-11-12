#ifndef HAS_CTRL
#define HAS_CTRL
#include "enums.h"
#include "pos.h"

// #include "HAS_TelescopeController.h"

enum runMode{
    DONE,
    CONSTANT,
    STEPS,
    ANGLE
};
enum autoManualMode{
    AUTO,
    MANUAL,
    BOTH
};
namespace ctrl {
    // void commandActions(command cmd);
    String checkTargetReachable(pos::Position target);
    void stepperControl();
    void stopAllMovement();
    void calibrateHomePosition();
    void moveTo(pos::Position position);
    void moveSteps(int axis, uint32_t steps);
    void waitForSync();
    void simSlew(pos::FrameSet& currentLocation, pos::Position& targetPosition);
};

#endif //HAS_CTRL

        // double stepsPerDeg;
        // unsigned long prevPulseMicros;
        // int targetSteps;
        // int stepCount;
        // double angleMoved;
        // double targetAngle;
         // int getTargetSteps() {return targetSteps}
        // int getStepCount() {return stepCount}
        // double getAngleMoved() {return angleMoved}
        // double getTargetAngle() {return targetAngle}
        // int getRunMode() {return runMode}
        
        // void setTargetSteps(int targetSteps);
        // void setTargetAngle(double targetAngle);
        // void setRunMode(int runMode);
        // void resetCount();
        // bool pulse();