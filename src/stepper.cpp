#include "stepper.h"
#include "enums.h"
// #include "io.h"

namespace io{


    void Stepper::init(int pinDIR, PulsePin pinPUL, uint32_t maxFrequency, stepperCalibration calParams){
        this->pinDIR = pinDIR;
        this->maxFrequency = maxFrequency;
        this->pulsesPerDeg = calParams.pulsesPerDeg;
        this->correctionSlope = calParams.correctionSlope;
        this->correctionIntercept = calParams.correctionIntercept;
        frequency = maxFrequency;

        dir = FORWARD;
        isEnabled = false;
        pinMode(pinDIR, OUTPUT);
        // Serial.println("StepFreq: "+ String(maxFrequency));
        Pulse.init(pinPUL, maxFrequency);
        Pulse.setDirection(dir);
        Pulse.setRunMode(TARGET);
        // Serial.println("Stepper::Stepper()");
    }

    /// @brief Set the frequency of the stepper motor. Cap at maxFrequency if input is greater.
    void Stepper::setFrequency(uint32_t frequency){
        if (frequency > maxFrequency) frequency = maxFrequency;
        this->frequency = frequency;
        Pulse.setFrequency(frequency);
    }

    /// @brief  set the direction of the stepper motor
    /// @param dir 
    void Stepper::setDirection(direction dir){
        this->dir = dir;
        Pulse.setDirection(dir);
        // bool dirState = (dir == FORWARD) ? dirState = HIGH : dirState = LOW;
        digitalWrite(pinDIR, dir);
        }

    void Stepper::setEnabled(bool isEnabled){
        this->isEnabled = isEnabled;
        // Serial.println("Stepper::setEnabled()");
        if (isEnabled) Pulse.enable();
        else Pulse.disable();
    }

    /// @brief  Run the stepper motor for a number of steps from current
    /// position
    void Stepper::runSteps(int32_t steps){
        Pulse.setRunMode(TARGET);
        if (steps < 0) setDirection(REVERSE);
        else setDirection(FORWARD);
        long currentStep = getStepCount();
        setTarget(currentStep + steps);
        enable();
    }

    /// @brief  Run the stepper motor to a target position.
    void Stepper::runToTarget(int32_t target){
        Pulse.setRunMode(TARGET);
        long currentStep = getStepCount();
        if (target > currentStep) {
            setDirection(FORWARD);
            setTarget(target);
            enable();
        }
        else if (target < currentStep) {
            setDirection(REVERSE);
            setTarget(target);
            enable();
        }
    }

    /// @brief  Move the stepper motor by a number of degrees. If the frequency 
    /// is above the correction xIntercept, a linear correction factor must be
    /// applied
    /// @param angle 
    void Stepper::runAngle(double angle){
        // Serial.println(angle);
        runSteps(angle*pulsesPerDeg);
        // if (frequency <= xIntercept) {
        // double xIntercept = -correctionIntercept/correctionSlope;
            // runSteps(angle*pulsesPerDeg);
        // }
        // else {
            // double correction = correctionSlope*frequency + correctionIntercept;
            // runSteps(angle*(pulsesPerDeg+correction));
        // }
        // int32_t steps = angle * pulsesPerDeg;
        // Serial.println(pulsesPerDeg);
        // Serial.println(steps);
        // runSteps(steps);
    }

    /// @brief  Run the stepper motor at specified speed indefinitely
    void Stepper::run(direction dir, uint32_t frequency){
        setDirection(dir);
        setFrequency(frequency);
        Pulse.setRunMode(CONST);
        enable();
    }

    /// @brief  Run the stepper motor at current speed indefinitely
    void Stepper::run(direction dir){
        setDirection(dir);
        Pulse.setRunMode(CONST);
        enable();
    }

    /// @brief Stop the stepper motor by disabling the pulse generator. Does not
    /// change the target, so the motor will resume when enable() is called again.
    /// use stop() method instead to stop motor at current position indefinitely.
    void Stepper::disable(){setEnabled(false);}

    /// @brief Enable the stepper motor by enabling the pulse generator.
        void Stepper::enable(){setEnabled(true);}

    /// @brief Stop the stepper motor. This is achieved by setting the target
    /// step count to the current step count. This differs from the disable()
    /// method, which stops the stepper motor immediately, but does not effect
    /// the target.
    void Stepper::stop(){
        Pulse.setRunMode(TARGET);
        setTarget(getStepCount());
    }

}
