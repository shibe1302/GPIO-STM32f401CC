
TARGET = led_blink
DEBUG = 1

OPT = -Os

BUILD_DIR = build
C_SOURCES = Core/Src/main.c \
Core/Src/system_stm32f4xx.c

# ASM sources - startup file
ASM_SOURCES = startup_stm32f401xc.s

PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

C_DEFS = -DSTM32F401xC

C_INCLUDES = -I. -ICore/Inc

# compile gcc flags
ASFLAGS = $(MCU) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDSCRIPT = STM32F401XX_FLASH.ld


LIBS = -lc -lm -lnosys 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections



all: $(BUILD_DIR)/$(TARGET).hex

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))


OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	@echo "Hex file generated: $@"

$(BUILD_DIR):
	mkdir $@

c:
	-rm -fR $(BUILD_DIR)


-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean erase flash verify flash-all reset debug size help

