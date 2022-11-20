#include <Arduino.h>
#include "pos.h"
#include "utils.h"

namespace pos {

    /// @brief  get the position in the chosen frame
    Position FrameSet::getPosition(frame frame){
        switch (frame){
            case BASE:
                return base;
            case SKY:
                return sky;
            case ALTAZ:
                return altaz;
            case MOTOR:
                return motor;
            default:
                Serial1.println("ERROR: FrameSet::get() - invalid frame");
                return base;
        }
    }

    double FrameSet::getCoord(frame frame, int coord){
        switch (frame){
            case BASE:
                if (coord == COORD1) return base.coord1;
                else if (coord == COORD2) return base.coord2;
                else {
                    Serial1.println("ERROR: FrameSet::getCoord() - invalid coord");
                    return 0;
                }
            case SKY:
                if (coord == COORD1) return sky.coord1;
                else if (coord == COORD2) return sky.coord2;
                else {
                    Serial1.println("ERROR: FrameSet::getCoord() - invalid coord");
                    return 0;
                }
            case ALTAZ:
                if (coord == COORD1) return altaz.coord1;
                else if (coord == COORD2) return altaz.coord2;
                else {
                    Serial1.println("ERROR: FrameSet::getCoord() - invalid coord");
                    return 0;
                }
            case MOTOR:
                if (coord == COORD1) return motor.coord1;
                else if (coord == COORD2) return motor.coord2;
                else {
                    Serial1.println("ERROR: FrameSet::getCoord() - invalid coord");
                    return 0;
                }
            default:
                Serial1.println("ERROR: FrameSet::getCoord() - invalid frame");
                return 0;
        }
    }
    /// @brief  when the sidereal time is updated, sky coordinates must be recalculated
    void FrameSet::updateSiderealTime(double siderealTime){
        this->siderealTime = siderealTime;
        base2Sky();
    }

    /// @brief Set current position, and convert to other frames
    void FrameSet::updatePosition(Position newPos){
        switch (newPos.frame){
            case BASE:
                // Serial1.println("Error: Position should be updated from motor or sky frame only");
                base = newPos;
                base2Motor();
                base2Sky();
                base2Altaz();
                break;
            case MOTOR:
                motor = newPos;
                motor2Base();
                base2Sky();
                base2Altaz();
                break;
            case SKY:
                sky = newPos;
                sky2Base();
                base2Motor();
                base2Altaz();
                break;
            case ALTAZ:
                // Serial1.println("Error: Position should be updated from motor or sky frame only");
                altaz = newPos;
                altaz2Base();
                base2Motor();
                base2Sky();
                break;
            default:
                break;
        }
    }

    void FrameSet::updatePosition(frame frame, double coord1, double coord2){
        Position pos;
        pos.frame = frame;
        pos.coord1 = coord1;
        pos.coord2 = coord2;
        updatePosition(pos);
    }

    void FrameSet::updateCoord(frame frame, int coordType, double coordValue){
        Position pos;
        pos.frame = frame;
        switch (coordType){
            case COORD1:
                pos.coord1 = coordValue;
                pos.coord2 = getCoord(frame, COORD2);
                break;
            case COORD2:
                pos.coord1 = getCoord(frame, COORD1);
                pos.coord2 = coordValue;
                break;
            default:
                Serial1.println("ERROR: FrameSet::updateCoord() - invalid coordType");
                break;
        }
        updatePosition(pos);
    }

    void FrameSet::incrementCoord(frame frame, int coordType, double coordValue){
        Position pos;
        pos.frame = frame;
        switch (coordType){
            case COORD1:
                pos.coord1 = getCoord(frame, COORD1) + coordValue;
                pos.coord2 = getCoord(frame, COORD2);
                break;
            case COORD2:
                pos.coord1 = getCoord(frame, COORD1);
                pos.coord2 = getCoord(frame, COORD2) + coordValue;
                break;
            default:
                Serial1.println("ERROR: FrameSet::incrementCoord() - invalid coordType");
                break;
        }
        updatePosition(pos);
    }
    
    /// @brief sets the celestial coordinates of the current position to match
    /// the target position, convert to base frame, then syncronise home position
    void FrameSet::syncTo(Position targetSky){
        if (targetSky.frame != SKY) {
            Serial1.println("Error: target frame must be SKY");
        }
        else {
            sky = targetSky;
            sky2Base();
            home.lha = base.lha - motor.degRa;
            home.dec = base.dec - motor.degDec;
            base2Altaz();
        }
    }

    /// @brief The base frame local hour angle is equal to motor coordinates 
    /// plus a fixed offset determined by the physical home position of the 
    /// telescope.
    Position  FrameSet::motor2Base(Position motor, Position home){
        Position base;
        base.frame = BASE;
        base.lha = wrap360(motor.degRa + home.lha);
        base.dec = wrap180(motor.degDec + home.dec);
        return base;
    }
    Position FrameSet::base2Motor(Position base, Position home){
        Position motor;
        motor.frame = MOTOR;
        motor.degRa = wrap360(base.lha - home.lha);
        motor.degDec = wrap180(base.dec - home.dec);
        return motor;
    }

    /// @brief Conversion from local hour angle to right ascension, based on the
    /// current sidereal time.
    Position FrameSet::base2Sky(Position base, double siderealTime){
        Position sky;
        sky.frame = SKY;
        sky.ra = wrap360((siderealTime) - base.lha);
        sky.dec = wrap180(base.dec);
        return sky;
    }

    /// @brief Conversion from right ascension to local hour angle, based on the
    /// current sidereal time.
    Position FrameSet::sky2Base(Position sky, double siderealTime){
        Position base;
        base.frame = BASE;
        base.lha = wrap360((siderealTime) - sky.ra);
        base.dec = wrap180(sky.dec);
        return base;
    }

    Position FrameSet::base2Altaz(Position base, double lat){
        Position altaz;
        altaz.frame = ALTAZ;
        altaz.az = wrap360(atand2(sind(base.lha), cosd(base.lha) * sind(lat) - tand(base.dec) * cosd(lat))+180);
        altaz.alt = wrap180(asind(sind(lat) * sind(base.dec) + cosd(lat) * cosd(base.dec) * cosd(base.lha)));
        return altaz;
    }
    Position FrameSet::altaz2Base(Position altaz, double lat){
        Position base;
        base.frame = BASE;
        base.lha = wrap360(atand2(sind(wrap360(altaz.az-180)), cosd(wrap360(altaz.az-180)) * sind(lat) + tand(altaz.alt) * cosd(lat)));
        base.dec = wrap180(asind(sind(lat) * sind(altaz.alt) - cosd(lat) * cosd(altaz.alt) * cosd(wrap360(altaz.az-180))));
        return base;
    }
    

    FrameSet::FrameSet(){
        base.frame = BASE;
        base.lha = 0;
        base.dec = 0;
        motor.frame = MOTOR;
        motor.degRa = 0;
        motor.degDec = 0;
        sky.frame = SKY;
        sky.ra = 0;
        sky.dec = 0;
        altaz.frame = ALTAZ;
        altaz.alt = 0;
        altaz.az = 0;
        home.frame = BASE;
        home.lha = 0;
        home.dec = 0;

    }


///




    /// @brief the initial syncronisation must be done from a known location
    void SiderealTime::_sync(Position base, Position sky){
        valueDeg = wrap360((base.lha + sky.ra));
    }
    /// @brief sets the current sidreal time. This function assumes that the telescope
    /// is in the home position, and that stellaium is syncing to the default position on startup.
    void SiderealTime::_sync(FrameSet location){
        _sync(location.getPosition(BASE),location.getPosition(SKY));
    }

    /// @brief Updates the current sidreal time based on elapsed time since last update.
    /// should be called once per loop
    void SiderealTime::_update(){
        static unsigned long prevMillis = millis();
        unsigned long currentMillis = millis();
        unsigned long deltaMillis = currentMillis - prevMillis;
        if (deltaMillis >= 150){
            const double msPerSiderealDay = 1.160576e-8; // milliseconds

            double deltaDeg = deltaMillis * msPerSiderealDay * 360;
            // Serial1.println(valueDeg);
            valueDeg +=  wrap360(deltaDeg);
            // Serial.println(msPerSiderealDay*86164100,15);
            // Serial.println("deltaDeg: " + String(deltaDeg,10) + " valueDeg: " + String(valueDeg));
            prevMillis = currentMillis;
            
        }
    }
}