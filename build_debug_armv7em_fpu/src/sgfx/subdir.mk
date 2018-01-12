################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sgfx/Bitmap.cpp \
../src/sgfx/Cursor.cpp \
../src/sgfx/Font.cpp \
../src/sgfx/FontFile.cpp \
../src/sgfx/FontMemory.cpp \
../src/sgfx/FontSvg.cpp \
../src/sgfx/FontSvgMemory.cpp \
../src/sgfx/Pen.cpp \
../src/sgfx/Point.cpp \
../src/sgfx/Vector.cpp 

OBJS += \
./src/sgfx/Bitmap.o \
./src/sgfx/Cursor.o \
./src/sgfx/Font.o \
./src/sgfx/FontFile.o \
./src/sgfx/FontMemory.o \
./src/sgfx/FontSvg.o \
./src/sgfx/FontSvgMemory.o \
./src/sgfx/Pen.o \
./src/sgfx/Point.o \
./src/sgfx/Vector.o 

CPP_DEPS += \
./src/sgfx/Bitmap.d \
./src/sgfx/Cursor.d \
./src/sgfx/Font.d \
./src/sgfx/FontFile.d \
./src/sgfx/FontMemory.d \
./src/sgfx/FontSvg.d \
./src/sgfx/FontSvgMemory.d \
./src/sgfx/Pen.d \
./src/sgfx/Point.d \
./src/sgfx/Vector.d 


# Each subdirectory must supply rules for building sources it contributes
src/sgfx/%.o: ../src/sgfx/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-eabi-g++ -D___debug -D__StratifyOS__ -D__armv7em -I"/Users/tgil/git/StratifyAPI/include" -Os -Wall -c -mlong-calls -ffunction-sections -fomit-frame-pointer -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -U__SOFTFP__ -D__FPU_USED=1 -fno-rtti -fno-exceptions -fno-threadsafe-statics -fno-unwind-tables -fno-use-cxa-atexit -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


