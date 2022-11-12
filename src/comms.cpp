// #include <Arduino.h>
#include "comms.h"
// ////////////////////////////////////////////////////////////////////////////////
/// ///Serial Communication, command parsing
// ////////////////////////////////////////////////////////////////////////////////
namespace comms {

    command parseCommand(String input) {
        command c;
        if (input == ":GR#") c = GET_RA;
        else if (input == ":GD#") c = GET_DEC;
        else if (input == ":CM#") c = SYNC_POSITION;
        else if (input == ":MS#") c = SLEW_TO_TARGET;
        else if (input == ":Q#") c = STOP_SLEW;
        else if (input.startsWith(":Sr")) c = SET_TARGET_RA;
        else if (input.startsWith(":Sd")) c = SET_TARGET_DEC;
        else c = NO_CMD;
        return c;
    }

    /// TODO add message string checking
    void sendReply(String message) {
        Serial.print(message+"#");
        // Serial1.println("-->" + message+"#");
    }

    bool readStringUntilChar(String& buffer,char terminator) {
        while (Serial.available()) {
            char c = Serial.read();
            buffer += c;
            if (c == terminator) {
                // Serial1.println("<--" + buffer);
                return true;
            }
        }
        return false;
    }

/// Returns a string of the form "HH:MM:SS.SS" from a double representing RA
/// in degrees
    String double2RaStr(double raDegrees){
        double remHour;
        double remMinute;
        double hour;
        double minute;
        double second;
        int hourInt;
        int minuteInt;
        int secondInt;
        String hourStr;
        String minuteStr;
        String secondStr;
        String outStr;

        if (raDegrees < 0) raDegrees += 360;
        hour = raDegrees / 15;
        hourInt = trunc(hour);
        remHour = hour - hourInt;
        minute = 60 * remHour;
        minuteInt = trunc(minute);
        remMinute = minute - minuteInt;
        second = 60 * remMinute;
        secondInt = round(second);
        if (secondInt >= 60) {
            secondInt == 0;
            minuteInt++;
            if (minuteInt >= 60) {
                minuteInt = 0;
                hourInt++;
                if (hourInt >= 24) {
                    hourInt = hourInt - 24;
                }
            }
        }
        hourStr = hourInt < 10 ? "0" + String(hourInt) : hourInt;
        minuteStr = minuteInt < 10 ? "0" + String(minuteInt) : minuteInt;
        secondStr = secondInt < 10 ? "0" + String(secondInt) : secondInt;
        outStr = hourStr + ":" + minuteStr + ":" + secondStr;
        return outStr;
    }

/// Returns a string in the form "$DD*MM:SS" from a double representing DEC
    String double2DecStr(double decDegrees){
        double deg;
        double remDeg;
        double remMinute;
        double minute;
        double second;
        int degInt;
        int minuteInt;
        int secondInt;
        String degStr;
        String minuteStr;
        String secondStr;
        String signStr;
        String outStr;
        deg = abs(decDegrees);
        degInt = trunc(deg);
        remDeg = deg - degInt;
        minute = 60 * remDeg;
        minuteInt = trunc(minute);
        remMinute = minute - minuteInt;
        second = 60 * remMinute;
        secondInt = round(second);

        signStr = decDegrees < 0 ? '-' : '+';
        degStr = degInt < 10 ? "0" + String(degInt) : degInt;
        minuteStr = minuteInt < 10 ? "0" + String(minuteInt) : minuteInt;
        secondStr = secondInt < 10 ? "0" + String(secondInt) : secondInt;
        outStr = signStr + degStr + "*" + minuteStr + ":" + secondStr;

        return outStr;
    }
    //// Parse coordinate string from Stellarium command, return degrees as double
    double decStr2Double(String input) {
        int deg;
        int minute;
        int second;
        int8_t sign;
        double value; 
        // sDD*MM:SS  -string format for DEC
        // 012345678  -char index
            // Declination c;
            sign = input.charAt(0)=='-' ? -1 : 1;
            deg = input.substring(1, 3).toInt();
            minute = input.substring(4, 6).toInt();
            second = input.substring(7, 9).toInt();
            value = sign*(deg + (minute / 60.0) + (second / 3600.0));

        return value;
    }

    //// Parse coordinate string from Stellarium command, return degrees as double
    double raStr2Double(String input) {
        int hour; 
        int minute;
        int second;
        double value; 
        // HH:MM:SS  -string format for RA
        // 01234567  -char index
        hour = input.substring(0, 2).toInt();
        minute = input.substring(3, 5).toInt();
        second = input.substring(6, 8).toInt();
        value = 15 * (hour + (minute / 60.0) + (second / 3600.0));

        return value;
    }
    String extractCoord(String buffer){return buffer.substring(3,buffer.length()-1);}

}
