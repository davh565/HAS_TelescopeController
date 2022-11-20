#include "src/config.h"
#include "src/comms.h"
#include "src/pos.h"
// #include "src/ctrl.h"
// #include "src/io.h"
#include "src/utils.h"
// #include "src/stepper.h"
#include "testData.h"
// #include "src/pulse.h"

// use the following site for independent coordinate conversion to produce reference values
// https://www.swift.psu.edu/secure/toop/convert.htm

// PulseGenerator raPulse;


void testValue(String testName, double refValue, double testValue, double tolerance=0.05){
    Serial.print(testName+ ", ref: " + String(refValue) + " test: " + String(testValue)+ " ");
    Serial.println((abs(refValue- testValue) <= tolerance ? "PASS" : "FAIL"));
}

template <typename T, typename U>
void testValue(String testName,T refValue, U testValue){
    Serial.print(testName+ ", ref: " + String(refValue) + " test: " + String(testValue)+ " ");
    Serial.println((refValue == testValue) ? "PASS" : "FAIL");
}

void _testHeader(String testName){
        Serial.println("TESTING " + testName + ":");
        Serial2.println();
        // Serial.println("____________________________________________________");
}

void _testFooter(){
        Serial.println("____________________________________________________");
}

void _testDivider(){
        Serial.println("- - - - - - - - - - - - - - - - - - - - - - - - - - ");
}

void testWrap(){
        const double diffang1[] = { 354.00, 358.43};
    const double diffang2[] = { 358.00, 2.43};
    const double diffang3[] = { 2.00, 6.43};
    const double diffAns = 4.43;
    const double diffAns2 = 355.57;
    _testHeader("testWrap");
    testValue("wrap 360", 0, wrap360(0));
    testValue("wrap 360", 180, wrap360(180));
    testValue("wrap 360", 0, wrap360(360));
    testValue("wrap 360", diffAns, wrap360(diffang1[1]-diffang1[0]));
    testValue("wrap 360", diffAns, wrap360(diffang2[1]-diffang2[0]));
    testValue("wrap 360", diffAns, wrap360(diffang3[1]-diffang3[0]));
    testValue("wrap 360", diffAns2, wrap360(diffang1[0]-diffang1[1]));
    testValue("wrap 360", diffAns2, wrap360(diffang2[0]-diffang2[1]));
    testValue("wrap 360", diffAns2, wrap360(diffang3[0]-diffang3[1]));
    _testDivider();
    testValue("wrap 180", 0, wrap180(0));
    testValue("wrap 180", 180, wrap180(180));
    testValue("wrap 180", 180, wrap180(-180));
    testValue("wrap 180", diffAns, wrap180(diffang1[1]-diffang1[0]));
    testValue("wrap 180", diffAns, wrap180(diffang2[1]-diffang2[0]));
    testValue("wrap 180", diffAns, wrap180(diffang3[1]-diffang3[0]));
    testValue("wrap 180", -diffAns, wrap180(diffang1[0]-diffang1[1]));
    testValue("wrap 180", -diffAns, wrap180(diffang2[0]-diffang2[1]));
    testValue("wrap 180", -diffAns, wrap180(diffang3[0]-diffang3[1]));
    _testFooter();

}
namespace comms {

    void testDouble2RaStr(double inRa[],String refRaStr[]){
        _testHeader("double2RaStr");

        for (int i = 0; i < numRefs ; i++) {
            String raStr = double2RaStr(inRa[i]);
            testValue("double2RaStr", refRaStr[i], raStr);
        }
        _testFooter();
    }
    void testDouble2DecStr(double inDec[], String refDecStr[]){
        _testHeader("double2DecStr");

        for (int i = 0; i < numRefs ; i++) {
            String decStr = double2DecStr(inDec[i]);
            testValue("double2DecStr", refDecStr[i], decStr);
        }
        _testFooter();
    }

    void testRaStr2Double(String inRaStr[], double refRa[]){
        _testHeader("raStr2Double");

        for (int i = 0; i < numRefs; i++) {
            double raDeg = raStr2Double(inRaStr[i]);
            testValue("raStr2Double", refRa[i], raDeg);
        }
        _testFooter();
    }

    void testDecStr2Double(String inDecStr[], double refDec[]){
        _testHeader("decStr2Double");

        for (int i = 0; i < numRefs; i++) {
            double decDeg = decStr2Double(inDecStr[i]);
            testValue("decStr2Double", refDec[i], decDeg);
        }
        _testFooter();
    }

}

namespace pos{
    // direct setting of FrameSet Values. For testing only. disable in production
    void FrameSet::__SET_POSITION(frame frame, double coord1, double coord2){
        switch (frame){
            case BASE:
            base.lha = coord1;
            base.dec = coord2;
            break;
            case MOTOR:
            motor.degRa = coord1;
            motor.degDec = coord2;
            break;
            case SKY:
            sky.ra = coord1;
            sky.dec = coord2;
            break;
            case ALTAZ:
            altaz.az = coord1;
            altaz.alt = coord2;
            break;
            default:
            break;

        }
    }
        Position FrameSet::__ALTAZ_2_BASE(Position altaz, double siderealTime){return altaz2Base(altaz,lat);} //For Testing Only
        Position FrameSet::__BASE_2_ALTAZ(Position base, double lat          ){return base2Altaz(base,lat);} //For Testing Only
        Position FrameSet::__BASE_2_MOTOR(Position base, Position home       ){return base2Motor(base,home);} //For Testing Only
        Position FrameSet::__BASE_2_SKY  (Position base, double siderealTime ){return base2Sky(base,siderealTime);} //For Testing Only
        Position FrameSet::__MOTOR_2_BASE(Position motor, Position home      ){return motor2Base(motor,home);} //For Testing Only
        Position FrameSet::__SKY_2_BASE  (Position sky, double siderealTime  ){return sky2Base(sky,siderealTime);} //For Testing Only
    
    void printFrameSet(FrameSet loc, String name = "FrameSet"){
        Serial.println(name + ":");
        Serial.println("base.lha: " + String(loc.getCoord(BASE, LHA)));
        Serial.println("base.dec: " + String(loc.getCoord(BASE, DECL)));
        Serial.println("sky.ra: " + String(loc.getCoord(SKY, RA)));
        Serial.println("sky.dec: " + String(loc.getCoord(SKY, DECL)));
        Serial.println("altaz.az: " + String(loc.getCoord(ALTAZ, AZ)));
        Serial.println("altaz.alt: " + String(loc.getCoord(ALTAZ, ALT)));
        Serial.println("motor.degRa: " + String(loc.getCoord(MOTOR, DEG_RA)));
        Serial.println("motor.degDec: " + String(loc.getCoord(MOTOR, DEG_DEC)));
        Serial.println("siderealTime(deg): " + String(loc.__GET_SIDEREAL_TIME()));
        _testDivider();
    }

    void TEST_FRAMESET(){
        const Position homePos = {BASE, 0.00, 5.00};
        // const Position syncPos = {SKY, 0.00, 5.00};
        // const Position zeroPos = {SKY, 0.00, 0.00};
        double lat = -37.77367;
        
        const int numRefs = 8;
        const String refName[numRefs] = { "HIP 66958 ", "HIP 66958 ", "HIP 66958 ", "HIP 66958 ", "Polaris Australis", "Polaris Australis", "Jupiter", "Jupiter"};
        const double refSkyRa[] = { 206.08, 206.08, 206.08, 206.08, 322.03, 322.03, 03.92, 03.91};
        const double refSkyDec[] = { 45.41, 45.41, 45.41, 45.41, -88.87, -88.87, -00.07, -00.07};
        const double refBaseLha[] = { 359.79, 357.58, 01.57, 48.13, 204.07, 117.65, 75.76, 100.41};
        const double refBaseDec[] = { 45.41, 45.41, 45.41, 45.41, -88.87, -88.87, -00.07, -00.07};
        const double refAltazAz[] = { 00.15, 01.71, 358.89, 328.40, 179.42, 181.26, 278.79, 263.52};
        const double refAltazAlt[] = { 06.81, 06.78, 06.80, -03.79, 36.75, 37.25, 11.26, -08.16};
        const double refSiderealTime[] = { 205.87, 203.66, 207.66, 254.22, 166.10, 79.68, 79.68, 104.32};

        _testHeader("Frameset");
        // int idx = 7;
        FrameSet ref;
        Position tstSky;
        Position tstBase;
        Position tstAltaz;
        Position tstMotor;
        for (int idx = 0; idx < numRefs; idx++){
        ref.__SET_POSITION(SKY, refSkyRa[idx], refSkyDec[idx]);
        ref.__SET_POSITION(BASE, refBaseLha[idx], refBaseDec[idx]);
        ref.__SET_POSITION(ALTAZ, refAltazAz[idx], refAltazAlt[idx]);
        ref.updateSiderealTime(refSiderealTime[idx]);
        // printFrameSet(ref,"ref0");
            Serial.println("refName: " + refName[idx]);
            tstBase = ref.__SKY_2_BASE(ref.getPosition(SKY), refSiderealTime[idx]);
            testValue("SKY_2_BASE_LHA", refBaseLha[idx], tstBase.lha);
            testValue("SKY_2_BASE_DEC", refBaseDec[idx], tstBase.dec);
            tstSky = ref.__BASE_2_SKY(ref.getPosition(BASE), refSiderealTime[idx]);
            testValue("BASE_2_SKY_RA", refSkyRa[idx], tstSky.ra);
            testValue("BASE_2_SKY_DEC", refSkyDec[idx], tstSky.dec);
            tstAltaz = ref.__BASE_2_ALTAZ(ref.getPosition(BASE),lat);
            testValue("BASE_2_ALTAZ_AZ", refAltazAz[idx], tstAltaz.az);
            testValue("BASE_2_ALTAZ_ALT", refAltazAlt[idx], tstAltaz.alt);
            tstBase = ref.__ALTAZ_2_BASE(ref.getPosition(ALTAZ),lat);
            testValue("ALTAZ_2_BASE_LHA", refBaseLha[idx], tstBase.lha);
            testValue("ALTAZ_2_BASE_DEC", refBaseDec[idx], tstBase.dec);
            _testDivider();
        }

        _testFooter();
    }
}
// PWM_DEC_STP_PUL
// namespace io{

//     void TEST_STEPPER(){
//         _testHeader("Stepper");
//         uint32_t testFreq = 60000;
//         // raPulse.init(PWM_RA_STP_PUL, testFreq);
//         // raPulse.setTarget(10000);
//         // raPulse.enable();
//         // raPulse.setRunMode(CONSTANT);
//         // raPulse.setFrequency(testFreq);
//         // Serial.println(OCR4A);
//         // Serial.println(TCCR4B);

//         // raPulse.setFrequency(testFreq);
//         double raCal = 30742.88868;
//         double decCal = 99603.48705;
//         // // Serial.println("RA Stepper");
//         Stepper raStp(DO_RA_STP_DIR, PWM_RA_STP_PUL, testFreq, raCal);
//         Stepper decStp(DO_DEC_STP_DIR, PWM_DEC_STP_PUL, testFreq, decCal);
//         // motor.enable();
//         // motor.runPulses(-100000);
//         // delay(5000);
//         // motor.runPulses(-100000);
//         // delay(5000);
//         // motor.runPulses(-100000);
//         // delay(5000);
//         // motor.runToTarget(1000000);
//         // delay(2000);
//         // // motor.stop();
//         // motor.disable();
//         // delay(2000);
//         // motor.run(FORWARD,50000);
//         // delay(5000);
//         // motor.setFrequency(testFreq/2);
//         // raStp.runAngle(-69.5);
//         decStp.runAngle(15);
//         // motor.runPulses(-1000000);
//         // delay(10000);
//         // motor.stop();

//         // motor.enable();
//         // motor.setFrequency(testFreq/2);
//         // raPulse.disable();
//         // motor.disable();
//         _testFooter();
//     }
// }

void setup(){
    Serial.begin(9600);

    pinMode(5,OUTPUT);
    _testFooter();
    // io::TEST_STEPPER();
    testWrap();
    comms::testDouble2RaStr(refSkyRa,refSkyRaStr);
    comms::testDouble2DecStr(refSkyDec, refSkyDecStr);
    comms::testRaStr2Double(refSkyRaStr, refSkyRa);
    comms::testDecStr2Double(refSkyDecStr, refSkyDec);
    pos::TEST_FRAMESET();

}

void loop(){
    // Serial.println(raPulse.getCount());
    // delay(1000);
    
    // Serial.println(io::motor.getDirection());
    // io::motor.changeDirection();
    // Serial.println(REVERSE);
    // static bool tst = false;
    // digitalWrite(DO_RA_STP_DIR, tst);
    // tst = !tst;
}