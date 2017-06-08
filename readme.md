# Work in progress

# EncSim
- [Library Description](#description)
- [Serial Interface](#serial_interface)
    - Usage
    - Commands
    - Examples
- [API]()


## Library Description 
EncSim can be used to simulate signals from optical or mechanical incremental [encoders](https://en.wikipedia.org/wiki/Rotary_encoder#Incremental_rotary_encoder) (quadrature signals). You can adjust the generated **count rate** and the **phase** of the signal. For simulating mechanical encoders you can also add adjustable [**contact bouncing**](https://en.wikipedia.org/wiki/Switch#Contact_bounce) (aka chatter) to the signal.
The signal can be used for testing quadrature encoder libraries and/or hardware in a manaual or automated test environment.

### Key Features
- Easy to use control interface via a serial terminal available.
- Count rate adjustable from 1Hz up to 1.4 MHz (Teensy 3.6 @240MHz, F_BUS 120MHz).
- Signal phase adjustable from 90° (standard) to 10°.
- Random bouncing:
  - Total bounce time adjustable.
  - Minimum width of bounce peaks adjustable.
  - Maximum width of bouce peaks - adjustable.
- Programming API for use in automated tests or simular applications available.

**A few examples of generated signals:**
**Bouncing:** 
The next two images show the simulation of a mechanical encoder. The pulse rate was set to 50Hz and a total bounce time of 5 ms was choosen. The min and max duration of the bounce pulses was set to 20µs and 500µs respectively. First picture shows an overview, second picture shows the same data but zoomed in.

![50Hz with enabled bouncing](/media/50Hz_bounce5000_20_500.PNG?raw=true "50 Hz bouncing")
![50Hz with enabled bouncing](/media/50Hz_bounce5000_20_500_zoom.PNG?raw=true "50 Hz bouncing")

**Phase Adjustment:**  
The follwing image shows the genenrated signal for a pulse rate of 100kHz and a signal phase of 45°. 

![100kHz, 45deg](/media/100kHz45deg.PNG?raw=true "100KHz, 45deg")

**Maximum Pulse Rate:**

The last example shows maximum possible pulsrate of 1.4 MHz. You need a T3.6 @240MHz with F_BUS=120MHz to be able to get such high pulse rates.

![100kHz, 45deg](/media/1_4MHz.PNG?raw=true)

# Serial Interface
The signal generation can be controlled via an easy to use serial interface. You can use the Aduino serial monitor,  [TyCommander](#https://github.com/Koromix/tytools) or any other serial terminal to send commands to EncSim. Internally the serial inteface uses Kroimons [SerialCommand](https://github.com/kroimon/Arduino-SerialCommand) lib.

Typing in *help* or *?* in the terminal shows a list of all available commands:
![help output](/media/interface.PNG?raw=true)




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
  
  simulator.moveRel(100);     // generate 100 counts
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

