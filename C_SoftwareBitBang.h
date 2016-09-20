
#ifndef SN
#define SN

/* SoftwareBitBang One wire interrupts-less implementation
   Part of the PJON framework (included in version v3.0)
   Copyright (c) 2012-2016, Giovanni Blu Mitolo All rights reserved.

Performance

Using only digitalWriteFast() micros() and delayMicroseconds() fast cross-architecture
communication speed can be achieved. SoftwareBitBang works in 3 different transmission
modes, STANDARD, FAST and OVERDRIVE:

  STANDARD: 16944Bd or 2.12kB/s cross-architecture, promiscuous clock compatible.

  FAST: 25157Bd or 3.15kB/s cross-architecture, promiscuous clock compatible.

  OVERDRIVE: Runs a specific architecture at its maximum limits (non cross-architecture
  compatible). Every architecture has its own limits, Arduino Duemilanove for example
  runs at 33898Bd or 4.23kB/s, Arduino Zero can reach 48000Bd or 6.00kB/s.

Compatibility

- ATmega88/168/328 16Mhz (Diecimila, Duemilanove, Uno, Nano, Mini, Lillypad)
- ATmega2560 16Mhz (Arduino Mega)
- ATmega16u4/32u4 16Mhz (Arduino Leonardo)
- ATtiny45/85 8Mhz, see https://github.com/gioblu/PJON/wiki/ATtiny-interfacing
- SAMD (Arduino Zero)
- ESP8266 v.1-7 80Mhz "AI-THINKER AT" firmware, see https://github.com/esp8266/Arduino
- ESP8266 NodeMCU v0.9-1.0 80Mhz, see https://github.com/esp8266/Arduino
- MK20DX256 96Mhz (Teensy 3.1)

Why not interrupts?

Usage of libraries is really extensive in the Arduino environment and often the
end user is not able to go over collisions or redefinitions. Very often a library
is using hardware resources of the microcontroller as timers or interrupts, colliding
or interrupting other libraries. This happens because in general Arduino boards have
limited hardware resources. To have a universal and reliable communication medium
in this sort of environment, software emulated bit-banging, I think, is a good, stable
and reliable solution that leads to "more predictable" results than interrupt driven
systems coexisting on small microcontrollers without the original developer and the
end user knowing about it.

Known issues

1) A pull down resistor in the order of mega ohms could be necessary on the bus to
reduce interference. See https://github.com/gioblu/PJON/wiki/Deal-with-interference

2) Consider that this is not an interrupt driven system and so all the time passed
in delay or executing something a certain amount of packets could be potentially
lost unheard, the packet manager of PJON will do its job scheduling the packet
to be sent again in future until is received or MAX_ATTEMPTS sending attempts is
reached, but a certain amount of bandwidth can be wasted. Structure intelligently
your loop cycle to avoid huge blind timeframes.

3) SoftwareBitBang strategy can have compatibility issues with codebases that
are using interrupts in their procedure, like for example the Servo library.
Reliability or bandwidth loss can be experienced because of the cyclical
interruptions made by third party interrupt driven software to the PJON code.



STANDARD transmission mode performance:
   Transfer speed: 16.944kBb or 2.12kB/s
   Absolute  communication speed: 1.81kB/s (data length 20 of characters)
   Data throughput: 1.51kB/s (data length 20 of characters) */
#define _SWBB_STANDARD  0

/* FAST transmission mode performance:
   Transfer speed: 25.157kBd or 3.15kB/s
   Absolute  communication speed: 2.55kB/s (data length 20 of characters)
   Data throughput: 2.13kB/s (data length 20 of characters) */
#define _SWBB_FAST      1

/* OVERDRIVE transmission mode performance:
   Architecture / setup dependant, see Timing.h */
#define _SWBB_OVERDRIVE 2

/* Set here the selected transmission mode - default STANDARD */
#define _SWBB_MODE _SWBB_STANDARD

#include "C_Timing.h"
#include "C_PJON.h"




#define NOT_ASSIGNED 255
#define FAIL 	0x100

boolean can_start(uint8_t input_pin, uint8_t output_pin);
uint8_t read_byte(uint8_t pin);
uint16_t receive_byte(uint8_t input_pin, uint8_t output_pin);
uint16_t receive_response(uint8_t input_pin, uint8_t output_pin);
void send_byte(uint8_t b, uint8_t input_pin, uint8_t output_pin);
uint16_t receive_response(uint8_t input_pin, uint8_t output_pin);
void send_response(uint8_t response, uint8_t input_pin, uint8_t output_pin);
uint16_t send_string(uint8_t id, char *string, uint8_t length, uint8_t header);
uint8_t syncronization_bit(uint8_t pin);

#define false 0
#define true 1


#endif
