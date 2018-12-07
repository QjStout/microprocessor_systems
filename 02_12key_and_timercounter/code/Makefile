GCCFLAGS=-g -Os -Wall -mmcu=atmega168 
LINKFLAGS=-Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm
AVRDUDEFLAGS=-c avr109 -p m168 -b 115200 -P COM5
LINKOBJECTS=../libnerdkits/delay.o ../libnerdkits/lcd.o ../libnerdkits/uart.o

all:	Assignment2-upload

Assignment2.hex:	Assignment2.c
	make -C ../libnerdkits
	avr-gcc ${GCCFLAGS} ${LINKFLAGS} -o Assignment2.o Assignment2.c ${LINKOBJECTS}
	avr-objcopy -O ihex Assignment2.o Assignment2.hex
	
Assignment2.ass:	Assignment2.hex
	avr-objdump -S -d Assignment2.o > Assignment2.ass
	
Assignment2-upload:	Assignment2.hex
	avrdude ${AVRDUDEFLAGS} -U flash:w:Assignment2.hex:a
