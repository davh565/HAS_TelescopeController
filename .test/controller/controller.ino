// #include <Arduino.h>
#include <Wire.h>
#include "src/config.h"
#include "src/ui.h"


bool autoMan = false;
bool track = false;
ui::HandheldController hhc;
ui::Display disp;



void setup(){
    Serial.begin(9600);
    hhc.initButtons(AI_POT_SPEED, DI_MUX_SIG, DO_MUX_ADDR_A, DO_MUX_ADDR_B, DO_MUX_ADDR_C);
    disp.init();
    // lcd.createChar(1, dotOff);
    // lcd.createChar(2, dotOn);
}

    void loop(){
        hhc.updateButtons();
        disp.updateStates(hhc,false,true);
        disp.show();
        // hhc.updateStates(autoMan, track, disp.menuState);

        // buttonTest(lcd,hhc);
        


    }




    // int show = -1;

    // // 2 custom characters

    // byte dotOff[] = { 0b00000, 0b01110, 0bdebounceTime01, 0bdebounceTime01,
    //                   0bdebounceTime01, 0b01110, 0b00000, 0b00000 };
    // byte dotOn[] = { 0b00000, 0b01110, 0b11111, 0b11111,
    //                  0b11111, 0b01110, 0b00000, 0b00000 };

    // void setup() {
    //   // int error;

    //   Serial.begin(115200);
    //   // Serial.println("LCD...");

    //   // // wait on Serial to be available on Leonardo
    //   // while (!Serial)
    //   //   ;

    //   // Serial.println("Probing for PCF8574 on address 0x27...");

    //   // // See http://playground.arduino.cc/Main/I2cScanner how to test for a I2C device.
    //   // Wire.begin();
    //   // Wire.beginTransmission(0x27);
    //   // error = Wire.endTransmission();
    //   // Serial.print("Error: ");
    //   // Serial.print(error);

    //   // if (error == 0) {
    //   //   Serial.println(": LCD found.");
    //   //   show = 0;
    //   //   lcd.begin(16, 2);  // initialize the lcd

    //   //   lcd.createChar(1, dotOff);
    //   //   lcd.createChar(2, dotOn);

    //   // } else {
    //   //   Serial.println(": LCD not found.");
    //   // }  // if

    //   //   lcd.setBacklight(255);
    // }  // setup()


    // void loop() {
    //     ////////////////////////
    //     byte data;
    //     for (byte i = 0; i < admux::mux.channelCount(); i++) {
    //         data = admux::mux.read(i) /* Reads from channel i (returns HIGH or LOW) */;
    //         Serial.print("Push button at channel "); Serial.print(i); Serial.print(" is "); Serial.println(data == LOW ? "pressed" : "not pressed");
    //       // delay(debounceTime);
    //     }
    //     Serial.println();
    //     delay(debounceTime0);


    //     /////////////////////////
    // //   if (show == 0) {
    // //     lcd.home();
    // //     lcd.clear();
    // //     lcd.print(" 14:39:37   |AUT");
    // //     lcd.setCursor(0, 1);
    // //     lcd.print("-60");
    // //     lcd.print((char)223);
    // //     lcd.print("debounceTime'02   |TRK");
    // //     delay(debounceTime00);

    // // //    lcd.setBacklight(0);
    // // //    delay(400);
    // //     // lcd.setBacklight(128);

    // //   } else if (show == 1) {
    // //     // lcd.setBacklight(0);
    // //     lcd.home();
    // //     lcd.clear();
    // //     lcd.print(" 14:39:37   |MAN");
    // //     lcd.setCursor(0, 1);
    // //     lcd.print("-60");
    // //     lcd.print((char)223);
    // //     lcd.print("debounceTime'02   |   ");
    // //     delay(debounceTime00);
    // //     lcd.clear();
    // //     lcd.print("Cursor On");
    // //     lcd.cursor();

    // //   } else if (show == 2) {
    // //     lcd.home();
    // //     lcd.clear();
    // //     lcd.print("NOT SYNCED  |MAN");
    // //     lcd.setCursor(0, 1);
    // //     lcd.print("NOT HOME    |   ");
    // //     delay(debounceTime00);

    // //   } else if (show == 3) {
    // //     lcd.clear();
    // //     lcd.print("Cursor OFF");
    // //     lcd.noBlink();
    // //     lcd.noCursor();

    // //   } else if (show == 4) {
    // //     lcd.clear();
    // //     lcd.print("Display Off");
    // //     lcd.noDisplay();

    // //   } else if (show == 5) {
    // //     lcd.clear();
    // //     lcd.print("Display On");
    // //     lcd.display();

    // //   } else if (show == 7) {
    // //     lcd.clear();
    // //     lcd.setCursor(0, 0);
    // //     lcd.print("*** first line.");
    // //     lcd.setCursor(0, 1);
    // //     lcd.print("*** second line.");

    // //   } else if (show == 8) {
    // //     lcd.scrollDisplayLeft();
    // //   } else if (show == 9) {
    // //     lcd.scrollDisplayLeft();
    // //   } else if (show == 10) {
    // //     lcd.scrollDisplayLeft();
    // //   } else if (show == 11) {
    // //     lcd.scrollDisplayRight();

    // //   } else if (show == 12) {
    // //     lcd.clear();
    // //     lcd.print("write-");

    // //   } else if (show == 13) {
    // //     lcd.clear();
    // //     lcd.print("custom 1:<\01>");
    // //     lcd.setCursor(0, 1);
    // //     lcd.print("custom 2:<\02>");

    // //   } else {
    // //     lcd.print(show - 13);
    // //   }  // if

    // // //   delay(1400);
    // // //   show = 0;
    // //   show = (show + 1) % 3;
    // }  // loop()