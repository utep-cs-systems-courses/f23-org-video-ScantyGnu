# makefile configuration

COMMON_OBJECTS  = main.o

CPU             = msp430g2553

CFLAGS          = -mmcu=${CPU} -I../h

LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include



#switch the compiler (for the internal make rules)

CC              = msp430-elf-gcc

AS              = msp430-elf-as



all: blinky.elf 



#additional rules for files

blinky.elf: ${COMMON_OBJECTS} ../lib/libTimer.a wdt_handler.s

	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd



load: blinky.elf

	msp430loader.sh $^



clean:

	rm -f *.o *.elf
