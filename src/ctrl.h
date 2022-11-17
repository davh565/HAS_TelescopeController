#ifndef HAS_CTRL
#define HAS_CTRL
#include "enums.h"
#include "pos.h"
// namespace io{
// extern Stepper;
    
// }

#include "stepper.h"

// #include "HAS_TelescopeController.h"

enum autoManualMode{
    AUTO,
    MANUAL,
    BOTH
};
/// @brief The ctrl namespace contains higher level control functionality for 
/// controlling the telescope.
namespace ctrl {
    const double trackRateHz = 125;
    const double minAltitude = 4.8;
    // void commandActions(command cmd);
    String checkTargetReachable(pos::Position target);
    void stopAllMovement(io::Stepper& ra, io::Stepper& dec);
    void horizonStop(pos::FrameSet& currentLocation, io::Stepper& ra, io::Stepper& dec); // Check if the current location is below the horizon and stop the motors if it is
    
    // void calibrateHomePosition();
    void move(pos::FrameSet& currentLocation, pos::Position& targetPosition, io::Stepper& ra, io::Stepper& dec);

    // void moveSteps(int axis, uint32_t steps);
    // void waitForSync();
    // void simSlew(pos::FrameSet& currentLocation, pos::Position& targetPosition);
};

#endif //HAS_CTRL