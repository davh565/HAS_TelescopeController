#ifndef HAS_ENC
#define HAS_ENC
#include <Arduino.h>

// #include "../HAS_TelescopeController.h"

namespace io{
    class Encoder{
    private:

        int pinA;
        int pinB;
        bool A;
        bool B;
        bool _A;
        bool _B;
        int pulseCount;
        int edgeCount;
        int revCount;
        int dir;
        int edgesPerRev;
        double degsPerEdge;
        double deg;

    public:
        Encoder(int pinA, int pinB, int maxEdges);
        int getStepCount(){return pulseCount;} 
        int getEdgeCount() {return edgeCount;}
        int getRevCount() {return revCount;}
        int getDirection() {return dir;}
        double getDegrees() {return deg;}
        void update();
        void reset();
    };   
};

#endif