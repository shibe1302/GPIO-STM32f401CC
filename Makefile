# Project name
TARGET = led_blink

# Build configuration
BUILD_DIR = build
C_SOURCES = Core/Src/main.c \
            Core/Src/system_stm32f4xx.c
ASM_SOURCES = startup_stm32f401xc.s

# Toolchain configuration
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
ELF = $(CP) -O elf32-littlearm

# MCU configuration
CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# Compiler flags
C_DEFS = -DSTM32F401xC
C_INCLUDES = -I. -ICore/Inc
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) -Wall -Os -fdata-sections -ffunction-sections
ASFLAGS = $(CFLAGS)
LDSCRIPT = STM32F401XX_FLASH.ld
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) -lc -lm -lnosys -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# Object files
OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(ASM_SOURCES:.s=.o)))

# Build rules
all: clean $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).hex flash

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: Core/Src/%.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(HEX) $< $@
	@echo "Hex file generated: $@"

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(CP) -O binary $< $@
	@echo "Binary file generated: $@"

clean:
	rm -rf $(BUILD_DIR)

flash: $(BUILD_DIR)/$(TARGET).hex
	JLink.exe -CommanderScript flash.jlink

.PHONY: all clean flash

