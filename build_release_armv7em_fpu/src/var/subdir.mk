################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/var/ChartMessage.cpp \
../src/var/Data.cpp \
../src/var/Flags.cpp \
../src/var/Item.cpp \
../src/var/JsonString.cpp \
../src/var/Message.cpp \
../src/var/Ring.cpp \
../src/var/String.cpp \
../src/var/StringUtil.cpp \
../src/var/Token.cpp 

OBJS += \
./src/var/ChartMessage.o \
./src/var/Data.o \
./src/var/Flags.o \
./src/var/Item.o \
./src/var/JsonString.o \
./src/var/Message.o \
./src/var/Ring.o \
./src/var/String.o \
./src/var/StringUtil.o \
./src/var/Token.o 

CPP_DEPS += \
./src/var/ChartMessage.d \
./src/var/Data.d \
./src/var/Flags.d \
./src/var/Item.d \
./src/var/JsonString.d \
./src/var/Message.d \
./src/var/Ring.d \
./src/var/String.d \
./src/var/StringUtil.d \
./src/var/Token.d 


# Each subdirectory must supply rules for building sources it contributes
src/var/%.o: ../src/var/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-eabi-g++ -D__ -D__StratifyOS__ -D__armv7em -I"/Users/tgil/git/StratifyAPI/include" -Os -Wall -c -mlong-calls -ffunction-sections -fomit-frame-pointer -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -U__SOFTFP__ -D__FPU_USED=1 -fno-rtti -fno-exceptions -fno-threadsafe-statics -fno-unwind-tables -fno-use-cxa-atexit -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


