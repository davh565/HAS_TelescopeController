#include <Arduino.h>
#include "pulse.h"


/// @brief Experimental pulse generator with dynamic prescaling and ramping.
/// works in isolation, but stability issues when multiple pulse generators used simultaenously

const uint32_t clock = 16000000;
uint16_t ocr;
uint32_t rampUpdateFrequency = 500;

/// Pulse Generator ISR variables
volatile bool timer1Initialized = false;
volatile bool timer3Initialized = false;
volatile bool timer4Initialized = false;
volatile bool timer5Initialized = false;
volatile bool timer1Enabled = false;
volatile bool timer3Enabled = false;
volatile bool timer4Enabled = false;
volatile bool timer5Enabled = false;
volatile Prescaler timer1Prescaler = ps0;
volatile Prescaler timer3Prescaler = ps0;
volatile Prescaler timer4Prescaler = ps0;
volatile Prescaler timer5Prescaler = ps0;
volatile bool pulseState1 = false;
volatile bool pulseState3 = false;
volatile bool pulseState4 = false;
volatile bool pulseState5 = false;
volatile int32_t count1 = 0; // timer 1 pulse count 
volatile int32_t count3 = 0; // timer 3 pulse count
volatile int32_t count4 = 0; // timer 4 pulse count
volatile int32_t count5 = 0; // timer 5 pulse count
volatile int32_t target1 = 0; // timer 1 target count 
volatile int32_t target3 = 0; // timer 3 target count
volatile int32_t target4 = 0; // timer 4 target count
volatile int32_t target5 = 0; // timer 5 target count
volatile bool dir1 = FORWARD; // timer 1 direction
volatile bool dir3 = FORWARD; // timer 3 direction
volatile bool dir4 = FORWARD; // timer 4 direction
volatile bool dir5 = FORWARD; // timer 5 direction

// Ramp ISR variables
volatile int32_t startCount1 = 0; // timer 1 count at start of ramp
volatile int32_t startCount3 = 0; // timer 3 count at start of ramp
volatile int32_t startCount4 = 0; // timer 4 count at start of ramp
volatile int32_t startCount5 = 0; // timer 5 count at start of ramp
volatile uint32_t fRampMin1 = 10000; // minimum frequency for ramping
volatile uint32_t fRampMax1 = 50000; // maximum frequency for ramping
volatile uint32_t rampUpPulses1 = 5000;
volatile uint32_t rampDnPulses1 = 5000;
volatile uint32_t fRampMin3 = 10000; // minimum frequency for ramping
volatile uint32_t fRampMax3 = 50000; // maximum frequency for ramping
volatile uint32_t rampUpPulses3 = 5000;
volatile uint32_t rampDnPulses3 = 5000;
volatile uint32_t fRampMin4 =25000; // minimum frequency for ramping
volatile uint32_t fRampMax4 = 100000; // maximum frequency for ramping
volatile uint32_t rampUpPulses4 = 25000;
volatile uint32_t rampDnPulses4 = 10000;
volatile uint32_t fRampMin5 = 10000; // minimum frequency for ramping
volatile uint32_t fRampMax5 = 50000; // maximum frequency for ramping
volatile uint32_t rampUpPulses5 = 2000;
volatile uint32_t rampDnPulses5 = 2000;
volatile uint32_t deltaCount;
volatile uint32_t deltaFreq;
volatile uint32_t currentPulse;
volatile uint32_t freq1;
volatile uint32_t freq3;
volatile uint32_t freq4;
volatile uint32_t freq5;

void rampInit();
uint16_t calcOcr(uint32_t frequency, uint16_t prescaler);
void setFrequency(PulsePin pulsePin, uint32_t frequency);
void setPrescaler(PulsePin pulsePin, Prescaler value);

// // Timer 2 is used for ramping the pulse rate of the other timers. This is needed
// // to prevent missed steps or stalling of stepper motors at high speeds.
ISR(TIMER2_COMPA_vect) {
    // static bool toggle = false;
    // digitalWrite(5, toggle);
    // toggle = !toggle;
    
    if(timer1Initialized && timer1Enabled && freq1 > fRampMin1){
        deltaCount = abs(target1 - startCount1);
        deltaFreq = fRampMax1 - fRampMin1;
        currentPulse = abs(count1 - startCount1);
        // if movement is longer than combined ramp times
        if (deltaCount >= rampUpPulses1 + rampDnPulses1){
            if (currentPulse==0 || deltaCount - currentPulse == 0) setFrequency(oc1a, fRampMin1);
            else if (currentPulse < rampUpPulses1) setFrequency(oc1a, fRampMin1 + currentPulse * double(deltaFreq) / rampUpPulses1);
            else if (currentPulse > deltaCount - rampDnPulses1) setFrequency(oc1a, fRampMin1 + ((deltaCount - currentPulse) * double(deltaFreq) / rampDnPulses1));
            else setFrequency(oc1a, fRampMax1);
        }
        // if movement is shorter than combined ramp times, ramp proporiionately
        // to ramp lengths
        else{
            if (currentPulse < deltaCount*(rampUpPulses1/(rampUpPulses1+rampDnPulses1))) setFrequency(oc1a, fRampMin1 + (currentPulse * double(deltaFreq) / rampUpPulses1));
            else setFrequency(oc1a, fRampMin1 + ((deltaCount - currentPulse) * double(deltaFreq) / rampDnPulses1));
        }
    }

    if(timer3Initialized && timer3Enabled && freq3 > fRampMin3){
        deltaCount = abs(target3 - startCount3);
        deltaFreq = fRampMax3 - fRampMin3;
        currentPulse = abs(count3 - startCount3);
        // if movement is longer than combined ramp times
        if (deltaCount >= rampUpPulses3 + rampDnPulses3){
            if (currentPulse==0 || deltaCount - currentPulse == 0) setFrequency(oc3a, fRampMin3);
            else if (currentPulse < rampUpPulses3) setFrequency(oc3a, fRampMin3 + currentPulse * double(deltaFreq) / rampUpPulses3);
            else if (currentPulse > deltaCount - rampDnPulses3) setFrequency(oc3a, fRampMin3 + ((deltaCount - currentPulse) * double(deltaFreq) / rampDnPulses3));
            else setFrequency(oc3a, fRampMax3);
        }
        // if movement is shorter than combined ramp times, ramp proporiionately
        // to ramp lengths
        else{
            if (currentPulse < deltaCount*(rampUpPulses3/(rampUpPulses3+rampDnPulses3))) setFrequency(oc3a, fRampMin3 + (currentPulse * double(deltaFreq) / rampUpPulses3));
            else setFrequency(oc3a, fRampMin3 + ((deltaCount - currentPulse) * double(deltaFreq) / rampDnPulses3));
        }
    }

    if(timer4Initialized && timer4Enabled && freq4 > fRampMin4){
        deltaCount = abs(target4 - startCount4);
        deltaFreq = fRampMax4 - fRampMin4;
        currentPulse = abs(count4 - startCount4);
        // if movement is longer than combined ramp times
        if (deltaCount >= rampUpPulses4 + rampDnPulses4){
            if (currentPulse==0 || deltaCount - currentPulse == 0) setFrequency(oc4a, fRampMin4);
            else if (currentPulse < rampUpPulses4) setFrequency(oc4a, fRampMin4 + currentPulse * double(deltaFreq) / rampUpPulses4);
            else if (currentPulse > deltaCount - rampDnPulses4) setFrequency(oc4a, fRampMin4 + ((deltaCount - currentPulse) * double(deltaFreq) / rampDnPulses4));
            else setFrequency(oc4a, fRampMax4);
        }
        // if movement is shorter than combined ramp times, ramp proporiionately
        // to ramp lengths
        else{
            if (currentPulse < deltaCount*(rampUpPulses4/(rampUpPulses4+rampDnPulses4))) setFrequency(oc4a, fRampMin4 + (currentPulse * double(deltaFreq) / rampUpPulses4));
            else setFrequency(oc4a, fRampMin4 + ((deltaCount - currentPulse) * double(deltaFreq) / rampDnPulses4));
        }
    }

    if(timer5Initialized && timer5Enabled && freq5 > fRampMin5){
        deltaCount = abs(target5 - startCount5);
        deltaFreq = fRampMax5 - fRampMin5;
        currentPulse = abs(count5 - startCount5);
        // if movement is longer than combined ramp times
        if (deltaCount >= rampUpPulses5 + rampDnPulses5){
            if (currentPulse==0 || deltaCount - currentPulse == 0) setFrequency(oc5a, fRampMin5);
            else if (currentPulse < rampUpPulses5) setFrequency(oc5a, fRampMin5 + currentPulse * double(deltaFreq) / rampUpPulses5);
            else if (currentPulse > deltaCount - rampDnPulses5) setFrequency(oc5a, fRampMin5 + ((deltaCount - currentPulse) * double(deltaFreq) / rampDnPulses5));
            else setFrequency(oc5a, fRampMax5);
        }
        // if movement is shorter than combined ramp times, ramp proporiionately
        // to ramp lengths
        else{
            if (currentPulse < deltaCount*(rampUpPulses5/(rampUpPulses5+rampDnPulses5))) setFrequency(oc5a, fRampMin5 + (currentPulse * double(deltaFreq) / rampUpPulses5));
            else setFrequency(oc5a, fRampMin5 + ((deltaCount - currentPulse) * double(deltaFreq) / rampDnPulses5));
        }
    }

}
// timer 1A interrupt service routine
ISR(TIMER1_COMPA_vect) {
    pulseState1 = !pulseState1;
    if (dir1 == FORWARD) {
        if(!pulseState1) count1++;
        if (count1 >= target1){
            // Disable Timer
            timer1Enabled = false;
            bitWrite(TCCR1B, CS10, LOW);
            bitWrite(TCCR1B, CS11, LOW);
            bitWrite(TCCR1B, CS12, LOW);
            // digitalWrite(oc1a, LOW);
        }
    }
    else {
        if(!pulseState1) count1--;
        if (count1 <= target1){
            // Disable Timer
            timer1Enabled = false;
            bitWrite(TCCR1B, CS10, LOW);
            bitWrite(TCCR1B, CS11, LOW);
            bitWrite(TCCR1B, CS12, LOW);
            // digitalWrite(oc1a, LOW);
        }
    }
}

// timer 3A interrupt service routine
ISR(TIMER3_COMPA_vect) {
    pulseState3 = !pulseState3;
    if (dir3 == FORWARD) {
        if(!pulseState3) count3++;
        if (count3 >= target3){
            // Disable Timer
            timer3Enabled = false;
            bitWrite(TCCR3B, CS30, LOW);
            bitWrite(TCCR3B, CS31, LOW);
            bitWrite(TCCR3B, CS32, LOW);
            // digitalWrite(oc3a, LOW);
        }
    }
    else {
        if(!pulseState3) count3--;
        if (count3 <= target3){
            // Disable Timer
            timer3Enabled = false;
            bitWrite(TCCR3B, CS30, LOW);
            bitWrite(TCCR3B, CS31, LOW);
            bitWrite(TCCR3B, CS32, LOW);
            // digitalWrite(oc3a, LOW);
        }
    }
}

// timer 4A interrupt service routine
ISR(TIMER4_COMPA_vect) {
    pulseState4 = !pulseState4;
    if (dir4 == FORWARD) {
        if(!pulseState4) count4++;
        if (count4 >= target4){
            // Disable Timer
            timer4Enabled = false;
            bitWrite(TCCR4B, CS40, LOW);
            bitWrite(TCCR4B, CS41, LOW);
            bitWrite(TCCR4B, CS42, LOW);
            // digitalWrite(oc4a, LOW);
        }
    }
    else {
        if(!pulseState4) count4--;
        if (count4 <= target4){
            // Disable Timer
            timer4Enabled = false;
            bitWrite(TCCR4B, CS40, LOW);
            bitWrite(TCCR4B, CS41, LOW);
            bitWrite(TCCR4B, CS42, LOW);
            // digitalWrite(oc4a, LOW);
        }
    }
}

// timer 5A interrupt service routine
ISR(TIMER5_COMPA_vect) {
    pulseState5 = !pulseState5;
    if (dir5 == FORWARD) {
        if(!pulseState5) count5++;
        if (count5 >= target5){
            // Disable Timer
            timer5Enabled = false;
            bitWrite(TCCR5B, CS50, LOW);
            bitWrite(TCCR5B, CS51, LOW);
            bitWrite(TCCR5B, CS52, LOW);
            // digitalWrite(oc5a, LOW);
        }
    }
    else {
        if(!pulseState5) count5--;
        if (count5 <= target5){
            // Disable Timer
            timer5Enabled = false;
            bitWrite(TCCR5B, CS50, LOW);
            bitWrite(TCCR5B, CS51, LOW);
            bitWrite(TCCR5B, CS52, LOW);
            // digitalWrite(oc5a, LOW);
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
    // Serial.println("disable");
    switch (pulsePin){
        case oc1a: timer1Enabled = false; break;
        case oc3a: timer3Enabled = false; break;
        case oc4a: timer4Enabled = false; break;
        case oc5a: timer5Enabled = false; break;
    }
    setPrescaler(pulsePin, ps0);
    // digitalWrite(pulsePin, LOW);
}
// Enable Timer by setting prescaler to 64
void PulseGenerator::enable(){
    // Serial.println("enable");
    switch (pulsePin){

        case oc1a:
            timer1Enabled = true; 
            setPrescaler(pulsePin, timer1Prescaler);
            break;
        case oc3a:
            timer3Enabled = true;
            setPrescaler(pulsePin, timer3Prescaler);
            break;
        case oc4a:
            timer4Enabled = true;
            setPrescaler(pulsePin, timer4Prescaler);
            break;
        case oc5a:
            timer5Enabled = true;
            setPrescaler(pulsePin, timer5Prescaler);
            break;
    }
}


// Setup timers
//Refer to table 17-2, pg 145 and table 17-4, pg 155, and table 17-6, pg 157
// of the datasheet for more info.
void PulseGenerator::init(PulsePin pulsePin, 
                        uint32_t frequency,
                        uint32_t rampUpPulses,
                        uint32_t rampDnPulses,
                        uint32_t minRampFrequency){
    this->pulsePin = pulsePin;
    // uint16_t ocr = 100;

    // Serial.println("freq: "+ String(frequency));
    noInterrupts();
    TCCR1A=0;
    TCCR3A=0;
    TCCR4A=0;
    TCCR5A=0;
    setPulseFrequency(frequency);
    switch(pulsePin){
        case oc1a:
        timer1Initialized = true;
        rampUpPulses1 = rampUpPulses;
        rampDnPulses1 = rampDnPulses;
        fRampMin1 = minRampFrequency;
        // set timer 1 to fast PWM mode with OCR1x as TOP
        bitWrite(TCCR1A, WGM10, HIGH);
        bitWrite(TCCR1A, WGM11, HIGH);
        bitWrite(TCCR1B, WGM12, HIGH);
        bitWrite(TCCR1B, WGM13, HIGH);
        // Compare output mode set to toggle on match channel A
        bitWrite(TCCR1A, COM1A0, HIGH);
        bitWrite(TCCR1A, COM1A1, LOW);
        // Enable interrupt
        bitWrite(TIMSK1, OCIE1A, HIGH);
        break;

        case oc3a:
        timer3Initialized = true;
        rampUpPulses3 = rampUpPulses;
        rampDnPulses3 = rampDnPulses;
        fRampMin3 = minRampFrequency;
        // set timer 3 to fast PWM mode with OCR3x as TOP
        bitWrite(TCCR3A, WGM30, HIGH);
        bitWrite(TCCR3A, WGM31, HIGH);
        bitWrite(TCCR3B, WGM32, HIGH);
        bitWrite(TCCR3B, WGM33, HIGH);
        // Compare output mode set to toggle on match channel A
        bitWrite(TCCR3A, COM3A0, HIGH);
        bitWrite(TCCR3A, COM3A1, LOW);
        // Enable interrupt
        bitWrite(TIMSK3, OCIE3A, HIGH);
        break;

        case oc4a:
        timer4Initialized = true;
        rampUpPulses4 = rampUpPulses;
        rampDnPulses4 = rampDnPulses;
        fRampMin4 = minRampFrequency;
        // set timer 4 to fast PWM mode with OCR4x as TOP
        bitWrite(TCCR4A, WGM40, HIGH);
        bitWrite(TCCR4A, WGM41, HIGH);
        bitWrite(TCCR4B, WGM42, HIGH);
        bitWrite(TCCR4B, WGM43, HIGH);
        // Compare output mode set to toggle on match channel A
        bitWrite(TCCR4A, COM4A0, HIGH);
        bitWrite(TCCR4A, COM4A1, LOW);
        // Enable interrupt
        bitWrite(TIMSK4, OCIE4A, HIGH);
        break;

        case oc5a:
        timer5Initialized = true;
        rampUpPulses5 = rampUpPulses;
        rampDnPulses5 = rampDnPulses;
        fRampMin5 = minRampFrequency;
        // set timer 5 to fast PWM mode with OCR5x as TOP
        bitWrite(TCCR5A, WGM50, HIGH);
        bitWrite(TCCR5A, WGM51, HIGH);
        bitWrite(TCCR5B, WGM52, HIGH);
        bitWrite(TCCR5B, WGM53, HIGH);
        // Compare output mode set to toggle on match channel A
        bitWrite(TCCR5A, COM5A0, HIGH);
        bitWrite(TCCR5A, COM5A1, LOW);
        // Enable interrupt
        bitWrite(TIMSK5, OCIE5A, HIGH);
        break;
    }
    rampInit();
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

void rampInit(){
        // set timer 2 to CTC mode with OCR2x as TOP
        bitWrite(TCCR2A, WGM20, LOW);
        bitWrite(TCCR2A, WGM21, HIGH);
        bitWrite(TCCR2B, WGM22, LOW);
        // Compare output mode set to disable pin output (use timer internally)
        bitWrite(TCCR2A, COM2A0, LOW);
        bitWrite(TCCR2A, COM2A1, LOW);
        // Set OCR2A to desired frequency
        OCR2A = calcOcr(rampUpdateFrequency/2, 1024);
        // Set prescaler to 1024
        bitWrite(TCCR2B, CS20, HIGH);
        bitWrite(TCCR2B, CS21, HIGH);
        bitWrite(TCCR2B, CS22, HIGH);
        // Enable interrupt
        bitWrite(TIMSK2, OCIE2A, HIGH);

}


void PulseGenerator::setPulseFrequency(uint32_t frequency){
    // Serial.println("setPulseFrequency");
    switch (pulsePin){
    case oc1a:
        freq1 = frequency;
        if(frequency > fRampMin1){
            fRampMax1 = frequency;
            if (!timer1Enabled) setFrequency(oc1a, fRampMin1);
        } 
        else setFrequency(oc1a, frequency);
        break;
    case oc3a:
        freq3 = frequency;
        if(frequency > fRampMin3){
            fRampMax3 = frequency;
            if (!timer3Enabled) setFrequency(oc3a, fRampMin3);
        }
        else setFrequency(oc3a, frequency);
        break;
    case oc4a:
        freq4 = frequency;
        if(frequency > fRampMin4){
            fRampMax4 = frequency;
            rampUpPulses4 = frequency*1.5;
            rampDnPulses4 = frequency/2;
            if (!timer4Enabled) setFrequency(oc4a, fRampMin4);
        }
        else setFrequency(oc4a, frequency);
        break;
    case oc5a:
        freq5 = frequency;
        if(frequency > fRampMin5){
            fRampMax5 = frequency;
            if (!timer5Enabled) setFrequency(oc5a, fRampMin5);
        }
        else setFrequency(oc5a, frequency);
        break;
    }
}


void PulseGenerator::setTarget(long target){
    switch(pulsePin){
        case oc1a:
        target1 = target;
        startCount1 = count1;
        break;
        case oc3a:
        target3 = target;
        startCount3 = count3;
        break;
        case oc4a:
        target4 = target;
        startCount4 = count4;
        break;
        case oc5a:
        target5 = target;
        startCount5 = count5;
        break;
    }
}
void PulseGenerator::changeTarget(long target){
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

void setPrescaler(PulsePin pulsePin, Prescaler value){
    switch (pulsePin){
    case oc1a:
        switch (value){
        case ps0:
            bitWrite(TCCR1B, CS10, LOW);
            bitWrite(TCCR1B, CS11, LOW);
            bitWrite(TCCR1B, CS12, LOW);
            break;
        case ps1:
            bitWrite(TCCR1B, CS10, HIGH);
            bitWrite(TCCR1B, CS11, LOW);
            bitWrite(TCCR1B, CS12, LOW);
            break;
        case ps8:
            bitWrite(TCCR1B, CS10, LOW);
            bitWrite(TCCR1B, CS11, HIGH);
            bitWrite(TCCR1B, CS12, LOW);
            break;
        case ps64:
            bitWrite(TCCR1B, CS10, HIGH);
            bitWrite(TCCR1B, CS11, HIGH);
            bitWrite(TCCR1B, CS12, LOW);
            break;
        case ps256:
            bitWrite(TCCR1B, CS10, LOW);
            bitWrite(TCCR1B, CS11, LOW);
            bitWrite(TCCR1B, CS12, HIGH);
            break;
        case ps1024:
            bitWrite(TCCR1B, CS10, HIGH);
            bitWrite(TCCR1B, CS11, LOW);
            bitWrite(TCCR1B, CS12, HIGH);
            break;
        }
    case oc3a:
        switch (value){
        case ps0:
            bitWrite(TCCR3B, CS30, LOW);
            bitWrite(TCCR3B, CS31, LOW);
            bitWrite(TCCR3B, CS32, LOW);
            break;
        case ps1:
            bitWrite(TCCR3B, CS30, HIGH);
            bitWrite(TCCR3B, CS31, LOW);
            bitWrite(TCCR3B, CS32, LOW);
            break;
        case ps8:
            bitWrite(TCCR3B, CS30, LOW);
            bitWrite(TCCR3B, CS31, HIGH);
            bitWrite(TCCR3B, CS32, LOW);
            break;
        case ps64:
            bitWrite(TCCR3B, CS30, HIGH);
            bitWrite(TCCR3B, CS31, HIGH);
            bitWrite(TCCR3B, CS32, LOW);
            break;
        case ps256:
            bitWrite(TCCR3B, CS30, LOW);
            bitWrite(TCCR3B, CS31, LOW);
            bitWrite(TCCR3B, CS32, HIGH);
            break;
        case ps1024:
            bitWrite(TCCR3B, CS30, HIGH);
            bitWrite(TCCR3B, CS31, LOW);
            bitWrite(TCCR3B, CS32, HIGH);
            break;
        }
    case oc4a:
        switch (value){
        case ps0:
            bitWrite(TCCR4B, CS40, LOW);
            bitWrite(TCCR4B, CS41, LOW);
            bitWrite(TCCR4B, CS42, LOW);
            break;
        case ps1:
            bitWrite(TCCR4B, CS40, HIGH);
            bitWrite(TCCR4B, CS41, LOW);
            bitWrite(TCCR4B, CS42, LOW);
            break;
        case ps8:
            bitWrite(TCCR4B, CS40, LOW);
            bitWrite(TCCR4B, CS41, HIGH);
            bitWrite(TCCR4B, CS42, LOW);
            break;
        case ps64:
            bitWrite(TCCR4B, CS40, HIGH);
            bitWrite(TCCR4B, CS41, HIGH);
            bitWrite(TCCR4B, CS42, LOW);
            break;
        case ps256:
            bitWrite(TCCR4B, CS40, LOW);
            bitWrite(TCCR4B, CS41, LOW);
            bitWrite(TCCR4B, CS42, HIGH);
            break;
        case ps1024:
            bitWrite(TCCR4B, CS40, HIGH);
            bitWrite(TCCR4B, CS41, LOW);
            bitWrite(TCCR4B, CS42, HIGH);
            break;
        }
    case oc5a:
        switch (value){
        case ps0:
            bitWrite(TCCR5B, CS50, LOW);
            bitWrite(TCCR5B, CS51, LOW);
            bitWrite(TCCR5B, CS52, LOW);
            break;
        case ps1:
            bitWrite(TCCR5B, CS50, HIGH);
            bitWrite(TCCR5B, CS51, LOW);
            bitWrite(TCCR5B, CS52, LOW);
            break;
        case ps8:
            bitWrite(TCCR5B, CS50, LOW);
            bitWrite(TCCR5B, CS51, HIGH);
            bitWrite(TCCR5B, CS52, LOW);
            break;
        case ps64:
            bitWrite(TCCR5B, CS50, HIGH);
            bitWrite(TCCR5B, CS51, HIGH);
            bitWrite(TCCR5B, CS52, LOW);
            break;
        case ps256:
            bitWrite(TCCR5B, CS50, LOW);
            bitWrite(TCCR5B, CS51, LOW);
            bitWrite(TCCR5B, CS52, HIGH);
            break;
        case ps1024:
            bitWrite(TCCR5B, CS50, HIGH);
            bitWrite(TCCR5B, CS51, LOW);
            bitWrite(TCCR5B, CS52, HIGH);
            break;
        }
    }
}

uint16_t calcOcr(uint32_t frequency, uint16_t prescaler){
    if (frequency == 0 || prescaler == 0) return 0-1;
    else return clock/(2*prescaler*frequency);
    }

void setFrequency(PulsePin pulsePin, uint32_t frequency){
    uint16_t currentPs;
    uint16_t ocr;

    // noInterrupts();
    if      (frequency >= 250) currentPs = ps1;
    else if (frequency >= 50)  currentPs = ps8;
    else if (frequency >= 2)   currentPs = ps64;
    else if (frequency >= 1)   currentPs = ps256;
    else if (frequency < 1)    currentPs = ps1024;
    ocr = calcOcr(frequency, currentPs);
    switch (pulsePin){
    case oc1a:
        timer1Prescaler = currentPs;
        if (timer1Enabled) setPrescaler(pulsePin, timer1Prescaler);
        else setPrescaler(pulsePin, ps0);
        OCR1A = ocr;
        break;
    case oc3a:
        timer3Prescaler = currentPs;
        if (timer3Enabled) setPrescaler(pulsePin, timer3Prescaler);
        else setPrescaler(pulsePin, ps0);
        OCR3A = ocr;
        break;
    case oc4a:
        OCR4A = ocr;
        timer4Prescaler = currentPs;
        if (timer4Enabled) setPrescaler(pulsePin, timer4Prescaler);
        else setPrescaler(pulsePin, ps0);
        break;
    case oc5a:
        timer5Prescaler = currentPs;
        if (timer5Enabled) setPrescaler(pulsePin, timer5Prescaler);
        else setPrescaler(pulsePin, ps0);
        OCR5A = ocr;
        break;
    }
    // interrupts();
}
