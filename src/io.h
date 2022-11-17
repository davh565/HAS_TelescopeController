#ifndef HAS_IO
#define HAS_IO

#include "pos.h"
#include "stepper.h"



namespace io{
    pos::Position getMotorPositions(Stepper& ra, Stepper& dec);
    void setupPinModes();
    void setupLimits();
    void stopMotors();
    void limitStop(Stepper& dec);
}
#endif
