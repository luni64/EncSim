#include <TeensyDelay.h>
#include <EncSim.h>
#include "commandInterpreter.h"


CommandInterpreter<EncSim<0,1>> interpreter;  //sets up pins 0 and 1 as output for the quadrature signal

void setup() {
    interpreter.begin();
}

void loop() {    
    interpreter.tick();    
}
