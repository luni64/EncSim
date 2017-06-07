#pragma once

#include "SerialCommand.h"
#include "StateMachine.h"

template<class EncSim>
class CommandInterpreter
{
public:
    void begin()
    {
        serialCommand.addCommand("mva", mva);
        serialCommand.addCommand("mvr", mvr);
        serialCommand.addCommand("up", up);
        serialCommand.addCommand("down", down);
        serialCommand.addCommand("stop", stop);
        serialCommand.addCommand("getpos", getpos);
        serialCommand.addCommand("setpos", setpos);
        serialCommand.addCommand("freq", freq);
        serialCommand.addCommand("phase", phase);
        serialCommand.addCommand("btot", btotal);
        serialCommand.addCommand("bmin", bmin);
        serialCommand.addCommand("bmax", bmax);

        serialCommand.addCommand("print", print);
        serialCommand.addCommand("help", help);
        serialCommand.addCommand("?", help);

        serialCommand.setDefaultHandler(unrecognized);
        sm.begin();
    }

    void tick()
    {
        serialCommand.readSerial();
        sm.tick();     
    }

private:
    static void unrecognized(const char *command) { Serial.printf("Command Error: %s\n", command); }
    static void mva();
    static void mvr();
    static void up();
    static void down();
    static void stop();
    static void getpos();
    static void setpos();
    static void freq();
    static void phase();
    static void btotal();
    static void bmin();
    static void bmax();
    static void print();
    static void help();
            
    static SerialCommand serialCommand;
    static SimulatorStateMachine<EncSim> sm;
 
    static int getIntParam()
    {
        char *param = serialCommand.next();
        return param != NULL ? atoi(param) : 0;
    }  
};


template<class Simulator>
void CommandInterpreter<Simulator>::mva()
{
    int target = getIntParam();
    Serial.printf("> mva %d\n", target);
    sm.moveAbs(target);
}

template<class Simulator>
void CommandInterpreter<Simulator>::mvr()
{
    int delta = getIntParam();
    Serial.printf("> mvr %d\n", delta);
    sm.moveRel(delta);
}

template<class Simulator>
void CommandInterpreter<Simulator>::up()
{
    Serial.printf("> up\n");
    sm.moveUp();
}

template<class Simulator>
void CommandInterpreter<Simulator>::down()
{
    Serial.printf("> down\n");
    sm.moveDown();
}

template<class Simulator>
void CommandInterpreter<Simulator>::stop()
{   
    Serial.printf("> stop\n");
    sm.stop();
}

template<class Simulator>
void CommandInterpreter<Simulator>::getpos()
{
    Serial.printf("> getpos\n");
    sm.getCount();
}

template<class Simulator>
void CommandInterpreter<Simulator>::setpos()
{
    int cnt = getIntParam();
    Serial.printf("> setpos %d\n",cnt);    
    sm.setCount(cnt);
}

template<class Simulator>
void CommandInterpreter<Simulator>::freq()
{
    int freq = getIntParam();
    Serial.printf("> freq %d\n", freq);
    sm.setFrequency(freq);
}

template<class Simulator>
void CommandInterpreter<Simulator>::phase()
{
    int phase = getIntParam();
    Serial.printf("> phase %d\n", phase);
    sm.setPhase(phase);
}

template<class Simulator>
void CommandInterpreter<Simulator>::btotal()
{
    int t = getIntParam();
    Serial.printf("> btotal %d\n", t);
    sm.setBounceTotal(t);
}

template<class Simulator>
void CommandInterpreter<Simulator>::bmin()
{
    int t = getIntParam();
    Serial.printf("> bmin %d\n", t);
    sm.setBounceMin(t);
}

template<class Simulator>
void CommandInterpreter<Simulator>::bmax()
{
    int t = getIntParam();
    Serial.printf("> bmax %d\n", t);
    sm.setBounceMax(t);
}

template<class Simulator>
void CommandInterpreter<Simulator>::print()
{
    Serial.printf("> print\n");
    sm.print();
}

template<class Simulator>
void CommandInterpreter<Simulator>::help()
{
    Serial.println("\nUsage");
    Serial.println("  mva pos    Move to absolute position");
    Serial.println("  mvr pos    Move to relative position");
    Serial.println("  up         Move up (same as mva INT_MAX)");
    Serial.println("  down       Move down (same as mva -INT_MAX)");
    Serial.println("  stop       Stop movement and show current position");
    Serial.println("  getpos     Show current position");
    Serial.println("  setpos pos Set current position to new value without generating output");
    Serial.println("  freq f     Set frequency to f; unit: Hz; default: 100");
    Serial.println("  phase p    Set phase; unit: deg; default: 90");
    Serial.println("  btot       Set total bouncing time; unit: \u00B5s, (0 disables bouncing); default: 0");
    Serial.println("  bmin       Set shortest duration of bouncing peaks; unit: \u00B5s, default: 50");
    Serial.println("  bmax       Set longest duration of bouncing peaks; unit: \u00B5s, default: 1000");
    Serial.println("  print      Print settings");
    Serial.println();

}


template<class Simulator>
SerialCommand CommandInterpreter<Simulator>::serialCommand;

template<class Simulator>
SimulatorStateMachine<Simulator> CommandInterpreter<Simulator>::sm;

