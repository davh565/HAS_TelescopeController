#ifndef HAS_IO
#define HAS_IO

#include "pos.h"
#include "stepper.h"



namespace io{
    pos::Position getMotorPositions(Stepper& raStp, Stepper& decStp);
    void setupPinModes();
    void stopMotors();
}
#endif
