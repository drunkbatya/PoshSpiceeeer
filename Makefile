TARGET = NastyaBirthdayDevBoard
DEBUG = 1
OPT = -Og
BUILD_DIR = build
CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
AS_DEFS =
AS_INCLUDES =

C_DEFS =  \
-DSTM32F401xE \
-DUSE_FULL_LL_DRIVER \
-DHSE_VALUE=25000000 \
-DHSE_STARTUP_TIMEOUT=100 \
-DLSE_STARTUP_TIMEOUT=5000 \
-DLSE_VALUE=32768 \
-DEXTERNAL_CLOCK_VALUE=12288000 \
-DHSI_VALUE=16000000 \
-DLSI_VALUE=32000 \
-DVDD_VALUE=3300 \
-DPREFETCH_ENABLE=1 \
-DINSTRUCTION_CACHE_ENABLE=1 \
-DDATA_CACHE_ENABLE=1

C_SOURCES =  \
src/my_main.c \
stm/Core/Src/main.c \
stm/Core/Src/stm32f4xx_it.c \
lib/lcd-st7920/st7920.c \
stm/Core/Src/system_stm32f4xx.c \
stm/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c \
stm/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.c \
stm/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.c \
stm/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.c

C_INCLUDES =  \
-Isrc \
-Istm/Core/Inc \
-Istm/Drivers/STM32F4xx_HAL_Driver/Inc \
-Istm/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
-Ilib/lcd-st7920 \
-Istm/Drivers/CMSIS/Include

ASM_SOURCES =  \
stm/startup_stm32f401xe.s

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
BIN = $(CP) -O binary -S

ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
LDSCRIPT = stm/STM32F401CDUx_FLASH.ld

LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

.PHONY: flash
flash: all
	st-flash write build/NastyaBirthdayDevBoard.bin 0x8000000

.PHONY: flash-dfu
flash-dfu: all
	dfu-util -a 0 -D build/NastyaBirthdayDevBoard.bin -s 0x08000000

.PHONY: lint
lint:
	find . -type f \( -name "*.c" -o -name "*.h" \) \
		\( -path "./lib/*" -o -path "./src/*" \) \
		| xargs clang-format --Werror --style=file -i --dry-run

.PHONY: format
format:
	find . -type f \( -name "*.c" -o -name "*.h" \) \
		\( -path "./lib/*" -o -path "./src/*" \) \
		| xargs clang-format --Werror --style=file -i

clean:
	-rm -fR $(BUILD_DIR)
-include $(wildcard $(BUILD_DIR)/*.d)
