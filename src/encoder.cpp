#include "encoder.h"
#include "enums.h"



namespace io{

    Encoder::Encoder(int pinA, int pinB, int maxEdges){
        this->pinA = pinA;
        this->pinB = pinB;
        edgesPerRev = maxEdges;
        degsPerEdge = 360/double(edgesPerRev);
        A = digitalRead(pinA);
        B = digitalRead(pinB);
        _A = A;
        _B = B;
        deg = 0.0;
        pulseCount = 0;
        edgeCount = 0;
        revCount = 0;
    }

    void Encoder::update(){
        _A = A; //last A value
        _B = B; //last B value
        A = digitalRead(pinA); // current A value
        B = digitalRead(pinB); // current B value

        /// Detect direction based on current and previous edges
        if(     !_A && !_B &&  A && !B ||
                !_A &&  _B && !A && !B ||
                _A && !_B &&  A &&  B ||
                _A &&  _B && !A &&  B) dir = FORWARD;
        else if(!_A && !_B && !A &&  B ||
                !_A &&  _B &&  A &&  B ||
                _A && !_B && !A && !B ||
                _A &&  _B &&  A && !B) dir = REVERSE;

        /// On pulse cycle (A leading edge if FWD, A falling edge if REV)
        if (!_A && A && dir == FORWARD) pulseCount += 1;
        else if (_A && !A && dir == REVERSE) pulseCount += -1;
        /// On any edge
        if ((_A != A) || (_B != B)){
            edgeCount += dir == FORWARD ? 1 : -1;
            deg += dir == FORWARD ? degsPerEdge : -1 * degsPerEdge;
        }
        /// reset counts on revolution.
        if (edgeCount >= edgesPerRev){
            edgeCount -= edgesPerRev;
            pulseCount -= edgesPerRev/4;
            revCount ++;
        } 
        else if (edgeCount <= -edgesPerRev) {
            edgeCount += edgesPerRev;
            pulseCount += edgesPerRev/4;
            revCount --;
        }
    }

    void Encoder::reset(){
        edgeCount = 0;
        pulseCount = 0;
        revCount = 0;
        deg = 0.0;
    }
    
};
////////////////////////////////////////////////////////////////////////////////
    // io::Encoder enc(3, 2, 2048);

    // void setup()
    //  {
    //     Serial.begin(9600);
    //     pinMode(2, INPUT_PULLUP);
    //     pinMode(3, INPUT_PULLUP);
    //  }

    // void loop()
    //  {
    //     static unsigned long currentMillis;
    //     static unsigned long prevMillis = 0;
    //     currentMillis = millis();
    //     enc.update();
    //     String message = "Pulse:" +
    //                     String(enc.getStepCount()) +
    //                     " Edge: " +
    //                     String(enc.getEdgeCount()) +
    //                     " Rev: " +
    //                     String(enc.getRevCount()) +
    //                     " Dir: " +
    //                     String(enc.getDirection() )+
    //                     " Deg: " +
    //                     String(enc.getDegrees() ) +
    //                     "\r";
    //     if(currentMillis - prevMillis >= 1000){

    //        Serial.print(message);
    //         prevMillis = currentMillis;
    //     }
    //  }
