#pragma once

#include <TeensyDelay.h>
#include "BouncingPin.h"
#include "PIT.h"

template<int A, int B>
class EncSim : IPitHandler
{
public:
    void begin()
    {
        mainTimer.begin(this, 16);
        TeensyDelay::begin();

        phaseA.begin();
        phaseB.begin();

        // defaults
        setFrequency(50);               // Quadrature signal with
        setPhase(90);                   // 50Hz and 90° phase    
        setTotalBounceDuration(5000);   // 5ms bouncing
        setBounceDurationMax(300);      // bounce peaks between
        setBounceDurationMin(30);       // 30 and 300µs      

        current = 0;                    // reset counter
    }

    // start absolute move and return immediately
    void moveAbsAsync(int _target) 
    {
        if (_target == current) return;

        target = _target;
        direction = (target >= current) ? 1 : -1;       
        mainTimer.setReloadValueAndStart(T[current & 1]);        
    }

    // start relative move and return immediately
    void moveRelAsync(int delta)
    {
        moveAbsAsync(current + delta);
    }

    // move to absolute position (blocking)
    void moveAbs(int target) 
    {
        moveAbsAsync(target);
        while (isRunning()) {
            delayMicroseconds(5);
            Serial.print("+");
            delay(10);
        }
    }

    // move to relative position (blocking)
    void moveRel(int delta) 
    {
        moveAbs(current + delta);
    }

    // stop movement
    void stop() const
    {
        mainTimer.disableInterupt();
    }

    bool isRunning()
    {
        return mainTimer.isRunning();
    }
  
    // main timer ISR -----------------------------------------------------------

    void pitISR()
    {  
        mainTimer.setReloadValue(T[current & 1]);  //T0 / T1 differ if phase != 90°

        //generate quadrature signal 
        if (current & 1) {
            direction == 1 ? phaseA.toggle() : phaseB.toggle();
        }
        else {
            direction == 1 ? phaseB.toggle() : phaseA.toggle();
        }
        current += direction;

        if (current == target) {
            mainTimer.disableInterupt();
        }
    }

    //settings----------------------------------------------------

    float frequency=10;
    float phase=90;

    EncSim& setFrequency(float f_Hz)
    {
        frequency = f_Hz;
        return setCountRate(frequency, phase);
    }

    EncSim& setPhase(float deg)
    {
        phase = deg;
        return setCountRate(frequency, phase);
    }

    EncSim& setCountRate(float f_Hz, float phase_deg = 90.0)
    {
        constexpr float minPhase = 5.f;
        constexpr float maxPhase = 90.f;
        constexpr unsigned minCycles = 80u;

        float cycles = max(minCycles, F_BUS / f_Hz - 1);
        float phase = max(minPhase, min(maxPhase, phase_deg));

        T[0] = max(minCycles, cycles * phase / 90.0);
        T[1] = 2.0 * cycles - T[0];

        f = (2.0 * F_BUS) / (T[0] + T[1] + 2);

        return *this;
    }

    EncSim& setTotalBounceDuration(unsigned microseconds)
    {
        phaseA.totalBounceTime = microseconds;
        phaseB.totalBounceTime = microseconds;
        return *this;
    }

    EncSim& setBounceDurationMin(unsigned microseconds)
    {
        phaseA.minBounceTime = microseconds;
        phaseB.minBounceTime = microseconds;
        return *this;
    }

    EncSim& setBounceDurationMax(unsigned microseconds)
    {
        phaseA.maxBounceTime = microseconds;
        phaseB.maxBounceTime = microseconds;
        return *this;
    }

    void printSettings(Stream& s)
    {       
        float phase = 180.0 * T[0] / (T[1] + T[0]);

        s.printf("  freq:      %.0f Hz\n", f);
        s.printf("  phase:     %.0f\u00B0\n", phase);
        s.printf("  btot:      %d \u00B5s\n", phaseA.totalBounceTime);
        s.printf("  bmin:      %d \u00B5s\n", phaseA.minBounceTime);
        s.printf("  bmax:      %d \u00B5s\n", phaseA.maxBounceTime);       
        s.println();
    }

    volatile int current;
    int direction;
    int target;

private:
    float f = 0;  
    unsigned T[2];    

    BouncingPin<A, 0> phaseA;
    BouncingPin<B, 1> phaseB;

    PIT mainTimer;
};


