#pragma once

#include "TeensyTimerTool.h"
#include "wprogram.h"

class BouncingPin
{
 public:
    BouncingPin() :bounceTimer(TeensyTimerTool::TCK) {}

    void begin(unsigned pin)
    {
        this->pin = pin;
        pinMode(pin, OUTPUT);
        digitalWriteFast(pin, LOW);
        bounceTimer.begin([this] { this->bounce(); });
        targetState = false;
    }

    void toggle()
    {
        targetState = !targetState;
        bounce();
    }

    void set(int level)
    {
        targetState = level;
        bounce();
    }
    unsigned totalBounceTime, minBounceTime, maxBounceTime;

 private:
    bool targetState;

    void bounce()
    {
        static unsigned time = 0;

        if (time < totalBounceTime) // do we need another bounce peak?
        {
            digitalWriteFast(pin, !digitalReadFast(pin));             // toggle pin
            int curBounceTime = random(minBounceTime, maxBounceTime); // get random time for this peak
            time += curBounceTime;
            bounceTimer.trigger(curBounceTime);
            //Serial.printf("bouncing %d\n", curBounceTime);
        } else
        {
            //Serial.printf("no bounce  %d\n", s);
            digitalWriteFast(pin, targetState); // we always end with the target level
            time = 0;                 // prepare for next bouncing
        }
    }

    TeensyTimerTool::OneShotTimer bounceTimer;

    unsigned pin;
};

// template<unsigned pin, unsigned channel>
// unsigned BouncingPin<pin, channel>::totalBounceTime = 10000;

// template<unsigned pin, unsigned channel>
// unsigned BouncingPin<pin, channel>::minBounceTime = 100;

// template<unsigned pin, unsigned channel>
// unsigned BouncingPin<pin, channel>::maxBounceTime = 2000;

// template<unsigned pin, unsigned channel>
// bool BouncingPin<pin, channel>::targetState;
