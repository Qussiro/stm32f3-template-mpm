# put your *.o targets here, make should handle the rest!
SRCS = main.c system_stm32f30x.c

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJ_NAME=main

# Location of the linker scripts
LDSCRIPT_INC=device/ldscripts

# location of OpenOCD Board .cfg files (only used with 'make program')
OPENOCD_BOARD_DIR=/usr/share/openocd/scripts/board

# Configuration (cfg) file containing programming directives for OpenOCD
OPENOCD_PROC_FILE=extra/stm32f3-openocd.cfg

# Location of the individual libraries and the spells to cast in order to use them
PATHLIBCORE = libraries/lib-core
INCLLIBCORE = -I$(PATHLIBCORE)/CMSIS/Device/ST/STM32F30x/Include -I$(PATHLIBCORE)/CMSIS/Include 

PATHLIBPERIPH = libraries/lib-mcu
INCLLIBPERIPH = -I$(PATHLIBPERIPH) -I$(PATHLIBPERIPH)/STM32F30x_StdPeriph_Driver/inc -include $(PATHLIBPERIPH)/stm32f30x_conf.h
STATLIBPERIPH = -L$(PATHLIBPERIPH) -lstm32f3

PATHLIBBOARD = libraries/lib-board
INCLLIBBOARD = -I$(PATHLIBBOARD)/Utilities
STATLIBBOARD = -L$(PATHLIBBOARD) -lstm32f3b

# tools in toolchain
CC=arm-unknown-eabi-gcc
OBJCOPY=arm-unknown-eabi-objcopy
OBJDUMP=arm-unknown-eabi-objdump
SIZE=arm-unknown-eabi-size

# compiler flags
CFLAGS  = -Wall -g -std=c99 -Os
# TODO next line seems facultative if the used toolchain
#      has the architecture hardcoded
#      we should try not using it
CFLAGS += -mlittle-endian -mcpu=cortex-m4 -march=armv7e-m -mthumb
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections -Wl,-Map=$(PROJ_NAME).map

CFLAGS += -I inc
CFLAGS += $(INCLLIBCORE)
CFLAGS += $(INCLLIBPERIPH)  # comment this line if you dont use standard peripheral library
CFLAGS += $(INCLLIBBOARD)   # comment this line if you dont use the board library

# linker flags
# TODO inclusion order does not allow for efficient commenting out
LDFLAGS  = $(STATLIBBOARD)  # comment this line if you dont use the board library
LDFLAGS += $(STATLIBPERIPH) # comment this line if you dont use standard peripheral library
LDFLAGS += -L$(LDSCRIPT_INC) -Tstm32f3.ld
###################################################

# startup code written is assembly
# contains code of importat handlers activated in the startup sequence
SRCS  = device/startup_stm32f30x.s
SRCS += src/system_stm32f30x.c
SRCS += src/stm32f30x_it.c
SRCS += src/main.c

OBJS = $(SRCS:.c=.o)

###################################################

all: lib proj

lib:
	$(MAKE) -C $(PATHLIBPERIPH)
	$(MAKE) -C $(PATHLIBBOARD)

proj: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(OBJDUMP) -St $(PROJ_NAME).elf >$(PROJ_NAME).lst
	$(SIZE) $(PROJ_NAME).elf
	
program: $(PROJ_NAME).bin
	openocd -f $(OPENOCD_BOARD_DIR)/stm32f3discovery.cfg -f $(OPENOCD_PROC_FILE) -c "stm_flash `pwd`/$(PROJ_NAME).bin" -c shutdown

clean:
	find ./ -name '*~' | xargs rm -f
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).map
	rm -f $(PROJ_NAME).lst

deepclean: clean
	$(MAKE) -C $(PATHLIBPERIPH) clean
	$(MAKE) -C $(PATHLIBBOARD) clean
