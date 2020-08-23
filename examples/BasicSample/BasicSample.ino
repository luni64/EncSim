#include "EncSim.h"

EncSim simulator(0,1);       // use pin 0 and pin 1 as output

void setup() {
  simulator.begin();

  simulator
  .setFrequency(150)          // 150Hz count rate
  .setPhase(90)               // normal 90� phase shift
  .setTotalBounceDuration(0); // no bouncing

  simulator.moveRel(200);      // relative movement 15 steps positive
  delay(1000);
  simulator.moveAbs(0);       // move back to 0
  delay(1000);
}

void loop()
{
}