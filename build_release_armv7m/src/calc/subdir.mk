################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/calc/Base64.cpp \
../src/calc/Pid.cpp \
../src/calc/Rle.cpp 

OBJS += \
./src/calc/Base64.o \
./src/calc/Pid.o \
./src/calc/Rle.o 

CPP_DEPS += \
./src/calc/Base64.d \
./src/calc/Pid.d \
./src/calc/Rle.d 


# Each subdirectory must supply rules for building sources it contributes
src/calc/%.o: ../src/calc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-eabi-g++ -D__ -D__StratifyOS__ -D__armv7m -I"/Users/tgil/git/StratifyAPI/include" -Os -Wall -c -g -mlong-calls -ffunction-sections -fomit-frame-pointer -march=armv7-m -mthumb -fno-rtti -fno-exceptions -fno-threadsafe-statics -fno-unwind-tables -fno-use-cxa-atexit -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


