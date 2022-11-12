#ifndef HAS_PULSE
#define HAS_PULSE

#define FORWARD 0
#define REVERSE 1

#define TARGET 0
#define CONSTANT 1
/// allow pwm pins for Timers 1,3,4,5. Note timer 1 is used by the Servo library
/// If using servo library, do not use Timer 1 pwm pins (OC1A, OC1B, OC1C)
enum PulsePin: uint8_t {
    oc3a = 5,
    oc4a = 6,
    oc1a = 11,
    oc5a = 46
};
class PulseGenerator{
    public:
    long getCount();
    void disable();
    void enable();
    void init(PulsePin pulsePin, uint32_t frequency);
    void resetCount();
    void setCount(long count);
    void setDirection(bool dir);
    void setFrequency(uint32_t frequency);
    void setRunMode(bool mode);
    void setTarget(long target);

    private:
    uint8_t pulsePin;
};

#endif // HAS_PULSE