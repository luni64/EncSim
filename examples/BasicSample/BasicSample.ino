#include <TeensyDelay.h>
#include <EncSim.h>

EncSim<0,1> simulator;       // use pin 0 and pin 1 as output

void setup() {
  simulator.begin();

  simulator
  .setFrequency(150)          // 150Hz count rate
  .setPhase(90)               // normal 90° phase shift
  .setTotalBounceDuration(0); // no bouncing
}

void loop() {
  simulator.moveRel(15);      // relative movement 15 steps positive
  delay(100);
  simulator.moveAbs(0);       // move back to 0
  delay(100);
}
