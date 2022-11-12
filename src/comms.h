#ifndef HAS_COMMS
#define HAS_COMMS
////////////////////////////////////////////////////////////////////////////////
///Serial Communication, command parsing
////////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include "enums.h"

// #include "HAS_TelescopeController.h"
  // Commands that can be recieved from Stellarium

namespace comms {
    
    // class Buffer{
    // private:
    //   String buffer;
    // public:
    //   Buffer();
    //   String read(){return buffer;}
    //   void update(char c){buffer += c;}
    //   void clear(){buffer = "";}
    // };
    
    String extractCoord(String buffer);


    command parseCommand(String input);
    void sendReply(String message);
    bool readStringUntilChar(String& buffer, char terminator);
    String double2RaStr(double raDegrees);
    String double2DecStr(double decDegrees);
    double raStr2Double(String input);
    double decStr2Double(String input);

}


#endif //HAS_COMMS