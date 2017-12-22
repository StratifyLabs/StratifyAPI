################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/hal/Adc.cpp \
../src/hal/CFifo.cpp \
../src/hal/Core.cpp \
../src/hal/Dac.cpp \
../src/hal/Dev.cpp \
../src/hal/Eint.cpp \
../src/hal/Fifo.cpp \
../src/hal/I2C.cpp \
../src/hal/Led.cpp \
../src/hal/Periph.cpp \
../src/hal/Pio.cpp \
../src/hal/Pwm.cpp \
../src/hal/Spi.cpp \
../src/hal/Tmr.cpp \
../src/hal/Uart.cpp \
../src/hal/Usb.cpp 

OBJS += \
./src/hal/Adc.o \
./src/hal/CFifo.o \
./src/hal/Core.o \
./src/hal/Dac.o \
./src/hal/Dev.o \
./src/hal/Eint.o \
./src/hal/Fifo.o \
./src/hal/I2C.o \
./src/hal/Led.o \
./src/hal/Periph.o \
./src/hal/Pio.o \
./src/hal/Pwm.o \
./src/hal/Spi.o \
./src/hal/Tmr.o \
./src/hal/Uart.o \
./src/hal/Usb.o 

CPP_DEPS += \
./src/hal/Adc.d \
./src/hal/CFifo.d \
./src/hal/Core.d \
./src/hal/Dac.d \
./src/hal/Dev.d \
./src/hal/Eint.d \
./src/hal/Fifo.d \
./src/hal/I2C.d \
./src/hal/Led.d \
./src/hal/Periph.d \
./src/hal/Pio.d \
./src/hal/Pwm.d \
./src/hal/Spi.d \
./src/hal/Tmr.d \
./src/hal/Uart.d \
./src/hal/Usb.d 


# Each subdirectory must supply rules for building sources it contributes
src/hal/%.o: ../src/hal/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	clang++ -m64 -arch x86_64 -mmacosx-version-min=10.8 -D__link -D__ -I"/Users/tgil/git/StratifyAPI/include" -I"/Applications/StratifyLabs-SDK/Tools/gcc/include" -I"/Applications/StratifyLabs-SDK/Tools/gcc/include" -Os -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


