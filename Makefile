SHELL:=/bin/bash
all: sid prg d64
sid:
	dd if=ginger.sid of=ginger.prg bs=1 skip=124
prg:
	mos-c64-clang -Os -o sidplay.prg sidplay.c
	mos-c64-clang -Os -o sidplayint.prg sidplayint.c
d64:
	c1541 -format "sidplay",id d64 sidplay.d64 -attach sidplay.d64 -write sidplay.prg sidplay -write sidplayint.prg sidplayint -write ginger.prg ginger
clean:
	rm -f *.prg *.elf *.d64
