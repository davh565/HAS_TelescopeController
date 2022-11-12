unsigned long prior_event_time = 0;
unsigned long event_time = 0;

enum runMode{
    DONE,
    CONSTANT,
    STEPS,
    ANGLE
};
enum direction{
    FORWARD,
    REVERSE
}; 
////////////////////////////////////////////////////////////////////////////////
namespace io{
    class Stepper{
        private:
        public:
        int pinENA;
        int pinPUL;
        int pinDIR;
        double stepsPerDeg;
        
        int targetSteps;
        int stepCount;
        double angleMoved;
        double targetAngle;
        int speedHz;
        bool isEnabled;
        int runMode;
        bool pulseState;
        unsigned long prevPulseMicros;
        int direction;
        bool generatePulse(unsigned long curMicros);

        Stepper(int pinENA, int pinPUL, int pinDIR,double stepsPerDeg);
        void setEnable(bool isEnabled){this->isEnabled = isEnabled;}
        void setDirection(int direction){
            resetCount();
            this->direction = direction;
            runMode = DONE;
            }
        void setTargetSteps(int targetSteps){
            this->targetSteps = targetSteps;
            }
        void setTargetAngle(double targetAngle){
            this->targetAngle = targetAngle;
            }
        void setRunMode(int runMode){
            resetCount();
            this->runMode = runMode;
            }
        void setSpeedHz(int speedHz){this->speedHz = speedHz;}
        // setSpeedArcsecPerSec()
        // setSpeedDegPerSec()
        // void disable();
        // void enable();
        void resetCount(){
            stepCount = 0;
            angleMoved = 0.0;
        }
        //run this once per loop
        void update(unsigned long curMicros);
    };

    Stepper::Stepper(int pinENA, int pinPUL, int pinDIR,double stepsPerDeg){
        this->pinENA = pinENA;
        this->pinPUL = pinPUL;
        this->pinDIR = pinDIR;
        this->stepsPerDeg = stepsPerDeg;
        targetSteps = 0;
        stepCount = 0;
        angleMoved = 0.0;
        targetAngle = 0.0;
        speedHz = 1500;
        isEnabled = true;
        runMode = DONE;
        pulseState = false;
        prevPulseMicros = micros();
        direction = FORWARD;
    }

    bool Stepper::generatePulse(unsigned long curMicros){
        long period = 1000000/speedHz; //us
        double stepsTaken;
        if(curMicros - prevPulseMicros >= period/2){
            pulseState = !pulseState;
            stepsTaken = (curMicros - prevPulseMicros)/(double(period)/2);
            angleMoved += stepsTaken/stepsPerDeg;
            if(pulseState == HIGH) stepCount++;
            prevPulseMicros = curMicros;
        } 
        return pulseState;
    }
    void Stepper::update(unsigned long curMicros){
        if(isEnabled){
            switch (runMode)
            {
            case CONSTANT:
                digitalWrite(pinPUL, generatePulse(curMicros));
                break;
            
            case STEPS:
                if(stepCount <= targetSteps) digitalWrite(pinPUL, generatePulse(curMicros)); 
                else{
                    setRunMode(DONE);
                }
                break;
            
            case ANGLE:
                if(angleMoved <= targetAngle) digitalWrite(pinPUL, generatePulse(curMicros));
                else{
                    setRunMode(DONE);
                }
                break;
            
            case DONE:
            default:
                break;
            }

        }
        else digitalWrite(pinPUL, LOW);
        digitalWrite(pinENA, isEnabled);
        digitalWrite(pinDIR, direction);
    }
}
////////////////////////////////////////////////////////////////////////////////
io::Stepper raStp(5,6,7,2500.0);
io::Stepper decStp(8,9,10,4411.0);

void declination_Stop(){
  event_time = millis();
  if (event_time - prior_event_time > 1000 && decStp.direction == FORWARD){
    delay(10); //wait for bouncing to settle
    if (digitalRead(2)) {
      decStp.setEnable(false);
      prior_event_time = event_time;
    }
  }
}

void setup(){
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(7,OUTPUT);
    pinMode(8,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), declination_Stop, RISING);
    Serial.begin(9600);
    decStp.setDirection(REVERSE);
    decStp.setEnable(true);
    decStp.setSpeedHz(750);
    decStp.setTargetSteps(20);
    decStp.setTargetAngle (0);
    decStp.setRunMode(ANGLE);

    raStp.setDirection(REVERSE);
    raStp.setEnable(true);
    raStp.setSpeedHz(1500);
    raStp.setTargetSteps(20);
    raStp.setTargetAngle (90);
    raStp.setRunMode(ANGLE);
}
void loop(){
    static unsigned long currentMicros;
    currentMicros = micros();
    static unsigned long logMicros = currentMicros;
    decStp.update(currentMicros);
    raStp.update(currentMicros);
    if(currentMicros-logMicros>1000000){
        Serial.print("ENA: ");
        Serial.print(decStp.isEnabled);
        Serial.print(" DIR: ");
        Serial.print(decStp.direction);
        Serial.print(" PUL: ");
        Serial.print(decStp.pulseState);
        Serial.print(" steps: ");
        Serial.print(decStp.stepCount);
        Serial.print(" deg: ");
        Serial.print(decStp.angleMoved);
        Serial.print(" MODE: ");
        Serial.print(decStp.runMode);
        Serial.print(" TGT: ");
        Serial.print(decStp.targetSteps);
        logMicros = currentMicros;
        Serial.print("\r\n");
    }
}
