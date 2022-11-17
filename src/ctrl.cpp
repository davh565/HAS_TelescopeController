#include <Arduino.h>
#include "pos.h"
#include "ctrl.h"
#include "comms.h"
#include "enums.h"
#include "utils.h"
#include "stepper.h"
// #include "io.h"

// extern bool g_isSlewing;

namespace ctrl {
  // pos::Position localPosition;
    


    String checkTargetReachable(pos::Position target) {
        String str;
        //target reachable
        if (true) str = "0#";
        // target below horizon
        else if (false) str = "1 ERROR: OBJECT BELOW HORIZON#";
        // target not reachable
        else str = "2 ERROR: OBJECT NOT REACHABLE#";
        return str;
    }


    void stopAllMovement(io::Stepper& ra, io::Stepper& dec) {
        ra.stop();
        dec.stop();
        // io::stopMotors();
    }

    void horizonStop(pos::FrameSet& currentLocation, io::Stepper& ra, io::Stepper& dec){
        if (currentLocation.getCoord(ALTAZ, ALT) < minAltitude) {
            stopAllMovement(ra, dec);
            pos::FrameSet newTarget = currentLocation;
            newTarget.updateCoord(ALTAZ, ALT, minAltitude+0.2);
            pos::Position newPos = newTarget.getPosition(SKY);
            move(currentLocation, newPos, ra, dec);

        }

    }


    void move(pos::FrameSet& currentLocation, pos::Position& targetPosition, io::Stepper& ra, io::Stepper& dec) {
            ra.setFrequency(20000);
            dec.setFrequency(60000);
            double deltaRa = -wrap180(targetPosition.ra - currentLocation.getCoord(SKY, RA));
            double deltaDec = -wrap180(targetPosition.dec - currentLocation.getCoord(SKY, DECL));
            // double estRaMoveTime = deltaRa1*ra.getPulsesPerDeg()/ra.getFrequency();
            // double trackingOffset = estRaMoveTime*trackRateHz/ra.getPulsesPerDeg();
            double trackingOffset = deltaRa*trackRateHz/double(ra.getFrequency());
            double deltaRaTotal = wrap180(deltaRa + trackingOffset);
            ra.runAngle(deltaRaTotal);
            dec.runAngle(deltaDec);
            // io::moveSteppers(deltaRa, deltaDec);
        }

// checkPath
    // void move(pos::FrameSet& currentLocation, pos::Position& targetPosition, io::Stepper& ra, io::Stepper& dec) {
    //         double deltaRa = wrap180(targetPosition.ra - currentLocation.getCoord(SKY, RA));
    //         double deltaDec = wrap180(targetPosition.dec - currentLocation.getCoord(SKY, DECL));
    //         ra.runAngle(deltaRa);
    //         dec.runAngle(deltaDec);
    //         // io::moveSteppers(deltaRa, deltaDec);
    //     }
    }
    // void simSlew(pos::FrameSet& currentLocation, pos::Position& targetPosition){
    //     static unsigned long prevMillis = millis();
    //     unsigned long currentMillis = millis();
    //     const int updateRate = 100;
    //     if (currentMillis - prevMillis >= updateRate) {
    //         double deltaRa = wrap180(targetPosition.ra - currentLocation.getCoord(SKY, RA));
    //         double deltaDec = wrap180(targetPosition.dec - currentLocation.getCoord(SKY, DECL));
            
    //         Serial1.println(String(currentLocation.getCoord(SKY, RA)) + " " + String(deltaRa) + " " + String(currentLocation.getCoord(SKY, DECL)) + " " + String(deltaDec));
    //         const double slewRateRa = 0.1253417;//1.0;
    //         const double slewRateDec = 0.0680118;//0.5;
    //         const double threshold = 0.25;
    //         int raDir = deltaRa > 0 ? 1 : -1;
    //         int decDir = deltaDec > 0 ? 1 : -1;
    //         if (abs(deltaRa) > threshold){
    //             currentLocation.incrementCoord(MOTOR, RA, -raDir * slewRateRa);
    //         }
    //         if (abs(deltaDec) > threshold){
    //             currentLocation.incrementCoord(MOTOR, DECL, decDir * slewRateDec);
    //         }
    //         if (abs(deltaRa) <= threshold && abs(deltaDec) <= threshold) g_isSlewing = false;
    //         prevMillis = currentMillis;
    //     }
    // }

// };
    // void Stepper::setEnabled(bool isenabled){this->isEnabled = isEnabled;}
    // void setDirection(direction direction){
        // resetCount();
        // this->direction = direction;
        // runMode = DONE;
        // }
    // void Stepper::setTargetSteps(int targetSteps){
    //     this->targetSteps = targetSteps;
    //     }
    // void Stepper::setTargetAngle(double targetAngle){
    //     this->targetAngle = targetAngle;
    //     }
    // void Stepper::setRunMode(int runMode){
    //     resetCount();
    //     this->runMode = runMode;
    //     }
    // void Stepper::resetCount(){
    //     stepCount = 0;
    //     angleMoved = 0.0;
    // }

        // bool Stepper::pulse(){
    //     static unsigned long curMicros = micros();
    //     stepsTaken = (curMicros - prevPulseMicros)/(double(period)/2);
    //     angleMoved += stepsTaken/stepsPerDeg;
    //     prevPulseMicros = curMicros;
    //     return Pulse.pulse(frequency);
    // }