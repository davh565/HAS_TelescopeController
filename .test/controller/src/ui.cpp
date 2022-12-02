#include <Arduino.h>
#include <LiquidCrystal_PCF8574.h>
#include "ui.h"

namespace ui{

    LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

    
    void HandheldController::initButtons(uint8_t pinPot, uint8_t pinSig, uint8_t addrA, uint8_t addrB, uint8_t addrC){
        this->pinPot = pinPot;
        this->pinSig = pinSig;
        this->addrA = addrA;
        this->addrB = addrB;
        this->addrC = addrC;

        pinMode(pinPot, INPUT);
        pinMode(pinSig, INPUT_PULLUP);
        pinMode(addrA, OUTPUT);
        pinMode(addrB, OUTPUT);
        pinMode(addrC, OUTPUT);
    }

    void HandheldController::updateButtons(){
        // Read potentiometer
        potValue = analogRead(pinPot);

        // Read buttons
        digitalWrite(addrA, LOW);
        digitalWrite(addrB, LOW);
        digitalWrite(addrC, LOW);
        delayMicroseconds(debounceMicros);
        btnRaPlus = digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, HIGH);
        // digitalWrite(addrB, LOW);
        // digitalWrite(addrC, LOW);
        delayMicroseconds(debounceMicros);
        btnRaMinus = digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, LOW);
        digitalWrite(addrB, HIGH);
        // digitalWrite(addrC, LOW);
        delayMicroseconds(debounceMicros);
        btnDecPlus = digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, HIGH);
        // digitalWrite(addrB, HIGH);
        // digitalWrite(addrC, LOW);
        delayMicroseconds(debounceMicros);
        btnDecMinus = digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, LOW);
        digitalWrite(addrB, LOW);
        digitalWrite(addrC, HIGH);
        delayMicroseconds(debounceMicros);
        btnMenu = digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, HIGH);
        // digitalWrite(addrB, LOW);
        // digitalWrite(addrC, HIGH);
        delayMicroseconds(debounceMicros);
        btnAutoMan = digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, LOW);
        digitalWrite(addrB, HIGH);
        // digitalWrite(addrC, HIGH);
        delayMicroseconds(debounceMicros);
        btnGoTo = digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, HIGH);
        // digitalWrite(addrB, HIGH);
        // digitalWrite(addrC, HIGH);
        delayMicroseconds(debounceMicros);
        btnTrack = digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
    }

    void HandheldController::updateStates(bool& autoMan, bool& track){
        static bool prevBtnAutoMan = false;
        static bool prevBtnTrack = false;
        if (btnAutoMan && !prevBtnAutoMan){
            autoMan = !autoMan;
        }
        if (btnTrack && !prevBtnTrack){
            track = !track;
        }
        prevBtnAutoMan = btnAutoMan;
        prevBtnTrack = btnTrack;
    }
    

void Display::init(){
    Wire.begin();
    Wire.beginTransmission(0x27);
    Wire.endTransmission();
    lcd.begin(16, 2);  // initialize the lcd
    lcd.setBacklight(255);
    lcd.clear();
    lcd.home();
}


void Display::updateAutoManState(bool autoMan){
    if (autoMan){
        lcd.setCursor(12, 0);
        lcd.print("|AUT");
    }
    else{
        lcd.setCursor(12, 0);
        lcd.print("|MAN");
    }
}
void Display::updateTrackState(bool track){
    if (track){
        lcd.setCursor(12, 1);
        lcd.print("|TRK");
    }
    else{
        lcd.setCursor(12, 1);
        lcd.print("|   ");
    }
}

void Display::updateCoords(double ra, double dec){
    lcd.setCursor(0, 0);
    lcd.print("RA: ");
    lcd.print(double2RaStr(ra));
    lcd.setCursor(0, 1);
    lcd.print("DE:");
    lcd.print(double2DecStr(dec));
}

/// @brief Convert a decimal angle in degees to a string of the form "HH:MM:SS" for RA.
/// @param raDegrees    The angle in degrees.
/// @return The string representation of the angle.
    String Display::double2RaStr(double raDegrees){
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

/// @brief Converts an angle in decimal degrees to the form "DD°MM'SS" for DEC.
/// @param decDegrees   The angle in degrees.
/// @return The string representation of the angle.
    String Display::double2DecStr(double decDegrees){
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
        outStr = signStr + degStr + char(223) + minuteStr + "'" + secondStr;

        return outStr;
    }
}