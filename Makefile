
TARG=ColorLights

CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size
OD = avr-objdump

SRCS = main.c WS2812.c
MCU=atmega8

BUILD = build
OBJS = $(SRCS:.c=.o)
TARGET_OBJS = $(addprefix $(BUILD)/, $(OBJS))

CFLAGS = -mmcu=$(MCU) -DF_CPU=8000000UL -Wall -gdwarf-2 -std=gnu99 -Os -funsigned-char \
         -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections
LDFLAGS = -mmcu=$(MCU) -Wl,-Map=build/Test.map -gc-sections
 
all: $(TARG)
 
$(TARG): $(OBJS)
	$(CC) $(LDFLAGS) -o $(BUILD)/$@.elf $(TARGET_OBJS) -lm
	$(OBJCOPY) -O binary -R .eeprom -R .nwram $(BUILD)/$@.elf $(BUILD)/$@.bin
	$(OBJCOPY) -O ihex -R .eeprom -R .nwram $(BUILD)/$@.elf $(BUILD)/$@.hex
	$(OD) -St $(BUILD)/$@.elf > $(BUILD)/$@.lst
	$(SIZE) $(BUILD)/$@.elf

%.o: %.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c -o $(BUILD)/$@ $<;

clean:
	rm -f -rf build
