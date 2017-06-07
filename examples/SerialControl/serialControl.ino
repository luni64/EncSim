#include <TeensyDelay.h>
#include <EncSim.h>
#include "commandInterpreter.h"


CommandInterpreter<EncSim<0,1>> interpreter;

void setup() {
    interpreter.begin();
}

void loop() {    
    interpreter.tick();    
}
