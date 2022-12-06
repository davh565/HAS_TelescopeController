#ifndef HAS_PULSE
#define HAS_PULSE

#define FORWARD 0
#define REVERSE 1

/// allow pwm pins for Timers 1,3,4,5. Note timer 1 is used by the Servo library
/// If using servo library, do not use Timer 1 pwm pins (OC1A, OC1B, OC1C)
enum PulsePin: uint8_t {
    oc3a = 5,
    oc4a = 6,
    oc1a = 11,
    oc5a = 46
};

enum Prescaler{
    ps0 = 0,
    ps1 = 1,
    ps8 = 8,
    ps64 = 64,
    ps256 = 256,
    ps1024 = 1024
};


class PulseGenerator{
    public:
    long getCount();
    void disable();
    void enable();
    void init(PulsePin pulsePin, 
            uint32_t frequency,
            uint32_t rampUpPulses, 
            uint32_t rampDnPulses,
            uint32_t minRampFrequency);
    void resetCount();
    void setCount(long count);
    void setDirection(bool dir);
    void setTarget(long target);
    void changeTarget(long target);

    void setPulseFrequency(uint32_t frequency);
    private:
    // uint16_t PulseGenerator::calcOcr(uint32_t frequency);
    // void setPrescaler(PulsePin pulsePin, Prescaler value);
    uint8_t pulsePin;


};


#endif // HAS_PULSE