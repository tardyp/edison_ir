Edison Home Assistant with MCU driver
=====================================
This project implement infra red signal generation inside the edison MCU for use within home assistant.

I removed eclipse SDK boilerplate as I prefer to use plain makefile, and atom as an editor

'make'  will generate the edison binary
'make test' will generate test binary for testing on a development machine.
'python python/test_hitachy.py' will run a simple e2e test, by analysing the output of the fakemcu simulator

testing/fakemcu.c contains a basic simulation for the mcu api, and especially pwm api.
It generates a pwm.bin with 1 byte per sample and 1 us sampling.
