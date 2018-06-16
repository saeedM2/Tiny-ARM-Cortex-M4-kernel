NAME = kernel
BUILDDIR = build

## Cross-compilation commands
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
AS      = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

SOURCES += src/main.c
SOURCES += src/startup.s

## Include C headers
#INCLUDES =

## Create object files (.o)
OBJS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

## Platform and optimization options
CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F411VETx -DSTM32F411E_DISCO
CFLAGS += -DDEBUG -DSTM32F411xE -DUSE_STDPERIPH_DRIVER -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP
LFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -mfpu=fpv4-sp-d16 -T"LinkerScript.ld" -Wl,-Map=build/out.map -Wl,--gc-sections

ELF = $(BUILDDIR)/$(NAME).elf
HEX = $(BUILDDIR)/$(NAME).hex
BIN = $(BUILDDIR)/$(NAME).bin

all: build/kernel.elf build/kernel.bin size
	 
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(ELF): $(OBJS) LinkerScript.ld
	$(LD) $(LFLAGS) -o $@ $(OBJS)

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

size: build/kernel.elf
	$(SIZE) $<

clean:
	rm -rf build
