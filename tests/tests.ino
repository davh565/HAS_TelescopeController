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
int raDir;
int decDir;
bool isRaPul;
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
    pinMode(DI_RA_DIR, INPUT_PULLUP);
    pinMode(DI_RA_PUL, INPUT_PULLUP);
    pinMode(DI_DEC_DIR, INPUT_PULLUP);
    pinMode(DI_DEC_PUL, INPUT_PULLUP);
    pinMode(DI_TRACK, INPUT_PULLUP);
    pinMode(AI_POT, INPUT);

    raStp.init(DO_RA_STP_DIR, PWM_RA_STP_PUL, maxFreq, raCal);
    decStp.init(DO_DEC_STP_DIR, PWM_DEC_STP_PUL, maxFreq, decCal);

}

void loop()

{
    potVal = analogRead(AI_POT);
    slewRateHz = 0.095*potVal*potVal;
    // Serial.println(slewRateHz);
    raDir = !digitalRead(DI_RA_DIR);
    isRaPul = !digitalRead(DI_RA_PUL);
    decDir = !digitalRead(DI_DEC_DIR);
    isDecPul = !digitalRead(DI_DEC_PUL);
    isTrack = !digitalRead(DI_TRACK);

    // digitalWrite(DO_RA_EN,isRaPul);
    // digitalWrite(DO_DEC_EN,isDecPul);
    if(isRaPul){
        raStp.run(raDir, slewRateHz);
    }
    else if(isTrack){
        raStp.run(FORWARD, trackRateHz);
    }
    else{
        raStp.stop();
    }
    if(isDecPul){
        decStp.run(decDir, slewRateHz);
    }
    else{
        decStp.stop();
    }

    static unsigned long lastTime = 0;
    if(millis() - lastTime > 1000){
        lastTime = millis();
        Serial.println("RA: " + String(raStp.getStepCount()) + " DEC: " + String(decStp.getStepCount()));
    }

    // decPulse.setDirection(isDecDir);
    // digitalWrite(DO_DEC_DIR,isDecDir);
}