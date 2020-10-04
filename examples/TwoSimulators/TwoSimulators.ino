#include "EncSim.h"

EncSim e1(0, 1, 2); // A=0, B=1, Z=2
EncSim e2(3, 4, 5); // A=2, B=3; Z=5

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    e1
        .begin()
        .setFrequency(1000)         // 1000 pps
        .setPeriod(500)             // Index every 500 pulses
        .setPhase(90)               // 90° phase signal
        .setTotalBounceDuration(0); // no bouncing

    e2
        .begin()
        .setFrequency(7000)         // 1000 pps
        .setPeriod(1500)            // Index every 500 pulses
        .setPhase(90)               // 90° phase signal
        .setTotalBounceDuration(0); // no bouncing


    e1.moveRelAsync(INT32_MAX); // run for some time INT32_MAX = 2147483647
    e2.moveRelAsync(INT32_MAX); // run for some time INT32_MAX = 2147483647
}

void loop()
{
    Serial.printf("e1: %d, e2: %d\n", e1.current, e2.current);
    delay(100);
}