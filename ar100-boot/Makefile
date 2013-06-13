# Commands
CROSS_COMPILE ?= arm-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)g++
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
REMOVE = rm -rf

# Compiler flags
CFLAGS = -marm -mno-thumb-interwork -mabi=aapcs-linux -march=armv7-a \
	-fno-common -fno-builtin -ffreestanding -msoft-float

# Linker flags
LDFLAGS = $(CFLAGS) --gc-sections -static -nostartfiles

# Sources
TARGET = main.c
SRC = $(TARGET)

SSRC = start.S

# Output filename
OUT = ar100-boot

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

clean:
	$(REMOVE) *~ $(COBJ) $(SOBJ) $(OUT) $(OUT).bin $(OUT).code
