#pragma once

#include "EncSim.h"
#include "SerialCommand.h"
#include "StateMachine.h"

class CommandInterpreter
{
 public:
    CommandInterpreter(SimulatorStateMachine* statemachine) : sm(statemachine) {}

    void begin()
    {
        serialCommand.addCommand("mva", [this] { this->mva(); });
        serialCommand.addCommand("mvr", [this] { this->mvr(); });
        serialCommand.addCommand("up", [this] { this->up(); });
        serialCommand.addCommand("down", [this] { this->down(); });
        serialCommand.addCommand("stop", [this] { this->stop(); });
        serialCommand.addCommand("getpos", [this] { this->getpos(); });
        serialCommand.addCommand("setpos", [this] { this->setpos(); });
        serialCommand.addCommand("freq", [this] { this->freq(); });
        serialCommand.addCommand("phase", [this] { this->phase(); });
        serialCommand.addCommand("btot", [this] { this->btotal(); });
        serialCommand.addCommand("bmin", [this] { this->bmin(); });
        serialCommand.addCommand("bmax", [this] { this->bmax(); });
        serialCommand.addCommand("period", [this] { this->period(); });

        serialCommand.addCommand("print", [this] { this->print(); });
        serialCommand.addCommand("help", [this] { this->help(); });
        serialCommand.addCommand("?", [this] { this->help(); });
        serialCommand.setDefaultHandler([](const char* cmd) { Serial.printf("Command Error: %s\n", cmd); });

        help();

        sm->begin();
    }

    void tick()
    {
        serialCommand.readSerial();
        sm->tick();
    }

 private:
    void mva();
    void mvr();
    void up();
    void down();
    void stop();
    void getpos();
    void setpos();
    void freq();
    void phase();
    void btotal();
    void bmin();
    void bmax();
    void print();
    void help();
    void period();

    SerialCommand serialCommand;

    SimulatorStateMachine* sm;

    int getIntParam()
    {
        char* param = serialCommand.next();
        return param != NULL ? atoi(param) : 0;
    }
};

void CommandInterpreter::mva()
{
    int target = getIntParam();
    Serial.printf("> mva %d\n", target);
    sm->moveAbs(target);
}

void CommandInterpreter::mvr()
{
    int delta = getIntParam();
    Serial.printf("> mvr %d\n", delta);
    sm->moveRel(delta);
}

void CommandInterpreter::up()
{
    Serial.printf("> up\n");
    sm->moveUp();
}

void CommandInterpreter::down()
{
    Serial.printf("> down\n");
    sm->moveDown();
}

void CommandInterpreter::stop()
{
    Serial.printf("> stop\n");
    sm->stop();
}

void CommandInterpreter::getpos()
{
    Serial.printf("> getpos\n");
    sm->getCount();
}

void CommandInterpreter::setpos()
{
    int cnt = getIntParam();
    Serial.printf("> setpos %d\n", cnt);
    sm->setCount(cnt);
}

void CommandInterpreter::freq()
{
    int freq = getIntParam();
    Serial.printf("> freq %d\n", freq);
    sm->setFrequency(freq);
}

void CommandInterpreter::phase()
{
    int phase = getIntParam();
    Serial.printf("> phase %d\n", phase);
    sm->setPhase(phase);
}

void CommandInterpreter::btotal()
{
    int t = getIntParam();
    Serial.printf("> btotal %d\n", t);
    sm->setBounceTotal(t);
}

void CommandInterpreter::bmin()
{
    int t = getIntParam();
    Serial.printf("> bmin %d\n", t);
    sm->setBounceMin(t);
}

void CommandInterpreter::bmax()
{
    int t = getIntParam();
    Serial.printf("> bmax %d\n", t);
    sm->setBounceMax(t);
}

void CommandInterpreter::period()
{
    int p = abs(getIntParam());
    Serial.printf("> period %d\n", p);    
    sm->setPeriod(p);
}

void CommandInterpreter::print()
{
    Serial.printf("> print\n");
    sm->print();
}

void CommandInterpreter::help()
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
    Serial.println("  period p   Set period for index pulses; unit: counts; default: 100");
    Serial.println("  phase p    Set phase; unit: deg; default: 90");
    Serial.println("  btot       Set total bouncing time; unit: \u00B5s, (0 disables bouncing); default: 0");
    Serial.println("  bmin       Set shortest duration of bouncing peaks; unit: \u00B5s, default: 50");
    Serial.println("  bmax       Set longest duration of bouncing peaks; unit: \u00B5s, default: 1000");
    Serial.println("  print      Print settings");
    Serial.println();
}

// SerialCommand CommandInterpreter::serialCommand;

// SimulatorStateMachine CommandInterpreter::sm;
