#include "pulse.h"

#define DI_DEC_DIR 24
#define DI_DEC_PUL 25
#define DI_RA_DIR 22
#define DI_RA_PUL 23
#define DI_TRACK A9
// #define DO_RA_EN 8 
// #define DO_DEC_EN 11
#define DO_DEC_DIR 4
#define DO_DEC_PUL 5
#define DO_RA_PUL 6
#define DO_RA_DIR 7
#define AI_POT A8

#define RA 0
#define DE 1

// Sidereal track rate 15.041 arcseconds per second
// 15.041/3600 = 0.0041780555555556 deg/s
// RA Stepper: 2500 step/deg
// trackRate = 0.004178*2500 = 10.44513888888889 step/s == Hz
const double trackRateHz = 250.0;
double slewRateHz = 250.0;
bool isRaDir;
bool isRaPul;
bool isDecDir;
bool isDecPul;
bool isTrack;
int potVal;


PulseGenerator raPulse;
PulseGenerator decPulse;

void setup()
{
    pinMode(DI_RA_DIR, INPUT_PULLUP);
    pinMode(DI_RA_PUL, INPUT_PULLUP);
    pinMode(DI_DEC_DIR, INPUT_PULLUP);
    pinMode(DI_DEC_PUL, INPUT_PULLUP);
    pinMode(DI_TRACK, INPUT_PULLUP);
    pinMode(AI_POT, INPUT);
    // pinMode(DO_RA_EN, OUTPUT);
    // pinMode(DO_DEC_EN, OUTPUT);
    pinMode(DO_DEC_DIR, OUTPUT);
    // pinMode(DO_DEC_PUL, OUTPUT);
    pinMode(DO_RA_DIR, OUTPUT);
    // pinMode(DO_RA_PUL, OUTPUT);
    raPulse.init(DO_RA_PUL, slewRateHz);
    decPulse.init(DO_DEC_PUL, slewRateHz);
    // Serial.begin(9600);

}
void loop()

{
    potVal = analogRead(AI_POT);
    slewRateHz = 0.095*potVal*potVal;
    // Serial.println(slewRateHz);
    isRaDir = !digitalRead(DI_RA_DIR);
    isRaPul = !digitalRead(DI_RA_PUL);
    isDecDir = !digitalRead(DI_DEC_DIR);
    isDecPul = !digitalRead(DI_DEC_PUL);
    isTrack = !digitalRead(DI_TRACK);

    // digitalWrite(DO_RA_EN,isRaPul);
    // digitalWrite(DO_DEC_EN,isDecPul);
    if(isRaPul){
        raPulse.setFrequency(slewRateHz);
        raPulse.setDirection(isRaDir);
        raPulse.enable();
        digitalWrite(DO_RA_DIR,isRaDir);
    }
    else if(isTrack){
        raPulse.setFrequency(trackRateHz);
        raPulse.setDirection(FORWARD);
        raPulse.enable();
        digitalWrite(DO_RA_DIR,FORWARD);
    }
    else{
        raPulse.disable();
        raPulse.setDirection(isRaDir);
        digitalWrite(DO_RA_DIR,isRaDir);
    }
    if(isDecPul){
        decPulse.setFrequency(slewRateHz);
        decPulse.setDirection(isDecDir);
        decPulse.enable();
    }
    else{
        decPulse.disable();
    }

    decPulse.setDirection(isDecDir);
    digitalWrite(DO_DEC_DIR,isDecDir);
}
