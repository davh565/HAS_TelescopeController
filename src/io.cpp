// #include "HAS_TelescopeController.h"
#include "config.h"
#include "io.h"
#include "encoder.h"


namespace io{

    pos::Position getMotorPositions(Stepper& raStp, Stepper& decStp){
    pos::Position pos;
    pos.frame = MOTOR;
    pos.ra = raStp.getStepCount()/raStp.getPulsesPerDeg();
    pos.dec = -decStp.getStepCount()/decStp.getPulsesPerDeg();
    return pos;
}

void setupPinModes(){
        pinMode(DI_MODE, INPUT_PULLUP);
    pinMode(DI_RA_DIR, INPUT_PULLUP);
    pinMode(DI_RA_PUL, INPUT_PULLUP);
    pinMode(DI_DEC_DIR, INPUT_PULLUP);
    pinMode(DI_DEC_PUL, INPUT_PULLUP);
    pinMode(DI_TRACK, INPUT_PULLUP);
    pinMode(AI_POT, INPUT);
}

}