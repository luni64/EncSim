#include "CommandInterpreter.h"
#include "EncSim.h"
#include "TeensyTimerTool.h"

auto* simulator    = new EncSim(0, 1, 2); // A=0, B=1, Z=2
auto* stateMachine = new SimulatorStateMachine(simulator);
auto* interpreter  = new CommandInterpreter(stateMachine);

void setup()
{
    while (!Serial) {}

    Serial.println("EncSim, Encoder Simulator v2.0.0.0");
    Serial.print("----------------------------------\n");
    interpreter->begin();

    Serial.println("Default Settings");
    stateMachine->print();
}

void loop()
{
    interpreter->tick();
}

void yield() // we do not need eventresponder or Serial events here...
{
    TeensyTimerTool::tick();
}