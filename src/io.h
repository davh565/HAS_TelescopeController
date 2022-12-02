#ifndef HAS_IO
#define HAS_IO

#include "pos.h"
#include "stepper.h"


/// @brief The io namespace contains functionality for initialising, reading, 
/// and controlling sensors and actuators.
namespace io{
    pos::Position getMotorPositions(Stepper& ra, Stepper& dec);
    void setupPinModes();
    void setupLimits();
    void stopMotors();
    void limitStop(Stepper& dec);
    bool decLimCheck(int dir);

}
#endif
