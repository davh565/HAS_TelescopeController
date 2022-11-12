#include "src/pinConfig.h"
// #include "HAS_TelescopeController.h"
// #include "src/comms.h"
// #include "src/pos.h"
// #include "src/ctrl.h"
// #include "src/io.h"
// #include "src/utils.h"
// #include "src/pinConfig.h"
#include "src/stepper.h"
// #include "testData.h"

// use the following site for independent coordinate conversion to produce reference values
// https://www.swift.psu.edu/secure/toop/convert.htm
#define DI_DEC_DIR 24
#define DI_DEC_PUL 25
#define DI_RA_DIR 22
#define DI_RA_PUL 23
#define DI_TRACK A9
#define AI_POT A8
double trackRateHz = 250.0;
double slewRateHz = 250.0;
bool isRaDir;
bool isRaPul;
bool isDecDir;
bool isDecPul;
bool isTrack;
int potVal;

io::Stepper raStp;
io::Stepper decStp;

        uint32_t maxFreq = 50000;
stepperCalibration raCal = {30742.88868,-0.4805,32558};
stepperCalibration decCal = {99603.48705,-1.2116,74717};
void testStepper(){

}
        // raStp.runAngle(5);
        // decStp.runAngle(-10);

void setup(){
    Serial.begin(9600);
raStp.init(DO_RA_STP_DIR, PWM_RA_STP_PUL, maxFreq, raCal);
decStp.init(DO_DEC_STP_DIR, PWM_DEC_STP_PUL, maxFreq, decCal);
        raStp.runAngle(5);
        decStp.runAngle(10);
    // testStepper();


}

void loop(){
    
    // Serial.println(io::motor.getDirection());
    // io::motor.changeDirection();
    // Serial.println(REVERSE);
    // static bool tst = false;
    // digitalWrite(DO_RA_STP_DIR, tst);
    // tst = !tst;
}