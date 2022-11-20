#ifndef HAS_POS
#define HAS_POS
#include "config.h"
#include "enums.h"

/// @brief The coordinate frame of a position
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
        Position __ALTAZ_2_BASE(Position altaz, double lat); //For Testing Only
        Position __BASE_2_ALTAZ(Position base, double lat          ); //For Testing Only
        Position __BASE_2_MOTOR(Position base, Position home       ); //For Testing Only
        Position __BASE_2_SKY  (Position base, double siderealTime ); //For Testing Only
        Position __MOTOR_2_BASE(Position motor, Position home      ); //For Testing Only
        Position __SKY_2_BASE  (Position sky, double siderealTime  ); //For Testing Only
        void __SET_POSITION(frame frame, double coord1, double coord2); // for testing only, disable in production
        double __GET_SIDEREAL_TIME(){return siderealTime;}
        FrameSet();
        double getCoord(frame frame, int coord);
        Position getPosition(frame frame);
        void incrementCoord(frame frame, int coordType, double coordValue);
        void syncTo(Position targetSky);
        void updateCoord(frame frame, int coordType, double coordValue);
        void updatePosition(Position newPos);
        void updatePosition(frame frame, double coord1, double coord2);
        void updateSiderealTime(double siderealTime);
        void initialiseSiderealTime(double siderealTime){this->siderealTime = siderealTime;}//this setter should only be
        // used on initial sync during startup. Otherwise use the updateSiderealTime() 
        //method to ensure that base coordinates are updated. 
    private:
        double siderealTime;
        Position altaz;
        Position base;
        Position home;
        Position motor;
        Position sky;
        Position altaz2Base(Position altaz, double lat);
        Position base2Altaz(Position base, double lat);
        Position base2Motor(Position base, Position home);
        Position base2Sky(Position base, double siderealTime);
        Position motor2Base(Position motor, Position home);
        Position sky2Base(Position sky, double siderealTime);
        void altaz2Base(){base = altaz2Base(altaz, siderealTime);}
        void base2Altaz(){altaz = base2Altaz(base, lat);}
        void base2Motor(){motor = base2Motor(base, home);}
        void base2Sky(){sky = base2Sky(base, siderealTime);}
        void motor2Base(){base = motor2Base(motor, home);}
        void sky2Base(){base = sky2Base(sky, siderealTime);}
        //void base2Dome();
        //void dome2Base();
    };

    /// @brief The Sidereal time determines the conversion between base and celestial frames.
    /// This class implements the Singleton pattern, as there should only ever be one instance
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
