# Commands
CROSS_COMPILE ?= or1k-elf-
CC = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)g++
LD =  $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
REMOVE = rm -rf
FEL ?= sunxi-fel

# Compiler flags
CFLAGS = -O2 -fno-common -fno-builtin -ffreestanding

# Linker flags
LDFLAGS = -static -nostartfiles -lgcc

# Sources
TARGET = main.c
SRC = $(TARGET)
SRC += uart.c
SRC += gpio.c
SRC += dram_sun6i.c

SSRC = start.S

# Output filename
OUT = ar100-info

# Object defines
COBJ = $(SRC:.c=.o)
SOBJ = $(SSRC:.S=.o)

all: $(SRC) $(OUT) $(OUT).bin $(OUT).code

$(OUT).code: $(OUT).bin
	srec_cat $< -Binary -Byte_Swap 4 -Output $@ -Binary

$(OUT).bin: $(OUT)
	$(OBJCOPY) -O binary $< $@

$(OUT): $(COBJ) $(SOBJ)
	$(CC) $(LDFLAGS) -T$@.ld $(SOBJ) $(COBJ) -o $@

$(COBJ) : %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SOBJ) : %.o : %.S
	$(CC) $(CFLAGS) -c $< -o $@

load: $(OUT).code
	$(FEL) write 0x40000 $<
	$(FEL) exe 0x44000

clean:
	$(REMOVE) *~ $(COBJ) $(SOBJ) $(OUT) $(OUT).bin $(OUT).code
