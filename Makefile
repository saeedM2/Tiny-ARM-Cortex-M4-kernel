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
SOURCES += uart/src/uart_init.c
SOURCES += system/src/system_init.c
SOURCES += cli/src/cli_communitcate.c
SOURCES += multi_task/src/context_switch.s
SOURCES += multi_task/src/task.c
SOURCES += common/src/lsprintf.c
SOURCES += common/src/lmemcpy.c
SOURCES += sys_call/src/syscall.s


## Include C headers
INCLUDES += -Icommon/include \
						-Igpio/include \
						-Iuart/include \
						-Ircc/include \
						-Iinterrupt/include \
						-Isystem/include \
						-Icli/include \
						-Imulti_task/include \

## Create object files (.o)
OBJS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

## Platform and optimization options
CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F411VETx -DSTM32F411E_DISCO
CFLAGS += -DDEBUG -DSTM32F411xE -DUSE_STDPERIPH_DRIVER -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -I. $(INCLUDES) -Werror
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
