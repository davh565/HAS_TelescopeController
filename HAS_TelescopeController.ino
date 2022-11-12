
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
    autoManualMode state = AUTO;
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
  //   case MANUAL:
  //   default:
    //   slewRateHz = double(analogRead(AI_PIN_POT))*1.5;
    // isRaDir = !digitalRead(DI_PIN_RA_DIR);
    // isRaEn = !digitalRead(DI_PIN_RA_EN);
    // isDecDir = !digitalRead(DI_PIN_DEC_DIR);
    // isDecEn = !digitalRead(DI_PIN_DEC_EN);
    // isTrack = !digitalRead(DI_PIN_TRACK);

    // digitalWrite(DO_PIN_RA_DIR,isRaDir);
    // //digitalWrite(DO_PIN_RA_EN,isRaEn);
    // digitalWrite(DO_PIN_DEC_DIR,isDecDir);
    // //digitalWrite(DO_PIN_DEC_EN,isDecEn);
    // if(isTrack) digitalWrite(DO_PIN_RA_PUL, Track.outputPulse(trackRateHz));
    // else if(isRaEn) digitalWrite(DO_PIN_RA_PUL, Slew.outputPulse(slewRateHz));
    // if(isDecEn) digitalWrite(DO_PIN_DEC_PUL, Slew.outputPulse(slewRateHz));

    }
    // io::outputUpdate();
}
