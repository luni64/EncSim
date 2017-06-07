#pragma once

//---------------------------------------------------------------------
// Simple wrapper around an interval timer allowing low level access
// to the registers
//--------------------------------------------------------------------

#include <IntervalTimer.h>

class IPitHandler
{
public:
    virtual void pitISR() = 0;
};

class PIT
{ 
public:
    bool begin(IPitHandler* handler, uint8_t priority=128);

    inline void stop() const { channel->TCTRL = 0; }
    inline void start()const {channel->TCTRL |= PIT_TCTRL_TEN; }
    inline void setCounter(uint32_t val)const { channel->CVAL = val; }
    inline void enableInterupt() const { channel->TFLG = 1; channel->TCTRL |= PIT_TCTRL_TIE; }
    inline void disableInterupt() const { channel->TCTRL &= ~PIT_TCTRL_TIE; }
    inline void setReloadValueAndStart(uint32_t val) const { channel->TCTRL = 0; channel->LDVAL = val; channel->TCTRL = 3; }
    inline void setReloadValue(uint32_t val)const{ channel->LDVAL = val;}
    inline bool isRunning() const { return (channel->TCTRL & 3) == 3 ; }

protected:
    IntervalTimer timer;
    void setupChannel();

    KINETISK_PIT_CHANNEL_t* channel = nullptr;
};
