#include "EncSim.h"

EncSim encSim(0, 1, 2); // A=0, B=1, Z=2

void setup()
{
    encSim
        .begin()
        .setFrequency(10'000)       // 10 kCounts per second
        .setPeriod(4096)            // Index pulse every 4096 counts
        .setPhase(40)               // 40° phase signal
        .setTotalBounceDuration(0); // no bouncing

    encSim.moveRelAsync(INT32_MAX); // run for some time
}

void loop()
{
    Serial.println(encSim.current); // print current 'position'
    delay(100);
}
