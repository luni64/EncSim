#include "EncSim.h"

EncSim encSim(0, 1, 2);           // A=0, B=1, Z=2

constexpr float rpm  = 800;       // revolutions per minute
constexpr float ppr  = 32768;     // encoder pulses per revolution
constexpr float rps  = rpm / 60;  // revolution per second11
constexpr float freq = rps * ppr; // count frequency

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    while (!Serial) {}

    Serial.printf("rpm:\t%8.2f 1/min\n", rpm);
    Serial.printf("pps:\t%8.2f kHz\n", freq / 1000.0f);
    Serial.printf("phase freq:\t%8.2f kHz\n", freq / 4000.0f);
    Serial.printf("z-freq :\t%8.2f Hz\n", rps);

    encSim
        .begin()
        .setFrequency(freq)         // calculated above
        .setPeriod(ppr)             // set above
        .setPhase(90)               // 90° phase signal
        .setTotalBounceDuration(0); // no bouncing

    encSim.moveRelAsync(INT32_MAX); // run for some time INT32_MAX = 2147483647
}

void loop()
{
    digitalToggleFast(LED_BUILTIN); // heart beat
    delay(250);
}