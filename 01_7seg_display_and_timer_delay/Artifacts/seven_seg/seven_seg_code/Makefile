GCCFLAGS=-g -Os -Wall -mmcu=atmega168 
ASRC = disp)zero.S
LINKFLAGS=-Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm
AVRDUDEFLAGS=-c avr109 -p m168 -b 115200 -P COM4

LINKOBJECTS=../libnerdkits/delay.o ../libnerdkits/lcd.o ../libnerdkits/uart.o disp_zero.o


all:	seven_segment-upload

seven_segment.hex:	seven_segment.c
	make -C ../libnerdkits 
	avr-as disp_zero.S -o disp_zero.o
	avr-gcc ${GCCFLAGS} ${LINKFLAGS} -o seven_segment.o seven_segment.c ${LINKOBJECTS}
	avr-objcopy -j .text -O ihex seven_segment.o seven_segment.hex
	
seven_segment.ass:	seven_segment.hex
	avr-objdump -S -d seven_segment.o > seven_segment.ass
	
seven_segment-upload:	seven_segment.hex
	avrdude ${AVRDUDEFLAGS} -U flash:w:seven_segment.hex:a
