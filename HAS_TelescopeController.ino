
#include "src/pinConfig.h"
#include "HAS_TelescopeController.h"
#include "src/comms.h"
#include "src/ctrl.h"
#include "src/pos.h"
#include "src/io.h"
#include "src/stepper.h"

#include "src/utils.h"

String buffer ="";

command currentCmd;
String coordString;
// bool g_isSlewing = false;
bool initialSync = false;


#define DI_MODE 21
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
////////////////////////////////////////////////////////////////////////////////
/// Main Program
////////////////////////////////////////////////////////////////////////////////

// void declination_Stop(){
//   event_time = millis();
//   if (event_time - prior_event_time > 1000){
//     delay(10); //wait for bouncing to settle
//     if (digitalRead(2)) {
//       Serial.println("Declination Halted");
//       prior_event_time = event_time;
//     }
//   }
// }

namespace pos{
    const Position homePosition = {BASE, 0.00, 47.2536};
    Position targetPosition = {SKY, 0, 0};
    FrameSet currentLocation;
}
namespace ctrl{
    autoManualMode state = MANUAL;
}



// void declination_Stop(){
//     curr_DE = millis();
//     if (curr_DE - prev_DE > 1000){
//         if (!digitalRead(DI_PIN_DEC)) {
//             //Insert code to stop the declination
//             if ((isDecEn == true) && (isDecDir == false)) isDecEn = false;
//             //En = false;
//             Serial.println("Declination Halted");
//             prev_DE = curr_DE;
//         }
//     }
// }

// void RA_Home(){
//     curr_RA = millis();
//     if (curr_RA - prev_RA > 1000){
//         if (!digitalRead(DI_PIN_RA)) {
//             if ((isRaEn == true) && (isRaDir == true)) isRaEn = false;
//             Serial.println("RA Home");
//             prev_RA = curr_RA;
//         }
//     }
// }

void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);

    pinMode(DI_MODE, INPUT_PULLUP);
    pinMode(DI_RA_DIR, INPUT_PULLUP);
    pinMode(DI_RA_PUL, INPUT_PULLUP);
    pinMode(DI_DEC_DIR, INPUT_PULLUP);
    pinMode(DI_DEC_PUL, INPUT_PULLUP);
    pinMode(DI_TRACK, INPUT_PULLUP);
    pinMode(AI_POT, INPUT);

    raStp.init(DO_RA_STP_DIR, PWM_RA_STP_PUL, maxFreq, raCal);
    decStp.init(DO_DEC_STP_DIR, PWM_DEC_STP_PUL, maxFreq, decCal);

    // attachInterrupt(digitalPinToInterrupt(DI_PIN_DEC), declination_Stop, CHANGE);
    // attachInterrupt(digitalPinToInterrupt(DI_PIN_RA), RA_Home, CHANGE);

  // ctrl::waitForSync();

}

  // command currentCmd;
void loop() {
    // The sidereal time is updated to account for the elapsed time since last 
    // loop. Then the base position is updated based on calculated movement. 
    // Finally, the base position is converted to celestial coordinates.
    pos::SiderealTime::update();
    ctrl::state = (digitalRead(DI_MODE) == LOW) ? AUTO : MANUAL;
    // io::inputUpdate();

    // if (g_isSlewing) ctrl::simSlew(pos::currentLocation, pos::targetPosition);

    switch(ctrl::state){
    case AUTO:
        if (comms::readStringUntilChar(buffer, '#')) {
            currentCmd = comms::parseCommand(buffer);
            switch (currentCmd) {
            case GET_RA:
            // Serial1.println("GET_RA");
            comms::sendReply(comms::double2RaStr(pos::currentLocation.getCoord(SKY, RA)));
            // comms::sendReply("00:00:00");
            buffer = "";
            break;

            case GET_DEC:
            comms::sendReply(comms::double2DecStr(pos::currentLocation.getCoord(SKY, DECL)));
            // comms::sendReply("+00:00:00");
            buffer = "";
            break;

            case SYNC_POSITION:
            if (!initialSync){
                pos::SiderealTime::sync(pos::homePosition, pos::targetPosition);
                pos::currentLocation.initialiseSiderealTime(pos::SiderealTime::getValue());
                initialSync = true;
            }
            pos::currentLocation.syncTo(pos::targetPosition);
            // pos::SiderealTime::sync(pos::currentLocation);

            comms::sendReply("SYNCED TO " + 
                            comms::double2RaStr(pos::currentLocation.getCoord(SKY, RA)) + " " +
                            comms::double2DecStr(pos::currentLocation.getCoord(SKY, DECL)));
            buffer = "";
            break;

            case SLEW_TO_TARGET:
            comms::sendReply(ctrl::checkTargetReachable(pos::targetPosition));
            ctrl::moveTo(pos::targetPosition);
            // g_isSlewing = true;
            buffer = "";
            break;

            case STOP_SLEW:
            ctrl::stopAllMovement();
            buffer = "";
            break;

            case SET_TARGET_RA:
            coordString = comms::extractCoord(buffer);
            pos::targetPosition.ra = comms::raStr2Double(coordString);
            comms::sendReply("1"); //add checking of of parsed string later
            buffer = "";
            Serial1.println("SET_TARGET_RA:");
            Serial1.println(pos::targetPosition.ra);
            break;

            case SET_TARGET_DEC:
            coordString = comms::extractCoord(buffer);
            pos::targetPosition.dec = comms::decStr2Double(coordString);
            comms::sendReply("1"); //add checking of of parsed string later
            buffer = "";
            Serial1.println("SET_TARGET_DEC:");
            Serial1.println(pos::targetPosition.dec);
            break;

            default:
            //        Serial1.println("WARN: invalid command received");
            buffer = "";
            break;
            }
        
        // Serial1.print("RX " + comms::receive);
        }
    break;
    case MANUAL:
    default:
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
            Serial.println("RA: " + String(raStp.getStepCount()) + " DEC: " + String(decStp.getStepCount()) + " MODE: " + String(digitalRead(DI_MODE)));
        }
    }
    // io::outputUpdate();
}
