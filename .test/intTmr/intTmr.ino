
volatile boolean pulseState4 = LOW; // timer 4 pulse state
volatile boolean pulseState5 = LOW; // timer 5 pulse state
volatile unsigned long pulseCount4 = 0; // timer 4 pulse count
volatile unsigned long pulseCount5 = 0; // timer 5 pulse count

const int ocr5Hz = 1561; //5.0016Hz  --this is the Sidereal tracking rate
const int ocr1_5kHz = 4;   //1.kHz
const int ocr2kHz = 3;   //1.953kHz

/// @brief calculate maximum OCR register value for a 256 prescaler and a 16MHz 
/// clock to achive the desired frequency
int calcOcr(unsigned int frequency){return 15625/(2*(frequency+1));}

/// @brief disable interrupts, adjust OCR register, and re-enable interrupts
void setFrequency(unsigned int frequency){
    noInterrupts();
    OCR4A = calcOcr(frequency);
    interrupts();
    }

int ocrMax = ocr2kHz;

void setup(){
    pinMode(12,OUTPUT);
 


//see ATMEGA2560 datasheet p. 154-157 for timer registers and prescaler values
//Based on code by A. Ghassaei 2012
//https://www.instructables.com/id/Arduino-Timer-Interrupts/
noInterrupts();//stop interrupts
 TCCR4A = 0;
 TCCR4B = 0;
 TCNT4  = 0;
 OCR4A = ocr5Hz;// = (16*10^6) / (1*1024) - 1 (must be <65536)
 // turn on CTC mode
 TCCR4B |= (1 << WGM42);
 // Set CS4 2bit for 256 prescaler
 TCCR4B |= (1 << CS42) | (1 << CS40); 
 // enable timer compare interrupt
 TIMSK4 |= (1 << OCIE4A);

interrupts();//allow interrupts
setFrequency(2000);
}//end setup

ISR(TIMER4_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)

 digitalWrite(12,pulseState4);
 pulseState4 = !pulseState4;

}
 
void loop(){
    // static int cnt = 1;
    // }
    // cnt+=100;
    // setFrequency(cnt);
    // delay(2000);

 //do other things here
}