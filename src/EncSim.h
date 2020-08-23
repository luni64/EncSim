#pragma once

#include "BouncingPin.h"
#include "TeensyTimerTool.h"
#include <algorithm>

using TimerGen = TeensyTimerTool::TimerGenerator* const;

class EncSim
{
 public:
    EncSim(unsigned pinA, unsigned pinB, unsigned pinZ = UINT32_MAX);

    //void begin(unsigned pinA, unsigned pinB, int pinZ=-1);
    EncSim& begin();

    void moveAbsAsync(int _target); // start absolute move and return immediately
    void moveRelAsync(int delta);   // start relative move and return immediately
    void moveAbs(int target);       // move to absolute position (blocking)
    void moveRel(int delta);        // move to relative position (blocking)
    void stop();                    // stop movement
    bool isRunning();               // check status
    volatile int current;

    void printSettings(Stream& s);

    //settings----------------------------------------------------
    EncSim& setFrequency(float f_Hz);
    EncSim& setPeriod(unsigned p);
    EncSim& setPhase(float deg);
    EncSim& setCountRate(float f_Hz, float phase_deg = 90.0);
    EncSim& setTotalBounceDuration(unsigned microseconds);
    EncSim& setBounceDurationMin(unsigned microseconds);
    EncSim& setBounceDurationMax(unsigned microseconds);


    protected:

    void pitISR()
    {
        current += direction;
        if (current == target)
        {
            stop();
        } else
            mainTimer.update(T[current & 1]); //T0 / T1 differ if phase != 90°

        if (Z < UINT32_MAX)
        {
            digitalWriteFast(Z, ((current  - 2) % period) == 0);  // current-2 to have the zero pulse on the rising edge of B
        }

        if (current & 1)
        {
            direction == 1 ? phaseA.toggle() : phaseB.toggle();
        } else
        {
            direction == 1 ? phaseB.toggle() : phaseA.toggle();
        }
    }

    int direction;
    int target;

    float frequency;
    float phase;
    float T[2];
    bool running = false;

    unsigned A, B, Z;
    unsigned period;

    BouncingPin phaseA, phaseB;
    IntervalTimer mainTimer;
};
