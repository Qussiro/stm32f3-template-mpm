CC=arm-unknown-eabi-gcc
AR=arm-unknown-eabi-ar

###########################################
PATHLIBCORE  = ../lib-core/CMSIS
INCLLIBCORE  = -I$(PATHLIBCORE)/Include -I$(PATHLIBCORE)/Device/ST/STM32F30x/Include

PATHLIBPERIPH  = ../lib-mcu
INCLLIBPERIPH  = -I$(PATHLIBPERIPH) -I$(PATHLIBPERIPH)/STM32F30x_StdPeriph_Driver/inc
INCLLIBPERIPH += -include $(PATHLIBPERIPH)/stm32f30x_conf.h

vpath %.c ./Utilities
#vpath %.a $(PATHLIBPERIPH)

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -march=armv7e-m
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -I. -I inc 
CFLAGS += $(INCLLIBCORE) $(INCLLIBPERIPH)

SRCS = stm32f3_discovery.c stm32f3_discovery_l3gd20.c stm32f3_discovery_lsm303dlhc.c
 
OBJS = $(SRCS:.c=.o)

#.PHONY: lib_Utilities.a

all: libstm32f3b.a

lib:
	$(MAKE) -C $(STD_PERIPH_LIB)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libstm32f3b.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libstm32f3b.a
