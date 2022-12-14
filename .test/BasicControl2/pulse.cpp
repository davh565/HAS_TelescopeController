#include <Arduino.h>
#include "pulse.h"

volatile long count1 = 0; // timer 1 pulse count 
volatile long count3 = 0; // timer 3 pulse count
volatile long count4 = 0; // timer 4 pulse count
volatile long count5 = 0; // timer 5 pulse count
long target1 = 0; // timer 1 target count 
long target3 = 0; // timer 3 target count
long target4 = 0; // timer 4 target count
long target5 = 0; // timer 5 target count
bool dir1 = FORWARD; // timer 1 direction
bool dir3 = FORWARD; // timer 3 direction
bool dir4 = FORWARD; // timer 4 direction
bool dir5 = FORWARD; // timer 5 direction
const uint32_t prescaler = 8;
const uint32_t clock = 16000000;
const uint32_t maxFrequency = clock / prescaler / 2;
// timer 1A interrupt service routine
ISR(TIMER1_COMPA_vect) {
    if (dir1 == FORWARD) {
        count1++;
        if (count1 >= target1 && target1 != 0){
            // Disable Timer
            bitWrite(TCCR1B, CS10, LOW);
            bitWrite(TCCR1B, CS11, LOW);
            bitWrite(TCCR1B, CS12, LOW);
            // digitalWrite(oc1a, LOW);
        }
    }
    else {
        count1--;
        if (count1 <= target1 && target1 != 0){
            // Disable Timer
            bitWrite(TCCR1B, CS10, LOW);
            bitWrite(TCCR1B, CS11, LOW);
            bitWrite(TCCR1B, CS12, LOW);
            // digitalWrite(oc1a, LOW);
        }
    }
}

// timer 3A interrupt service routine
ISR(TIMER3_COMPA_vect) {
    if (dir3 == FORWARD) {
        count3++;
        if (count3 >= target3 && target3 != 0){
            // Disable Timer
            bitWrite(TCCR3B, CS30, LOW);
            bitWrite(TCCR3B, CS31, LOW);
            bitWrite(TCCR3B, CS32, LOW);
            // digitalWrite(oc3a, LOW);
        }
    }
    else {
        count3--;
        if (count3 <= target3 && target3 != 0){
            // Disable Timer
            bitWrite(TCCR3B, CS30, LOW);
            bitWrite(TCCR3B, CS31, LOW);
            bitWrite(TCCR3B, CS32, LOW);
            // digitalWrite(oc3a, LOW);
        }
    }
}

// timer 4A interrupt service routine
ISR(TIMER4_COMPA_vect) {
    if (dir4 == FORWARD) {
        count4++;
        if (count4 >= target4 && target4 != 0){
            // Disable Timer
            bitWrite(TCCR4B, CS40, LOW);
            bitWrite(TCCR4B, CS41, LOW);
            bitWrite(TCCR4B, CS42, LOW);
            // digitalWrite(oc4a, LOW);
        }
    }
    else {
        count4--;
        if (count4 <= target4 && target4 != 0){
            // Disable Timer
            bitWrite(TCCR4B, CS40, LOW);
            bitWrite(TCCR4B, CS41, LOW);
            bitWrite(TCCR4B, CS42, LOW);
            // digitalWrite(oc4a, LOW);
        }
    }
}

// timer 5A interrupt service routine
ISR(TIMER5_COMPA_vect) {
    if (dir5 == FORWARD) {
        count5++;
        if (count5 >= target5 && target5 != 0){
            // Disable Timer
            bitWrite(TCCR5B, CS50, LOW);
            bitWrite(TCCR5B, CS51, LOW);
            bitWrite(TCCR5B, CS52, LOW);
            digitalWrite(oc5a, LOW);
        }
    }
    else {
        count5--;
        if (count5 <= target5 && target5 != 0){
            // Disable Timer
            bitWrite(TCCR5B, CS50, LOW);
            bitWrite(TCCR5B, CS51, LOW);
            bitWrite(TCCR5B, CS52, LOW);
            digitalWrite(oc5a, LOW);
        }
    }
}

long PulseGenerator::getCount(){
    if (pulsePin == oc1a) return count1;
    else if (pulsePin == oc3a) return count3;
    else if (pulsePin == oc4a) return count4;
    else if (pulsePin == oc5a) return count5;
    else return 0;
}
// Disable Timer by setting prescaler to 0
void PulseGenerator::disable(){
    switch(pulsePin){
        case oc1a:
            bitWrite(TCCR1B, CS10, LOW);
            bitWrite(TCCR1B, CS11, LOW);
            bitWrite(TCCR1B, CS12, LOW);
            break;
        case oc3a:
            bitWrite(TCCR3B, CS30, LOW);
            bitWrite(TCCR3B, CS31, LOW);
            bitWrite(TCCR3B, CS32, LOW);
            break;
        case oc4a:
            bitWrite(TCCR4B, CS40, LOW);
            bitWrite(TCCR4B, CS41, LOW);
            bitWrite(TCCR4B, CS42, LOW);
            break;
        case oc5a:
            bitWrite(TCCR5B, CS50, LOW);
            bitWrite(TCCR5B, CS51, LOW);
            bitWrite(TCCR5B, CS52, LOW);
            break;
    }
    // digitalWrite(pulsePin, LOW);
}
// Enable Timer by setting prescaler to 64
void PulseGenerator::enable(){
    switch(pulsePin){
        case oc1a:
            // bitWrite(TCCR1B, CS10, HIGH);
            bitWrite(TCCR1B, CS11, HIGH);
            // bitWrite(TCCR1B, CS12, LOW);
            break;
        case oc3a:
            // bitWrite(TCCR3B, CS30, HIGH);
            bitWrite(TCCR3B, CS31, HIGH);
            // bitWrite(TCCR3B, CS32, LOW);
            break;
        case oc4a:
            // bitWrite(TCCR4B, CS40, HIGH);
            bitWrite(TCCR4B, CS41, HIGH);
            // bitWrite(TCCR4B, CS42, LOW);
            break;
        case oc5a:
            // bitWrite(TCCR5B, CS50, HIGH);
            bitWrite(TCCR5B, CS51, HIGH);
            // bitWrite(TCCR5B, CS52, LOW);
            break;
    }
}

// Setup timers
//Refer to table 17-2, pg 145 and table 17-4, pg 155, and table 17-6, pg 157
// of the datasheet for more info.
void PulseGenerator::init(PulsePin pulsePin, uint32_t frequency){
    this->pulsePin = pulsePin;
    uint16_t ocr = clock/(2*prescaler*frequency); // 16MHz/(2*256*(frequency+1))
    noInterrupts();
    switch(pulsePin){
        case oc1a:
        // set timer 1 to fast PWM mode with OCR1x as TOP
        bitWrite(TCCR1A, WGM10, HIGH);
        bitWrite(TCCR1A, WGM11, HIGH);
        bitWrite(TCCR1B, WGM12, HIGH);
        bitWrite(TCCR1B, WGM13, HIGH);
        // Disable Timer
        bitWrite(TCCR1B, CS10, LOW);
        bitWrite(TCCR1B, CS11, LOW);
        bitWrite(TCCR1B, CS12, LOW);
        // Compare output mode set to toggle on match channel A
        bitWrite(TCCR1A, COM1A0, HIGH);
        bitWrite(TCCR1A, COM1A1, LOW);
        // Enable interrupt
        bitWrite(TIMSK1, OCIE1A, HIGH);
        // Set OCR1A to desired frequency
        OCR1A = ocr;
        break;

        case oc3a:
        // set timer 3 to fast PWM mode with OCR3x as TOP
        bitWrite(TCCR3A, WGM30, HIGH);
        bitWrite(TCCR3A, WGM31, HIGH);
        bitWrite(TCCR3B, WGM32, HIGH);
        bitWrite(TCCR3B, WGM33, HIGH);
        // Disable Timer
        bitWrite(TCCR3B, CS30, LOW);
        bitWrite(TCCR3B, CS31, LOW);
        bitWrite(TCCR3B, CS32, LOW);
        // Compare output mode set to toggle on match channel A
        bitWrite(TCCR3A, COM3A0, HIGH);
        bitWrite(TCCR3A, COM3A1, LOW);
        // Enable interrupt
        bitWrite(TIMSK3, OCIE3A, HIGH);
        // Set OCR3A to desired frequency
        OCR3A = ocr;
        break;

        case oc4a:
        // set timer 4 to fast PWM mode with OCR4x as TOP
        bitWrite(TCCR4A, WGM40, HIGH);
        bitWrite(TCCR4A, WGM41, HIGH);
        bitWrite(TCCR4B, WGM42, HIGH);
        bitWrite(TCCR4B, WGM43, HIGH);
        // Disable Timer
        bitWrite(TCCR4B, CS40, LOW);
        bitWrite(TCCR4B, CS41, LOW);
        bitWrite(TCCR4B, CS42, LOW);
        // Compare output mode set to toggle on match channel A
        bitWrite(TCCR4A, COM4A0, HIGH);
        bitWrite(TCCR4A, COM4A1, LOW);
        // Enable interrupt
        bitWrite(TIMSK4, OCIE4A, HIGH);
        // Set OCR4A to desired frequency
        OCR4A = ocr;
        break;

        case oc5a:
        // set timer 5 to fast PWM mode with OCR5x as TOP
        bitWrite(TCCR5A, WGM50, HIGH);
        bitWrite(TCCR5A, WGM51, HIGH);
        bitWrite(TCCR5B, WGM52, HIGH);
        bitWrite(TCCR5B, WGM53, HIGH);
        // Set prescaler to 256
        bitWrite(TCCR5B, CS50, LOW);
        bitWrite(TCCR5B, CS51, LOW);
        bitWrite(TCCR5B, CS52, HIGH);
        // Compare output mode set to toggle on match channel A
        bitWrite(TCCR5A, COM5A0, HIGH);
        bitWrite(TCCR5A, COM5A1, LOW);
        // Enable interrupt
        bitWrite(TIMSK5, OCIE5A, HIGH);
        // Set OCR5A to desired frequency
        OCR5A = ocr;
        break;
    }
    interrupts();
    pinMode(pulsePin, OUTPUT);
}

void PulseGenerator::resetCount(){
    noInterrupts();
    switch(pulsePin){
        case oc1a:
        count1 = 0;
        break;
        case oc3a:
        count3 = 0;
        break;
        case oc4a:
        count4 = 0;
        break;
        case oc5a:
        count5 = 0;
        break;
    }
    interrupts();
}

void PulseGenerator::setCount(long count){
    noInterrupts();
    switch(pulsePin){
        case oc1a:
        count1 = count;
        break;
        case oc3a:
        count3 = count;
        break;
        case oc4a:
        count4 = count;
        break;
        case oc5a:
        count5 = count;
        break;
    }
    interrupts();
}

void PulseGenerator::setDirection(bool dir){
    switch(pulsePin){
        case oc1a:
        dir1 = dir;
        break;
        case oc3a:
        dir3 = dir;
        break;
        case oc4a:
        dir4 = dir;
        break;
        case oc5a:
        dir5 = dir;
        break;
    }
}

void PulseGenerator::setFrequency(uint32_t frequency){
    // noInterrupts();

    uint32_t ocr = double(double(clock)/(2.0*double(prescaler)*double(frequency))); // 16MHz/(2*256*(frequency+1))
    // int ocr = 65535;
    // Serial.println(frequency);
    switch (pulsePin){
    case oc1a:
        OCR1A = ocr;
        break;
    case oc3a:
        OCR3A = ocr;
        break;
    case oc4a:
        OCR4A = ocr;
        break;
    case oc5a:
        OCR5A = ocr;
        break;
    }
    // interrupts();
}

void PulseGenerator::setTarget(long target){
    switch(pulsePin){
        case oc1a:
        target1 = target;
        break;
        case oc3a:
        target3 = target;
        break;
        case oc4a:
        target4 = target;
        break;
        case oc5a:
        target5 = target;
        break;
    }
}
