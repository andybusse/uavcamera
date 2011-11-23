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
ArduinoBlinkTest_Template.lss \

FLASH_IMAGE += \
ArduinoBlinkTest_Template.hex \

EEPROM_IMAGE += \
ArduinoBlinkTest_Template.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: ArduinoBlinkTest_Template.elf secondary-outputs

# Tool invocations
ArduinoBlinkTest_Template.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-gcc --cref -s -Os -o"ArduinoBlinkTest_Template.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lm -Wl,-Map,ArduinoBlinkTest_Template.map,--cref -L"H:\gdp\uavcamera\eclipse\ArduinoCore\Release" -mmcu=atmega328p
	@echo 'Finished building target: $@'
	@echo ' '

ArduinoBlinkTest_Template.lss: ArduinoBlinkTest_Template.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S ArduinoBlinkTest_Template.elf  >"ArduinoBlinkTest_Template.lss"
	@echo 'Finished building: $@'
	@echo ' '

ArduinoBlinkTest_Template.hex: ArduinoBlinkTest_Template.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex ArduinoBlinkTest_Template.elf  "ArduinoBlinkTest_Template.hex"
	@echo 'Finished building: $@'
	@echo ' '

ArduinoBlinkTest_Template.eep: ArduinoBlinkTest_Template.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex ArduinoBlinkTest_Template.elf  "ArduinoBlinkTest_Template.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: ArduinoBlinkTest_Template.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p ArduinoBlinkTest_Template.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EEPROM_IMAGE)$(ELFS)$(FLASH_IMAGE)$(LSS)$(S_DEPS)$(CXX_DEPS)$(S_UPPER_DEPS)$(C++_DEPS)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) ArduinoBlinkTest_Template.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
