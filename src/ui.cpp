#include <Arduino.h>
#include <avr/wdt.h>
// #include <LiquidCrystal_PCF8574.h>
#include <LiquidCrystal.h>
#include "ui.h"
#include "config.h"


/// @brief Functionality realated to the handheld controller buttons and LCD.
/// Could probably use a rewrite, was put together pretty quickly.
namespace ui{

    // LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display
    LiquidCrystal lcd(DO_LCD_RS, DO_LCD_E, DO_LCD_D4, DO_LCD_D5, DO_LCD_D6, DO_LCD_D7);
    ////////////////////////////////////////////////////////////////////////////
    // HandheldController
    ////////////////////////////////////////////////////////////////////////////
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
        btnRaPlus = !digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, HIGH);
        // digitalWrite(addrB, LOW);
        // digitalWrite(addrC, LOW);
        delayMicroseconds(debounceMicros);
        btnRaMinus = !digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, LOW);
        digitalWrite(addrB, HIGH);
        // digitalWrite(addrC, LOW);
        delayMicroseconds(debounceMicros);
        btnDecPlus = !digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, HIGH);
        // digitalWrite(addrB, HIGH);
        // digitalWrite(addrC, LOW);
        delayMicroseconds(debounceMicros);
        btnDecMinus = !digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, LOW);
        digitalWrite(addrB, LOW);
        digitalWrite(addrC, HIGH);
        delayMicroseconds(debounceMicros);
        btnMenu = !digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, HIGH);
        // digitalWrite(addrB, LOW);
        // digitalWrite(addrC, HIGH);
        delayMicroseconds(debounceMicros);
        btnAutoMan = !digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, LOW);
        digitalWrite(addrB, HIGH);
        // digitalWrite(addrC, HIGH);
        delayMicroseconds(debounceMicros);
        btnGoTo = !digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        digitalWrite(addrA, HIGH);
        // digitalWrite(addrB, HIGH);
        // digitalWrite(addrC, HIGH);
        delayMicroseconds(debounceMicros);
        btnTrack = !digitalRead(pinSig);
        delayMicroseconds(debounceMicros);
        updateEdges();
    }

    void HandheldController::updateEdges(){
        static bool prevBtnRaMinus = false;
        static bool prevBtnRaPlus = false;
        static bool prevBtnDecMinus = false;
        static bool prevBtnDecPlus = false;
        static bool prevBtnMenu = false;
        static bool prevBtnAutoMan = false;
        static bool prevBtnGoTo = false;
        static bool prevBtnTrack = false;

        if (btnRaMinus & !prevBtnRaMinus) btnRaMinusEdgeR = true;
        else btnRaMinusEdgeR = false;
        if (!btnRaPlus & prevBtnRaPlus) btnRaPlusEdgeR = true;
        else btnRaPlusEdgeR = false;
        if (btnDecMinus & !prevBtnDecMinus) btnDecMinusEdgeR = true;
        else btnDecMinusEdgeR = false;
        if (!btnDecPlus & prevBtnDecPlus) btnDecPlusEdgeR = true;
        else btnDecPlusEdgeR = false;
        if (btnMenu & !prevBtnMenu) btnMenuEdgeR = true;
        else btnMenuEdgeR = false;
        if (btnAutoMan & !prevBtnAutoMan) btnAutoManEdgeR = true;
        else btnAutoManEdgeR = false;
        if (btnGoTo & !prevBtnGoTo) btnGoToEdgeR = true;
        else btnGoToEdgeR = false;
        if (btnTrack & !prevBtnTrack) btnTrackEdgeR = true;
        else btnTrackEdgeR = false;

        prevBtnRaMinus = btnRaMinus;
        prevBtnRaPlus = btnRaPlus;
        prevBtnDecMinus = btnDecMinus;
        prevBtnDecPlus = btnDecPlus;
        prevBtnMenu = btnMenu;
        prevBtnAutoMan = btnAutoMan;
        prevBtnGoTo = btnGoTo;
        prevBtnTrack = btnTrack;
    }
    

    ////////////////////////////////////////////////////////////////////////////
    // HandheldDisplay
    ////////////////////////////////////////////////////////////////////////////
void Display::init(){
    byte upArrow[8] =   {0b00000, 0b00100, 0b01110, 0b10101,
                        0b00100, 0b00100, 0b00100, 0b00100};
    byte dnArrow[8] =   {0b00000, 0b00100, 0b00100, 0b00100,
                        0b00100, 0b10101, 0b01110, 0b00100};
    byte lfArrow[8] =   {0b00000, 0b00100, 0b01000, 0b11111,
                        0b01000, 0b00100, 0b00000, 0b00000};
    byte rtArrow[8] =   {0b00000, 0b00100, 0b00010, 0b11111,
                        0b00010, 0b00100, 0b00000, 0b00000};
    byte enArrow[8] =   {0b00000, 0b00001, 0b00001, 0b00101,
                        0b01001, 0b11111, 0b01000, 0b00100};

    menuIdx = 0;
    dbgMenuIdx = 0;
    prevMenuIdx = 0;
    prevDbgMenuIdx = 0;
    mode = COORDS;
    prevMode = COORDS;

    // Wire.begin();
    // Wire.beginTransmission(0x27);
    // Wire.endTransmission();
    lcd.begin(16, 2);  // initialize the lcd
    lcd.createChar(1, upArrow);
    lcd.createChar(2, dnArrow);
    lcd.createChar(3, lfArrow);
    lcd.createChar(4, rtArrow);
    lcd.createChar(5, enArrow);
    // lcd.setBacklight(255);
    lcd.clear();
    lcd.home();
}

void Display::updateStates(HandheldController hhc, bool sync, bool home){
    syncState = sync;
    homeState = home;
    if (hhc.getBtnMenuRise()){
        if (mode != MENU_MAIN){
            lastMode = mode;
            mode = MENU_MAIN;
        }
        else mode = lastMode;
    }
    switch(mode){
    case MENU_MAIN:
        if (hhc.getBtnDecPlusRise()) menuIdx++;
        if (hhc.getBtnDecMinusRise()) menuIdx--;
        if (menuIdx < 0) menuIdx = 2;
        if (menuIdx > 2) menuIdx = 0;
        if (menuIdx == ITM_COORDS && hhc.getBtnRaPlusRise()) mode = COORDS;
        else if (menuIdx == ITM_SYNC_STATUS && hhc.getBtnRaPlusRise()) mode = SYNC;
        else if (menuIdx == ITM_DEBUG && hhc.getBtnRaPlusRise()) mode = MENU_DEBUG;
        break;
    case MENU_DEBUG:
        if (hhc.getBtnDecPlusRise()) dbgMenuIdx++;
        if (hhc.getBtnDecMinusRise()) dbgMenuIdx--;
        if (dbgMenuIdx < 0) dbgMenuIdx = 1;
        if (dbgMenuIdx > 1) dbgMenuIdx = 0;
        if (dbgMenuIdx == ITM_TEST_BTNS && hhc.getBtnRaPlusRise()) mode = BTN_TST;
        // else if (menuIdx == ITM_HORZ_LIM && hhc.getBtnRaPlusRise()) mode = SYNC;
        else if (dbgMenuIdx == ITM_REBOOT && hhc.getBtnRaPlusRise()) reboot();
        break;

    case SYNC:
        if (hhc.getBtnAutoManRise()){
            autoManState = !autoManState;
            tone(PWM_BZR,NOTE_A5,BEEP_TIME);

        } 
        if (hhc.getBtnTrackRise()){
            trackState = !trackState;
            tone(PWM_BZR,NOTE_D6,BEEP_TIME);

        }
        break;
    case COORDS:
        if (hhc.getBtnAutoManRise()){
            autoManState = !autoManState;
            tone(PWM_BZR,NOTE_A5,BEEP_TIME);
        } 
        if (hhc.getBtnTrackRise()){
            trackState = !trackState;
            tone(PWM_BZR,NOTE_D6,BEEP_TIME);

        }
        if (!syncState) mode = SYNC;
        break;
    }
}

    void Display::show(HandheldController hhc, pos::Position pos, bool homing){
        // static bool prevSyncState = false;
        static bool prevMenuState = false;
        static int8_t prevMenuIdx = 0;
        switch (mode){
            case MENU_MAIN:
                if (prevMode != MENU_MAIN || prevMenuIdx != menuIdx) lcd.clear();
                showMenu();
            break;
            case MENU_DEBUG:
                if (prevMode != MENU_DEBUG || prevDbgMenuIdx != dbgMenuIdx) lcd.clear();
                showMenuDebug();
            break;
            case COORDS:
                if (prevMode != COORDS) lcd.clear();
                showCoords(pos.ra, pos.dec);
                showAutoManState();
                showTrackState(homing);
            break;
            case SYNC:
                if (prevMode != SYNC) lcd.clear();
                showSyncHome();
                showAutoManState();
                showTrackState(homing);
                break;
            case BTN_TST:
                if (prevMode != BTN_TST) lcd.clear();
                testButtons(hhc);
            break;
        }
        prevMenuIdx = menuIdx;
        prevDbgMenuIdx = dbgMenuIdx;
        prevMode = mode;
    }

void Display::showSyncHome(){
    lcd.setCursor(0, 0);
    if(syncState) lcd.print("SYNCED    ");
    else lcd.print("NOT SYNCED");
    lcd.setCursor(0, 1);
    if(homeState) lcd.print("HOME    ");
    else lcd.print("NOT HOME");
}

void Display::showMenu(){
    String options[3] = {"COORDS", "SYNC STATUS", "DEBUG"};
    // lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MENU:");
    lcd.setCursor(15,0);
    lcd.print("\01");
    lcd.setCursor(0, 1);
    lcd.print(options[menuIdx] + " \04");
    lcd.setCursor(15,1);
    lcd.print("\02");
    }

void Display::showMenuDebug(){
    String options[2] = {"TEST BUTTONS", "REBOOT"};
    // lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DEBUG:");
    lcd.setCursor(15,0);
    lcd.print("\01");
    lcd.setCursor(0, 1);
    lcd.print(options[dbgMenuIdx] + " \04");
    lcd.setCursor(15,1);
    lcd.print("\02");
    }


void Display::showAutoManState(){
    if (autoManState){
        lcd.setCursor(12, 0);
        lcd.print("|AUT");
    }
    else{
        lcd.setCursor(12, 0);
        lcd.print("|MAN");
    }
}
void Display::showTrackState(bool homing){
    if (homing){
        lcd.setCursor(12, 1);
        lcd.print("|HOM");
    }
    else if (trackState){
        lcd.setCursor(12, 1);
        lcd.print("|TRK");
    }
    else{
        lcd.setCursor(12, 1);
        lcd.print("|   ");
    }
}

void Display::showCoords(double ra, double dec){
    lcd.setCursor(0, 0);
    lcd.print("RA: ");
    lcd.print(double2RaStr(ra));
    lcd.setCursor(0, 1);
    lcd.print("DE:");
    lcd.print(double2DecStr(dec));
}


void Display::reboot(){
    wdt_disable();
    wdt_enable(WDTO_15MS);
    while (1) {}
}
/// @brief check functionality of buttons and potentiometer.
void Display::testButtons(HandheldController hhc){
    hhc.updateButtons();
        lcd.home();
        // lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ButtonTest");
        lcd.setCursor(0, 1);
        lcd.print(hhc.getBtnRaMinus());
        lcd.print(hhc.getBtnRaPlus());
        lcd.print(hhc.getBtnDecPlus());
        lcd.print(hhc.getBtnDecMinus());
        lcd.print(hhc.getBtnMenu());
        lcd.print(hhc.getBtnAutoMan());
        lcd.print(hhc.getBtnGoTo());
        lcd.print(hhc.getBtnTrack());
        lcd.print(" ");
        lcd.print(hhc.getPotValue());
        lcd.print(" ");
        // delay(5);

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
    ////////////////////////////////////////////////////////////////////////////
}