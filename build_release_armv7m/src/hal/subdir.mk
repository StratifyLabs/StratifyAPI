################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hal/util_c.c 

CPP_SRCS += \
../src/hal/Adc.cpp \
../src/hal/Core.cpp \
../src/hal/Dac.cpp \
../src/hal/Dev.cpp \
../src/hal/Display.cpp \
../src/hal/DisplayDev.cpp \
../src/hal/Eint.cpp \
../src/hal/Fifo.cpp \
../src/hal/I2C.cpp \
../src/hal/Led.cpp \
../src/hal/Mcfifo.cpp \
../src/hal/Periph.cpp \
../src/hal/Pio.cpp \
../src/hal/Pwm.cpp \
../src/hal/Spi.cpp \
../src/hal/Tmr.cpp \
../src/hal/Uart.cpp \
../src/hal/Usb.cpp 

OBJS += \
./src/hal/Adc.o \
./src/hal/Core.o \
./src/hal/Dac.o \
./src/hal/Dev.o \
./src/hal/Display.o \
./src/hal/DisplayDev.o \
./src/hal/Eint.o \
./src/hal/Fifo.o \
./src/hal/I2C.o \
./src/hal/Led.o \
./src/hal/Mcfifo.o \
./src/hal/Periph.o \
./src/hal/Pio.o \
./src/hal/Pwm.o \
./src/hal/Spi.o \
./src/hal/Tmr.o \
./src/hal/Uart.o \
./src/hal/Usb.o \
./src/hal/util_c.o 

C_DEPS += \
./src/hal/util_c.d 

CPP_DEPS += \
./src/hal/Adc.d \
./src/hal/Core.d \
./src/hal/Dac.d \
./src/hal/Dev.d \
./src/hal/Display.d \
./src/hal/DisplayDev.d \
./src/hal/Eint.d \
./src/hal/Fifo.d \
./src/hal/I2C.d \
./src/hal/Led.d \
./src/hal/Mcfifo.d \
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
	arm-none-eabi-g++ -D__ -D__StratifyOS__ -D__armv7m -I"/Users/tgil/git/StratifyAPI/include" -Os -Wall -c -g -mlong-calls -ffunction-sections -fomit-frame-pointer -march=armv7-m -mthumb -fno-rtti -fno-exceptions -fno-threadsafe-statics -fno-unwind-tables -fno-use-cxa-atexit -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/hal/%.o: ../src/hal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -D__ -D__StratifyOS__ -D__armv7m -I"/Users/tgil/git/StratifyAPI/include" -Os -Wall -c -g -mlong-calls -ffunction-sections -fomit-frame-pointer -march=armv7-m -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


