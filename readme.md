# EncSim
- [Library Description](#library-description)
- [Serial Interface](#serial-interface)
- [API](#api)

## Library Description 
EncSim can be used for testing the performance of encoder libraries and/or hardware in a manual or automated test environment. It generates a quadrature signal which simulates optical or mechanical incremental [encoders](https://en.wikipedia.org/wiki/Rotary_encoder#Incremental_rotary_encoder). You can adjust the **direction**, **count rate** and the **phase** of the generated signal. For simulating mechanical encoders you can add  random [**contact bouncing**](https://en.wikipedia.org/wiki/Switch#Contact_bounce) (aka chatter) peaks to the signal. EncSym uses [TeensyDelay](https://github.com/luni64/TeensyDelay) for generation of the random bounce peaks.

### Key Features
- Easy to use interface to control signal generation with a serial terminal.
- Count rate adjustable from 1Hz up to 1.4 MHz.
- Signal phase adjustable from 90° (standard) to 10°.
- Bouncing:
  - Total bounce time adjustable.
  - Minimum width of random bounce peaks adjustable.
  - Maximum width of random bouce peaks adjustable.
- Programming API for use in own sketches, automated tests or simular applications.

**Precompiled Firmware** 
If you just want to use EncSym for testing your encoder library you do not need to install EncSim. You can download precompiled hex files for various Teensy boards instead. Just upload the firmware, open a serial terminal (arduinos serial monitor, [TyCommander](https://github.com/Koromix/tytools),  [PuTTY](http://www.putty.org/), ...) and start testing. Hera a link to the [firmware download](https://github.com/luni64/EncSim/releases/tag/V0.9-beta) page.

### Examples of generated signals
**Bouncing:** 
The follwing two images show the simulation of a mechanical encoder. The pulse rate was set to 50Hz and a total bounce time of 5 ms was choosen. The min and max duration of the bounce pulses was set to 20µs and 500µs respectively. The first image shows an overview, the second show the same data zoomed in.

![50Hz with enabled bouncing](/media/50Hz_bounce5000_20_500.PNG?raw=true "50 Hz bouncing")
![50Hz with enabled bouncing](/media/50Hz_bounce5000_20_500_zoom.PNG?raw=true "50 Hz bouncing")

**Phase Adjustment:**  
Below the generated signal for a pulse rate of 100kHz and a signal phase of 45° is shown.

![100kHz, 45deg](/media/100kHz45deg.PNG?raw=true "100KHz, 45deg")

**Maximum Pulse Rate:**

The last example shows a signal with the maximum possible pulsrate of 1.4 MHz. You'll need a T3.6 @240MHz with F_BUS=120MHz to be able to get such high pulse rates. 

![100kHz, 45deg](/media/1_4MHz.PNG?raw=true)

*** 

# Serial Interface
The signal generation can be controlled by an easy to use [serial interface](https://github.com/luni64/EncSim/tree/master/examples/SerialControl). You can use the Aduino serial monitor,  [TyCommander](https://github.com/Koromix/tytools) or any other serial terminal to send commands to the library. Internally the serial inteface uses Kroimons [SerialCommand](https://github.com/kroimon/Arduino-SerialCommand) lib for communication with the terminal.

**Commands:**

Typing in *help* or *?* in the terminal shows a list of all available commands:

![help output](/media/interface.PNG?raw=true)

**Command description**

- The *up* and *down* commands generate a continous quadrature signal until you send the *stop* command. The current counter value is printed during the 'movement'.
- *mva* and *mvr* 'move' to a given absolute or relative position respectively. Current counter value is printed during amd after the move.
- Each move command can be intercepted by *stop* or other move commands. E.g., sending *up* followed by *down* will change the direction of the signal immediately. Sending *up* followed by *mva 0* will change direction and move back to position 0.
- *getpos* and *setpos* get and set the current counter value
- *freq*, *phase*, *btot*, *bmin* and *bmax* set pulse rate, phase, total bounce time, minimal and maximal bounce pulse width respectively.
- *print* shows the current settings.  Please note: the library clips the frequency and phase settings if the requested signal can not be generated with the given board. It is a good idea to check the actual settings with *print* if you use high frequencies and/or small phases settings.

# API
Using EncSim from your sketches without the serial interface is easy:
```c++
#include <TeensyDelay.h>
#include <EncSim.h>

EncSim<0,1> simulator;       // use pin 0 and pin 1 as output (any two digital pins can be used)

void setup() {
  simulator.begin();

  simulator                   // settings can be done by a "fluent interface"
  .setFrequency(150)          // 150Hz count rate
  .setPhase(90)               // normal 90° phase shift
  .setTotalBounceDuration(0); // no bouncing
  
  simulator.moveRel(100);     // generate 100 counts
}
```
The following settings are available
```c++
setFrequency(Hz)            // pulse rate in Hz (1-f_max), f_max depends on board and F_BUS settings
setPhase(deg)               // signal phase in deg (10-90)
setTotalBounceDuration(µs)  // total duration of bouncing phase in µs
setBounceDurationMin(µs)    // minimum peak width of a bouncing peak
setBounceDurationMax(µs)    // maximum peak width of a bouncing peak
```
The following commands handle signal generation
```c++
moveAbsAsync(pos)           // moves (generates pulses) to absolute position, returns after starting the move
moveRelAsync(delta)         // moves relative to current position
moveAbs(pos)                // moves to absolute position, blocks until movement is done
moveRes(delta)              // moves relative to current position, blocks until movement is done
stop()                      // stops movement immediately
bool isRunning()            // true if simulator is currently moving
```


