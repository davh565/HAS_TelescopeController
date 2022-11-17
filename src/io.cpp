// #include "HAS_TelescopeController.h"
#include "config.h"
#include "io.h"
#include "encoder.h"
bool g_decLimLo = false;
bool g_decLimHi = false;

void decLimLoISR(){
    g_decLimLo = true;
}

void decLimHiISR(){
    g_decLimHi = true;
}

namespace io{

    pos::Position getMotorPositions(Stepper& ra, Stepper& dec){
    pos::Position pos;
    pos.frame = MOTOR;
    pos.ra = ra.getStepCount()/ra.getPulsesPerDeg();
    pos.dec = -dec.getStepCount()/dec.getPulsesPerDeg();
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

void setupLimits(){
    attachInterrupt(digitalPinToInterrupt(DI_DEC_LIM_LO), decLimLoISR, LOW);
    attachInterrupt(digitalPinToInterrupt(DI_DEC_LIM_HI), decLimHiISR, LOW);
}

void limitStop(Stepper& dec){
    if(g_decLimLo){
        dec.stop();
        g_decLimLo = false;
    }
    if(g_decLimHi){
        dec.stop();
        g_decLimHi = false;
    }

}
}