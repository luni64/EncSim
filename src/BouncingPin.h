#pragma once

#include <TeensyDelay.h>
#include "wprogram.h"

template<unsigned pin, unsigned channel>
class BouncingPin
{
public:
    void begin()
    {
        pinMode(pin, OUTPUT);
        digitalWriteFast(pin, LOW);
        TeensyDelay::addDelayChannel(bounce, channel);
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
    static unsigned totalBounceTime, minBounceTime, maxBounceTime;

private:
    static bool targetState;

    static void bounce()
    {
        static unsigned time = 0;

        if (time < totalBounceTime) {
            digitalWriteFast(pin, !digitalReadFast(pin));
            int curBounceTime = random(minBounceTime, maxBounceTime);
            time += curBounceTime;
            TeensyDelay::trigger(curBounceTime, channel);
        }
        else {
            digitalWriteFast(pin, targetState);
            time = 0;
        }
    }
};

template<unsigned pin, unsigned channel>
unsigned BouncingPin<pin, channel>::totalBounceTime = 10000;

template<unsigned pin, unsigned channel>
unsigned BouncingPin<pin, channel>::minBounceTime = 100;

template<unsigned pin, unsigned channel>
unsigned BouncingPin<pin, channel>::maxBounceTime = 2000;

template<unsigned pin, unsigned channel>
bool BouncingPin<pin, channel>::targetState;


