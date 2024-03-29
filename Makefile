all: compile-debug gdb

detect:
	mbed detect

deploy:
	mbed deploy

compile:
	mbed compile -t GCC_ARM -m NUCLEO_F446RE

copy:
	cp -v ./BUILD/NUCLEO_F446RE/GCC_ARM/midiplayer.bin /mnt/hd

copy-debug:
	cp -v ./BUILD/NUCLEO_F446RE/GCC_ARM-DEBUG/mbed-os-example-sh1106.bin /mnt/hd

screen:
	screen /dev/ttyACM0

compile-debug:
	mbed compile -t GCC_ARM -m NUCLEO_F446RE --profile mbed-os/tools/profiles/debug.json

clean:
	rm -Rvf BUILD

clean-all:
	rm -Rvf BUILD
	rm -Rvf mbed-os

add-mbed-os:
	mbed add mbed-os

openocd:
	openocd -f /usr/share/openocd/scripts/board/st_nucleo_f4.cfg

ddd: compile-debug
	ddd --debugger arm-none-eabi-gdb BUILD/NUCLEO_F446RE/GCC_ARM/mbed-os-example-sh1106.elf

gdb: compile-debug
	arm-none-eabi-gdb -batch -ex "target remote localhost:3333" -ex "load" -ex "continue" BUILD/NUCLEO_F446RE/GCC_ARM-DEBUG/mbed-os-example-sh1106.elf

