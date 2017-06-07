#pragma once

#include "EncSim.h"
#include <functional>
#include <limits>

template<typename Sim>
class SimulatorStateMachine
{
  public:
    using sm = SimulatorStateMachine<Sim>;

    void begin()
    {
      simulator.begin();
    }

    void moveAbs(int t)
    {
      intParam1 = t;
      currentState(this, doMove);
    }

    void moveRel(int delta)
    {
      intParam1 = simulator.current + delta;
      currentState(this, doMove);
    }

    void moveRandom(int min, int max)
    {
      intParam1 = max;
      intParam2 = min;
      currentState(this, doMoveRandom);
    }

    void moveUp()
    {
      intParam1 = std::numeric_limits<int>::max();
      currentState(this, doMove);
    }

    void moveDown()
    {
      intParam1 = std::numeric_limits<int>::min();
      currentState(this, doMove);
    }

    void stop()
    {
      currentState(this, doStop);
    }

    void getCount()
    {
      currentState(this, doGetCount);
    }

    void setCount(int cnt)
    {
      intParam1 = cnt;
      currentState(this, doSetCount);
    }

    void print()
    {
      currentState(this, doPrintSettings);
    }

    void setFrequency(int freq)
    {
      intParam1 = freq;
      currentState(this, doSetFreq);
    }

    void setPhase(int phase)
    {
      intParam1 = phase;
      currentState(this, doSetPhase);
    }

    void setBounceTotal(int t)
    {
      intParam1 = t;
      currentState(this, doSetBounceTime);
    }


    void setBounceMax(int t)
    {
      intParam1 = t;
      currentState(this, doSetBounceMax);
    }

    void setBounceMin(int t)
    {
      intParam1 = t;
      currentState(this, doSetBounceMin);
    }

    void tick()
    {
      currentState(this, doTick);
    }


    SimulatorStateMachine()
    {
      currentState = &sm::stateIdle;
    }

  private:
    enum stateEvent
    {
      doMove, doMoveRandom, doStop,
      doGetCount, doSetCount,
      doSetFreq, doSetPhase,
      doSetBounceTime, doSetBounceMin, doSetBounceMax,
      doHelp,
      doPrintSettings,
      finished,
      doTick,
    };

    std::function<void(sm*, stateEvent)> currentState;

    Sim simulator;
    //Simulator<2, 3> simulator;

    int intParam1, intParam2;
    float floatParam;

    //----------------------------------------------------------------------
    // IDLE

    void stateIdle(stateEvent e)
    {
      switch (e)
      {
        case doMove:
          simulator.moveAbsAsync(intParam1);
          currentState = &sm::stateMoving;
          break;

        case doGetCount:
          Serial.printf("  %d\n", simulator.current);
          break;

        case doSetCount:
          simulator.current = intParam1;
          break;

        case doSetFreq:
          simulator.setFrequency(intParam1);
          break;

        case doSetPhase:
          simulator.setPhase(intParam1);
          break;

        case doSetBounceTime:
          simulator.setTotalBounceDuration(intParam1);
          break;

        case doSetBounceMin:
          simulator.setBounceDurationMin(intParam1);
          break;

        case doSetBounceMax:
          simulator.setBounceDurationMax(intParam1);
          break;

        case doPrintSettings:
          simulator.printSettings(Serial);
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
          simulator.stop();
          break;

        case doMove:
          simulator.moveAbsAsync(intParam1);
          break;

        case doTick:
          static unsigned timeout;
          if (millis() > timeout)
          {
            timeout = millis() + 200;
            if (!simulator.isRunning()) {
              currentState = &sm::stateIdle;
            }
            Serial.printf("  %d\n", simulator.current);
          }
          break;

        default:
          Serial.println("  command ignored");
          break;
      }
    }
};

