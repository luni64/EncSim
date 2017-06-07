## Content
 - [EncSim Libary](#encsim)
    -  [Key Features](#key_features)
    -  [Usage](#usage)
 - [Serial Interface](#serial_interface)
 
# EncSim
Generates a quadrature signal for testing quadrature encoder libraries and/or hardware. The library can also be used to set up a automated testing environment.
The examples include a convenient [serial interface](#serial_interface) to control the signal generation. 

## Key Features
- Count rate adjustable from 1Hz up to 1.4 MHz (Teensy 3.6 @240MHz, F_BUS 90MHz)
- Signal phase adjustable from 90° (standard) to 10°
- Random bouncing
-- total bounce time - adjustable
-- minimum width of bounce peaks - adjustable
-- maximum width of bouce peaks - adjustable

## Usage
Basic usage of the library
```c++
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
```
The following settings are available for the *EncSim* class:
```c++
setFrequency(Hz)            // pulse rate in Hz (1-f_max), f_max depends on board and F_BUS settings
setPhase(deg)               // signal phase in deg (10-90)
setTotalBounceDuration(µs)  // total duration of bouncing phase in µs
setBounceDurationMin(µs)    // minimum peak width of a bouncing peak
setBounceDurationMax(µs)    // maximum peak width of a bouncing peak
```
Commands:
```c++
moveAbsAsync(pos)           // moves (generates pulses) to absolute position, returns after starting the move
moveRelAsync(delta)         // moves relative to current position
moveAbs(pos)                // moves to absolute position, blocks until movement is done
moveRes(delta)              // moves relative to current position, blocks until movement is done
stop()                      // stops movement immediately
bool isRunning()            // true if simulator is currently moving
```
# Serial Interface
tbd

