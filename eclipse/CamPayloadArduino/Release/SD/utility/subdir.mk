################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SD/utility/Sd2Card.cpp \
../SD/utility/SdFile.cpp \
../SD/utility/SdVolume.cpp 

OBJS += \
./SD/utility/Sd2Card.o \
./SD/utility/SdFile.o \
./SD/utility/SdVolume.o 

CPP_DEPS += \
./SD/utility/Sd2Card.d \
./SD/utility/SdFile.d \
./SD/utility/SdVolume.d 


# Each subdirectory must supply rules for building sources it contributes
SD/utility/%.o: ../SD/utility/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"H:\arduino-0022\hardware\arduino\cores\arduino" -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


