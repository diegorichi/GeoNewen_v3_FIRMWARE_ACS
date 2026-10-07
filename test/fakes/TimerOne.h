#pragma once
class TimerOneFake {
public:
    int lastPin = -1;
    int lastValue = 0;
    unsigned long lastPeriod = 0;
    void pwm(int pin, int value, unsigned long period) {
        lastPin = pin;
        lastValue = value;
        lastPeriod = period;
    }
    void disablePwm(int) {}
};
extern TimerOneFake Timer1;
