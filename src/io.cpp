// #include "HAS_TelescopeController.h"
#include "pinConfig.h"
#include "io.h"
#include "encoder.h"
#include "stepper.h"


namespace io{
    const double raStepsPerDeg = 2500.0;
    const double decStepsPerDeg = 2500.0;
    const unsigned int maxFrequency = 100000; //100kHz is max safe speed where steppers
    // will not stall. Faster speeds can be attained if steppers are ramped (not implemented yet)


    // io::Stepper decStepper(DO_DEC_STP_DIR, PWM_DEC_STP_PUL,DECL, maxFrequency, decStepsPerDeg);
    // io::Stepper raStepper(DO_RA_STP_DIR, PWM_RA_STP_PUL, RA , maxFrequency, raStepsPerDeg);
    // // Encoder raEncoder(DI_RA_ENC_A, DI_RA_ENC_B, 2048);
    // // Encoder decEncoder(DI_DEC_ENC_A, DI_DEC_ENC_B, 2048);

    // pos::Position getMotorPositions(){
    //     pos::Position pos;
    //     pos.frame = MOTOR;
    //     pos.ra = raStepper.getStepCount()/raStepper.getStepsPerDeg();
    //     pos.dec = decStepper.getStepCount()/decStepper.getStepsPerDeg();
    //     return pos;
    // }

    // void stopMotors(){
    //     // raStepper.disable();
    //     // decStepper.disable();
    // }
    

}