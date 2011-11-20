################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
ArduinoBlinkTest.lss \

FLASH_IMAGE += \
ArduinoBlinkTest.hex \

EEPROM_IMAGE += \
ArduinoBlinkTest.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: ArduinoBlinkTest.elf secondary-outputs

# Tool invocations
ArduinoBlinkTest.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-gcc --cref -s -Os -o"ArduinoBlinkTest.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lm -Wl,-Map,ArduinoBlinkTest.map,--cref -L"H:\gdp\uavcamera\ArduinoCore\Release" -mmcu=atmega328p
	@echo 'Finished building target: $@'
	@echo ' '

ArduinoBlinkTest.lss: ArduinoBlinkTest.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S ArduinoBlinkTest.elf  >"ArduinoBlinkTest.lss"
	@echo 'Finished building: $@'
	@echo ' '

ArduinoBlinkTest.hex: ArduinoBlinkTest.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex ArduinoBlinkTest.elf  "ArduinoBlinkTest.hex"
	@echo 'Finished building: $@'
	@echo ' '

ArduinoBlinkTest.eep: ArduinoBlinkTest.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex ArduinoBlinkTest.elf  "ArduinoBlinkTest.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: ArduinoBlinkTest.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p ArduinoBlinkTest.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EEPROM_IMAGE)$(ELFS)$(FLASH_IMAGE)$(LSS)$(S_DEPS)$(CXX_DEPS)$(S_UPPER_DEPS)$(C++_DEPS)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) ArduinoBlinkTest.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
