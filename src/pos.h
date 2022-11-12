#ifndef HAS_POS
#define HAS_POS
#include "pinConfig.h"
#include "enums.h"

enum frame{
    BASE,
    SKY,
    ALTAZ,
    MOTOR
};

/// @brief the 'pos' namespace contains all the functions and variables related
/// coordinates and positioning of the telescope.
namespace pos {

    /// @brief  The Latitude of the Observatory in degrees.
    const double lat = -37.77367;

    /// @brief A Position is a combination of two coordinates, each in decimal
    /// degrees. The unions allow for different coordinate names to be used 
    /// interchangeably.
    struct Position {
        frame frame;
        union  {
                double coord1, ra, az, lha, degRa;
        };
        union {
            double coord2, dec, alt, degDec;
        };
    };

    /// @brief Stores Position coordinates for a single position across multiple
    /// reference frames, and provides methods to convert between them.
    class FrameSet {
    public:
        FrameSet();
        double getCoord(frame frame, int coord);
        double __GET_SIDEREAL_TIME(){return siderealTime;}
        Position getPosition(frame frame);
        void incrementCoord(frame frame, int coordType, double coordValue);
        void syncTo(Position targetSky);
        void updateCoord(frame frame, int coordType, double coordValue);
        void updatePosition(Position newPos);
        void updatePosition(frame frame, double coord1, double coord2);
        void updateSiderealTime(double siderealTime);
        void __SET_POSITION(frame frame, double coord1, double coord2); // for testing only, disable in production
         /// @brief this setter should only be used on initial sync during startup.
         /// otherwise use the updateSiderealTime() method to ensure that base
         /// coordinates are updated. 
        void initialiseSiderealTime(double siderealTime){this->siderealTime = siderealTime;}
    private:
        double siderealTime;
        Position altaz;
        Position base;
        Position home;
        Position motor;
        Position sky;
        void altaz2Base();
        void base2Altaz();
        void base2Motor();
        void base2Sky();
        void motor2Base();
        void sky2Base();
        //void base2Dome();
        //void dome2Base();
    };

    /// @brief Singleton pattern controls access to siderealTime value. This value 
    /// determines the conversion between base and celestial frames
    class SiderealTime{
    public:
        static SiderealTime& getInstance(){
            static SiderealTime instance;
            return instance;
        }

        static double getValue(){return getInstance()._getValue();}
        static void sync(Position base, Position sky){getInstance()._sync(base, sky);}
        static void sync(FrameSet location){getInstance()._sync(location);}
        static void update(){return getInstance()._update();}

    private:
        double valueDeg;
        
        double _getValue(){return valueDeg;}
        void _sync(Position base, Position sky);
        void _sync(FrameSet location);
        void _update();
        SiderealTime(){}
    };
}

#endif  // HAS_POS
