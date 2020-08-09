#pragma once

#include "EncSim.h"
#include <functional>
#include <limits>

class SimulatorStateMachine
{
 public:
    SimulatorStateMachine(EncSim* s) : simulator(s){};

    void begin()
    {
        currentState = [this](stateEvent e) { this->stateIdle(e); };
        simulator->begin();
    }

    void moveAbs(int t)
    {
        intParam1 = t;
        currentState(doMove);
    }

    void moveRel(int delta)
    {
        intParam1 = simulator->current + delta;
        currentState(doMove);
    }

    void moveRandom(int min, int max)
    {
        intParam1 = max;
        intParam2 = min;
        currentState(doMoveRandom);
    }

    void moveUp()
    {
        intParam1 = std::numeric_limits<int>::max();
        currentState(doMove);
    }

    void moveDown()
    {
        intParam1 = std::numeric_limits<int>::min();
        currentState(doMove);
    }

    void stop()
    {
        currentState(doStop);
    }

    void getCount()
    {
        currentState(doGetCount);
    }

    void setCount(int cnt)
    {
        intParam1 = cnt;
        currentState(doSetCount);
    }

    void print()
    {
        currentState(doPrintSettings);
    }

    void setFrequency(int freq)
    {
        intParam1 = freq;
        currentState(doSetFreq);
    }

    void setPhase(int phase)
    {
        intParam1 = phase;
        currentState(doSetPhase);
    }

    void setBounceTotal(int t)
    {
        intParam1 = t;
        currentState(doSetBounceTime);
    }

    void setBounceMax(int t)
    {
        intParam1 = t;
        currentState(doSetBounceMax);
    }

    void setBounceMin(int t)
    {
        intParam1 = t;
        currentState(doSetBounceMin);
    }

    void tick()
    {
        currentState(doTick);
    }

 private:
    enum stateEvent {
        doMove,
        doMoveRandom,
        doStop,
        doGetCount,
        doSetCount,
        doSetFreq,
        doSetPhase,
        doSetBounceTime,
        doSetBounceMin,
        doSetBounceMax,
        doHelp,
        doPrintSettings,
        finished,
        doTick,
    };

    EncSim* simulator;

    std::function<void(stateEvent)> currentState;

    int intParam1, intParam2;
    float floatParam;

    //----------------------------------------------------------------------
    // IDLE

    void stateIdle(stateEvent e)
    {
        switch (e)
        {
            case doMove:
                simulator->moveAbsAsync(intParam1);
                currentState = [this](stateEvent e) { this->stateMoving(e); };
                break;

            case doGetCount:
                Serial.printf("  %d\n", simulator->current);
                break;

            case doSetCount:
                simulator->current = intParam1;
                break;

            case doSetFreq:
                simulator->setFrequency(intParam1);
                break;

            case doSetPhase:
                simulator->setPhase(intParam1);
                break;

            case doSetBounceTime:
                simulator->setTotalBounceDuration(intParam1);
                break;

            case doSetBounceMin:
                simulator->setBounceDurationMin(intParam1);
                break;

            case doSetBounceMax:
                simulator->setBounceDurationMax(intParam1);
                break;

            case doPrintSettings:
                simulator->printSettings(Serial);
                break;

            case doTick:
                break;

            default:
                Serial.println("  command ignored");
                break;
        }
    }

    //-------------------------------------------------------------------------------
    // MOVING

    void stateMoving(stateEvent e)
    {
        switch (e)
        {
            case doStop:
                simulator->stop();
                break;

            case doMove:
                simulator->moveAbsAsync(intParam1);
                break;

            case doTick:
                static unsigned timeout;
                if (millis() > timeout)
                {
                    timeout = millis() + 200;
                    if (!simulator->isRunning())
                    {
                        currentState = [this](stateEvent e) {this->stateIdle(e);};
                    }
                    Serial.printf("  %d\n", simulator->current);
                }
                break;

            default:
                Serial.println("  command ignored");
                break;
        }
    }
};
