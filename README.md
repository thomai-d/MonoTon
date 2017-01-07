# MonoTon
Arduino based step sequencer and software-synth

![MonoTon1](https://github.com/thomai-d/MonoTon/blob/master/img/MonoTon1.png)

[![Youtube](http://img.youtube.com/vi/V6P4GdR-m2A/0.jpg)](http://www.youtube.com/watch?v=V6P4GdR-m2A)
[(Audio only)](https://github.com/thomai-d/MonoTon/blob/master/media/MonoTon1.mp3).

## Features
* 8 instruments (Kick, Hat, Snare, Shaker, Clap, Noise, Sine, Square)
* Each instrument has 4 banks a 16 steps each and can be muted
* Synthesis using 8bit wavetables
* 20000Hz sampling rate
* Master is filtered through analog high-pass filter (2pole)

## Hardware
* atmega1284 @ 16MHz using [Mighty1284 platform](https://github.com/maniacbug/mighty-1284p)
* lots of 74HC595 shift registers (for LEDs)
* MCP23S17 port extenders (for buttons)
* CD4067 multiplexer (for analog potis)
* R2R network for D/A conversion
* Some eagle files included

## Lessons learned while developing
* The analog inputs are quite noisy. De-noising uses quite much cpu.
* Don't ever solder this much wires by hand. PCB services are your friend.
* Prefer [DDS](http://interface.khm.de/index.php/lab/interfaces-advanced/arduino-dds-sinewave-generator/) over an R2R ladder.

## Open bugs / flaws
* Although the output is buffered by an opamp, the sound sounds distorted when using the PC's line in. If I conntect it directly to the speakers, the sound is quite clear.
