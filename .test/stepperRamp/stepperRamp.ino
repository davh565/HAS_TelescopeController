#include "src/pinConfig.h"
// #include "HAS_TelescopeController.h"
// #include "src/comms.h"
// #include "src/pos.h"
// #include "src/ctrl.h"
// #include "src/io.h"
// #include "src/utils.h"
// #include "src/pinConfig.h"
#include "src/stepper.h"
#include <Filters.h>
// #include "testData.h"
// #include "src/pulse.h"

// #include "pulse.h"
#define DI_DEC_DIR 24
#define DI_DEC_PUL 25
#define DI_RA_DIR 22
#define DI_RA_PUL 23
#define DI_TRACK A9
#define AI_POT A8

const double trackRateHz = 257.0;
double slewRateHz = 250.0;
direction isRaDir;
direction isDecDir;
bool isRaPul;
bool isDecPul;
bool isTrack;
int potVal;
// StepperParams: {pulesPerDeg, rampUpTime, rampDownTime, minRampFreq, maxFreq}
StepperParams raParams = {30742.88868,1.5,0.5,25000,60000};
StepperParams decParams = {99603.48705,1.5,0.5,25000,60000};

// PulseGenerator raPulse;
// PulseGenerator decPulse;

FilterOnePole potLPF( LOWPASS, 0.5); //Low pass filter used to smoother potentiometer readings
io::Stepper decStp;
io::Stepper raStp;

void setup()
{
    pinMode(DI_RA_DIR, INPUT_PULLUP);
    pinMode(DI_RA_PUL, INPUT_PULLUP);
    pinMode(DI_DEC_DIR, INPUT_PULLUP);
    pinMode(DI_DEC_PUL, INPUT_PULLUP);
    pinMode(DI_TRACK, INPUT_PULLUP);
    pinMode(AI_POT, INPUT);
    Serial.begin(9600);
    Serial.println("Starting");
    // TEST_STEPPERS();
    // raStp.runPulses(200000);
    // decStp.runPulses(200000);
    // decStp.runAngle(-15);

    // decPulse.init(PWM_DEC_STP_PUL, 60000, 120000, 120000, 25000);
    // raPulse.init(PWM_RA_STP_PUL, 60000, 120000, 120000, 25000);
    // decPulse.setDirection(REVERSE);
    // raPulse.setDirection(REVERSE);
    // decPulse.setTarget(500000);
    // decPulse.enable();
    // raPulse.setTarget(-500000);
    // raPulse.enable();

    decStp.init(DO_DEC_STP_DIR,PWM_DEC_STP_PUL, decParams);
    raStp.init(DO_RA_STP_DIR,PWM_RA_STP_PUL, raParams);
    decStp.runPulses(200000);

    

}
void loop()

{
// Serial.println("DEC:" + String(decStp.getStepCount()) + " RA:" + String(raStp.getStepCount()));
// Serial.println("DEC:" + String(decPulse.getCount()) + " RA:" + String(raPulse.getCount()));
}




// void setup()
// {
//     pinMode(DI_RA_DIR, INPUT_PULLUP);
//     pinMode(DI_RA_PUL, INPUT_PULLUP);
//     pinMode(DI_DEC_DIR, INPUT_PULLUP);
//     pinMode(DI_DEC_PUL, INPUT_PULLUP);
//     pinMode(DI_TRACK, INPUT_PULLUP);
//     pinMode(AI_POT, INPUT);
    
//     Serial.begin(9600);

// }
// void loop()

// {
//     // potLPF.input(analogRead(AI_POT));
//     // potVal = potLPF.output();
//     potVal = analogRead(AI_POT);
//     // slewRateHz: Quadratic curve was found to provide good control at low speeds, while 
//     // still allowing for high speeds at the top end of the potentiometer range
//     slewRateHz = 0.058*potVal*potVal; 
//     isRaDir = !digitalRead(DI_RA_DIR) ? FORWARD: REVERSE;
//     isRaPul = !digitalRead(DI_RA_PUL);
//     isDecDir = !digitalRead(DI_DEC_DIR) ? FORWARD: REVERSE;
//     isDecPul = !digitalRead(DI_DEC_PUL);
//     isTrack = !digitalRead(DI_TRACK);

//     // if (isRaPul) raStp.run(isRaDir, 30000);
//     // else raStp.stop();
//     // decStp.runPulses(10000);
//     // else decStp.stop();
//     // else decStp.stop();
//     // if (isRaPul) raStp.run(isRaDir, slewRateHz);
//     // else if(isTrack) raStp.run(isRaDir, trackRateHz);
//     // else raStp.stop();
//     // if(isDecPul) decStp.run(isDecDir, slewRateHz);
//     // else decStp.stop();
//     // Serial.println("DEDIR: " + String(isDecDir) + "RADIR: " +String(isRaDir));
// }
