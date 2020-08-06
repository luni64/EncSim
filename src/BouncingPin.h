#pragma once

//#include <TeensyDelay.h>
#include "wprogram.h"

template<unsigned pin, unsigned channel>
class BouncingPin
{
public:
    void begin()
    {
        pinMode(pin, OUTPUT);
        digitalWriteFast(pin, LOW);
        //TeensyDelay::addDelayChannel(bounce, channel);     // see luni64/TeensyDelay for info about this library
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

        if (time < totalBounceTime)                                     // do we need another bounce peak? 
        {
            digitalWriteFast(pin, !digitalReadFast(pin));               // toggle pin
            int curBounceTime = random(minBounceTime, maxBounceTime);   // get random time for this peak
            time += curBounceTime;                                                  
            //TeensyDelay::trigger(curBounceTime, channel);                // retrigger the delay channel
        }
        else {                                                           // bounce time over           
            digitalWriteFast(pin, targetState);                          // we always end with the target level           
            time = 0;       	                                         // prepare for next bouncing    
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


