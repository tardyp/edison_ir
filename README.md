Edison Home Assistant with MCU driver
=====================================
This project implement infra red signal generation inside the edison MCU for use within home assistant.

I removed eclipse SDK boilerplate as I prefer to use plain makefile, and atom as an editor

* 'make'  will generate the edison binary (provided you have the mcu sdk installed and MCUSDK_PATH MCUSDK_OS environment variable set)

* 'make test' will generate test binary for testing on a development machine.

* 'python python/test_hitachi.py' will run a simple e2e test, by analysing the output of the fakemcu simulator

testing/fakemcu.c contains a basic simulation for the mcu api, and especially pwm api.
It generates a pwm.bin with 1 byte per sample and 1 us sampling.

The python/hitachi.py uses this IR generator to generate signals for my hitachi AC.
The IR generator shall be generic enough to generate all kinds of signals although I haven't tested it yet.

The reverse engineering is based on the work here
https://perhof.wordpress.com/2015/03/29/reverse-engineering-hitachi-air-conditioner-infrared-remote-commands/
Even if the protocol for my own Hitachi is slightly different.

This IR generator generates two start pulse + N pulses where N is the length of the bitstream to send.
It encodes the bits as the pause number.

This IR generator hardcodes port 0 as the PWM port, which corresponds to the PIN 3 of the arduino shield.
This has been tested with Grove-IR.
http://wiki.seeedstudio.com/wiki/Grove_-_Infrared_Emitter


The cmd to send to /dev/ttymcu0 is 'IRCODE' + hexdecimal buffer

The buffer is as follow:

* u16: PULSE_FREQ  : The frequency of the PWM (usually 36000)
* u16: START_PULSE : The length of the first start pulse (us)
* u16: START_PAUSE : The pause after the first start pulse (us)
* u16: START_PULSE2: The length of the second start pulse (us)
* u16: START_PAUSE2: The length of the second start pulse (us)

* u16: PULSE_LEN   : The length of the pulse encoding data
* u16: PAUSE_HIGH  : The length of the pause when data bit is 1
* u16: PAUSE_LOW   : The length of the pause when data bit is 0

* u8 * N: The bytes to transmit. Least significant bit is transmitted first.
