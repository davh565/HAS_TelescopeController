#include <avr/wdt.h>
#include "src/config.h"
#include "src/comms.h"
#include "src/ctrl.h"
#include "src/pos.h"
#include "src/io.h"
#include "src/stepper.h"
#include "src/ui.h"
#include "src/utils.h"

//TODO: move variables to appropriate modules. Ideally only setup and loop should be in this file
String buffer ="";
command currentCmd;
String coordString;
// bool g_isSlewing = false;
bool initialSync = false;
uint32_t maxFreqRa = 30000;
uint32_t maxFreqDec = 50000;
stepperCalibration raCal = {29918.22352,-0.4805,32558};
stepperCalibration decCal = {99603.48705,-1.2116,74717};
namespace pos{
    const Position homePosition = {BASE, 0.00, 47.2536};
    Position targetPosition = {SKY, 0, 0};
    FrameSet currentLocation;
}

namespace ctrl{
    autoManualMode ctrlMode;
    trackMode trkMode;

}
io::Stepper raStp;
io::Stepper decStp;
ui::HandheldController hhc;
ui::Display disp;
////////////////////////////////////////////////////////////////////////////////
/// Main Program
////////////////////////////////////////////////////////////////////////////////


/// @brief Main program entry point.
void setup() {
    io::setupLimits();
    ctrl::ctrlMode = MANUAL;
    ctrl::trkMode = NO_TRACK;
    hhc.initButtons(AI_POT_SPEED, DI_MUX_SIG, DO_MUX_ADDR_A, DO_MUX_ADDR_B, DO_MUX_ADDR_C);
    disp.init();
    Serial.begin(9600);
    // Serial1.begin(9600);
    wdt_enable(WDTO_8S); // Enable Watchdog Timer, 8s


    raStp.init(DO_RA_STP_DIR, PWM_RA_STP_PUL, maxFreqRa, false, raCal);
    decStp.init(DO_DEC_STP_DIR, PWM_DEC_STP_PUL, maxFreqDec, true, decCal);

}

/// @brief Main loop
void loop() {

    pos::SiderealTime::update(); // Update the sidereal time
    pos::currentLocation.updateSiderealTime(pos::SiderealTime::getValue()); // Pass the sidereal time to the current location
    pos::currentLocation.updatePosition(io::getMotorPositions(raStp, decStp)); // Update the current location from the motor positions
    hhc.updateButtons();
    disp.updateStates(hhc,true,true);
    disp.show(hhc,pos::currentLocation.getPosition(SKY));
    ctrl::ctrlMode = (disp.getAutoManState()) ? AUTO : MANUAL;
    ctrl::trkMode = (disp.getTrackState()) ? TRACK : NO_TRACK;

    if (comms::readStringUntilChar(buffer, '#')) {
        currentCmd = comms::parseCommand(buffer);
        switch (currentCmd) {
            case GET_RA:
            comms::sendReply(comms::double2RaStr(pos::currentLocation.getCoord(SKY, RA)));
            buffer = "";
            break;

            case GET_DEC:
            comms::sendReply(comms::double2DecStr(pos::currentLocation.getCoord(SKY, DECL)));
            buffer = "";
            break;

            case SYNC_POSITION:
            if (!initialSync){
                pos::SiderealTime::sync(pos::homePosition, pos::targetPosition);
                pos::currentLocation.initialiseSiderealTime(pos::SiderealTime::getValue());
                initialSync = true;
            }
            pos::currentLocation.syncTo(pos::targetPosition);

            comms::sendReply("SYNCED TO " + 
                            comms::double2RaStr(pos::currentLocation.getCoord(SKY, RA)) + " " +
                            comms::double2DecStr(pos::currentLocation.getCoord(SKY, DECL)));
            buffer = "";
            break;

            case SLEW_TO_TARGET:
            switch (ctrl::ctrlMode){
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
            switch (ctrl::ctrlMode){
                case AUTO:
                ctrl::stopAllMovement(raStp, decStp);
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
    
    }

    if(ctrl::ctrlMode == MANUAL){
        double slewRateHz = 0.095*hhc.getPotValue()*hhc.getPotValue(); //quadradic curve allows for fine control at low end, while still allowing fast slew at high end
        // digitalWrite(DO_RA_EN,isRaPul);
        // digitalWrite(DO_DEC_EN,isDecPul);
        if(hhc.getBtnRaPlus()){
            raStp.run(REVERSE, slewRateHz/2);
        }
        else if(hhc.getBtnRaMinus()){
            raStp.run(FORWARD, slewRateHz/2);
        }
        else if(ctrl::trkMode == TRACK){
            raStp.run(FORWARD, ctrl::trackRateHz);
        }
        else{
            raStp.stop();
        }

        if(hhc.getBtnDecPlus()){
            decStp.run(FORWARD, slewRateHz);
        }
        else if(hhc.getBtnDecMinus()){
            decStp.run(REVERSE, slewRateHz);
        }
        else{
            decStp.stop();
        }
    }
    
    // static unsigned long prevMillis = millis();
    // if(millis()-prevMillis>=500){
    //     // Serial.println("AUTO: " + String(disp.getAutoManState()));
    //     // Serial.println("TRACK: " + String(disp.getTrackState()));
    //     // Serial.println("RUN_RA: " + String(raStp.getEnabled()));
    //     // Serial.println("RUN_DEC: " + String(decStp.getEnabled()));
    //     Serial.println("RUN_DEC: " + String(hhc.getBtnDecMinus()));
    //     prevMillis = millis();
    // }

    // Serial.println(disp.getAutoManState());
    // // // // // // // // // SAFTEY LIMITS // // // // // // // // // // // //
    // ctrl::horizonStop(pos::currentLocation, raStp, decStp, ctrl::ctrlMode);
    io::limitStop(decStp); //Should be last function called in loop to ensure limit switches will stop motors
    // // // // // // // // // // // // // // // // // // // // // // // // // /
    wdt_reset();
}
