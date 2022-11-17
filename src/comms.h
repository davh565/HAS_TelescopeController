#ifndef HAS_COMMS
#define HAS_COMMS
////////////////////////////////////////////////////////////////////////////////
///Serial Communication, command parsing
////////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include "enums.h"
/// @brief The comms namespace contains functionality for communicating with 
/// Stellarium via the Meade LX200 protocol.
namespace comms {
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