
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



double trackRateHz = 125;
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
stepperCalibration raCal = {29918.22352,-0.4805,32558};
stepperCalibration decCal = {99603.48705,-1.2116,74717};
////////////////////////////////////////////////////////////////////////////////
/// Main Program
////////////////////////////////////////////////////////////////////////////////

namespace pos{
    const Position homePosition = {BASE, 0.00, 47.2536};
    Position targetPosition = {SKY, 0, 0};
    FrameSet currentLocation;
}
namespace ctrl{
    autoManualMode state = MANUAL;
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
}


void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);
    io::setupPinModes();


    raStp.init(DO_RA_STP_DIR, PWM_RA_STP_PUL, maxFreq, raCal);
    decStp.init(DO_DEC_STP_DIR, PWM_DEC_STP_PUL, maxFreq, decCal);

    // attachInterrupt(digitalPinToInterrupt(DI_PIN_DEC), declination_Stop, CHANGE);
    // attachInterrupt(digitalPinToInterrupt(DI_PIN_RA), RA_Home, CHANGE);

  // ctrl::waitForSync();

}

  // command currentCmd;
void loop() {

    pos::SiderealTime::update(); // Update the sidereal time
    pos::currentLocation.updateSiderealTime(pos::SiderealTime::getValue()); // Pass the sidereal time to the current location
    pos::currentLocation.updatePosition(io::getMotorPositions(raStp, decStp)); // Update the current location from the motor positions
    ctrl::state = (digitalRead(DI_MODE) == LOW) ? AUTO : MANUAL;
    // io::inputUpdate();

    // if (g_isSlewing) ctrl::simSlew(pos::currentLocation, pos::targetPosition);

    // switch(ctrl::state){
    // case AUTO:
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
            switch (ctrl::state){
                case AUTO:
                comms::sendReply(ctrl::checkTargetReachable(pos::targetPosition));
                ctrl::move(pos::currentLocation, pos::targetPosition,raStp, decStp);
                // g_isSlewing = true;
                break;
                case MANUAL:
                comms::sendReply("2: Telescope in Manual mode");
                break;
            }
            buffer = "";
            break;

            case STOP_SLEW:
            switch (ctrl::state){
                case AUTO:
                ctrl::stopAllMovement();
                break;
                case MANUAL:
                break;
            }
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
    // break;
    // case MANUAL:
    // default:
    if(ctrl::state == MANUAL){
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
            raStp.run(raDir, slewRateHz/2);
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
            Serial1.println("RA: " + String(raStp.getStepCount()) + " DEC: " + String(decStp.getStepCount()) + " MODE: " + String(digitalRead(DI_MODE)));
        }
    }
    // io::outputUpdate();
}
