This is a set of simple SID player routines for Commodore 64 written in C:
* `sidplay.c` uses a simple loop to play patterns on every frame
* `sidplayint.c` uses raster interrupts to play patterns on every frame

To compile the program you need the following:
* [llvm-mos](https://github.com/llvm-mos/llvm-mos-sdk) compiler
* `c1541` utility from [VICE Commodore emulator](https://vice-emu.sourceforge.io/)

Ginger.sid is a tune by Kristian Røstøen and comes form the [High Voltage SID Collection](https://www.hvsc.c64.org/).
