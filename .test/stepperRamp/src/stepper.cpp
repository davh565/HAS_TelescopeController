#include "stepper.h"
#include "enums.h"
// #include "io.h"

namespace io{


    void Stepper::init(int pinDIR, PulsePin pinPUL, StepperParams params){
        this->pinDIR = pinDIR;
        maxFrequency = params.maxFrequency;
        pulsesPerDeg = params.pulsesPerDeg;
        rampUpTime = params.rampUpTime;
        rampDownTime = params.rampDownTime;
        minRampFrequency = params.minRampFrequency;
        frequency = maxFrequency;
        rampUpPulses = (uint32_t)(rampUpTime * frequency);
        rampDownPulses = (uint32_t)(rampDownTime * frequency);
        dir = FORWARD;
        isEnabled = false;
        pinMode(pinDIR, OUTPUT);
        // Serial.println("StepFreq: "+ String(maxFrequency));
        Pulse.init(pinPUL, maxFrequency, rampUpPulses, rampDownPulses, minRampFrequency);
        Pulse.setDirection(dir);
        Pulse.setPulseFrequency(45000);

        // Pulse.setTarget(120000);
        // setFrequency(45000);
        // Pulse.enable();
    }

    /// @brief Set the frequency of the stepper motor. Cap at maxFrequency if input is greater.
    void Stepper::setFrequency(uint32_t frequency){
        if (frequency > maxFrequency) frequency = maxFrequency;
        this->frequency = frequency;
        Pulse.setPulseFrequency(frequency);
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
    void Stepper::runPulses(int32_t steps){
        // noInterrupts();
                Pulse.setTarget(120000);
        Pulse.enable();
        // interrupts();
        // if (steps < 0) setDirection(REVERSE);
        // else setDirection(FORWARD);
        // long currentStep = getStepCount();
        // setTarget(currentStep + steps);
        // enable();
    }

    /// @brief  Run the stepper motor to a target position.
    void Stepper::runToTarget(int32_t target){
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

    /// @brief  Move the stepper motor by a number of degrees.
    /// @param angle 
    void Stepper::runAngle(double angle){
        runPulses(angle*pulsesPerDeg);
    }

    /// @brief  Move the stepper motor to a target angle.
    /// @param angle
    void Stepper::runToAngle(double angle){
        runToTarget(angle*pulsesPerDeg);
    }

    /// @brief  Run the stepper motor at specified speed indefinitely
    void Stepper::run(direction dir, uint32_t frequency){
        setFrequency(frequency);
        run(frequency);
        enable();
    }

    /// @brief  Run the stepper motor at current speed indefinitely
    void Stepper::run(direction dir){
        if (dir == FORWARD) runPulses(2147483647);
        else runPulses(-2147483647);
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
        setTarget(getStepCount());
    }

    /// @brief Stop the motor after ramping down to the minimum ramp frequency.
    void Stepper::rampStop(){
        if(isEnabled && frequency > minRampFrequency){
            if(dir == FORWARD) Pulse.changeTarget(getStepCount() + rampDownPulses);
            else Pulse.changeTarget(getStepCount() - rampDownPulses);
        }
    }

}
