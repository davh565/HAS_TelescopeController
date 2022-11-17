// #include "HAS_TelescopeController.h"
#include "config.h"
#include "io.h"
#include "encoder.h"
bool g_decLimLo = false;
bool g_decLimHi = false;

/// @brief Triggered when the lower declination limit is reached.
void decLimLoISR(){
    g_decLimLo = true;
}

/// @brief Triggered when the upper declination limit is reached.
void decLimHiISR(){
    g_decLimHi = true;
}

namespace io{
    /// @brief get the current position in degrees in the motor reference frame.
    /// @param ra The RA stepper motor.
    /// @param dec The Dec stepper motor.
    pos::Position getMotorPositions(Stepper& ra, Stepper& dec){
        pos::Position pos;
        pos.frame = MOTOR;
        pos.ra = ra.getPulseCount()/ra.getPulsesPerDeg();
        pos.dec = -dec.getPulseCount()/dec.getPulsesPerDeg();
        return pos;
}

    /// @brief set the pin modes for I/O pins.
    void setupPinModes(){
        pinMode(DI_MODE, INPUT_PULLUP);
        pinMode(DI_RA_DIR, INPUT_PULLUP);
        pinMode(DI_RA_PUL, INPUT_PULLUP);
        pinMode(DI_DEC_DIR, INPUT_PULLUP);
        pinMode(DI_DEC_PUL, INPUT_PULLUP);
        pinMode(DI_TRACK, INPUT_PULLUP);
        pinMode(AI_POT, INPUT);
    }

    /// @brief set up the limit switches.
    void setupLimits(){
        attachInterrupt(digitalPinToInterrupt(DI_DEC_LIM_LO), decLimLoISR, LOW);
        attachInterrupt(digitalPinToInterrupt(DI_DEC_LIM_HI), decLimHiISR, LOW);
    }

    /// @brief stop the declination motor if a limit switch is triggered.
    /// @param dec the declination stepper motor.
    void limitStop(Stepper& dec){
        if(g_decLimLo && dec.getDirection() == REVERSE){
            dec.stop();
            g_decLimLo = false;
        }
        if(g_decLimHi && dec.getDirection() == FORWARD){
            dec.stop();
            g_decLimHi = false;
        }

    }
}