################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/draw/Animation.cpp \
../src/draw/Drawing.cpp \
../src/draw/Icon.cpp \
../src/draw/Image.cpp \
../src/draw/Panel.cpp \
../src/draw/ProgressArc.cpp \
../src/draw/ProgressBar.cpp \
../src/draw/ProgressCircle.cpp \
../src/draw/Rect.cpp \
../src/draw/Text.cpp \
../src/draw/TextAttr.cpp \
../src/draw/TextBox.cpp 

OBJS += \
./src/draw/Animation.o \
./src/draw/Drawing.o \
./src/draw/Icon.o \
./src/draw/Image.o \
./src/draw/Panel.o \
./src/draw/ProgressArc.o \
./src/draw/ProgressBar.o \
./src/draw/ProgressCircle.o \
./src/draw/Rect.o \
./src/draw/Text.o \
./src/draw/TextAttr.o \
./src/draw/TextBox.o 

CPP_DEPS += \
./src/draw/Animation.d \
./src/draw/Drawing.d \
./src/draw/Icon.d \
./src/draw/Image.d \
./src/draw/Panel.d \
./src/draw/ProgressArc.d \
./src/draw/ProgressBar.d \
./src/draw/ProgressCircle.d \
./src/draw/Rect.d \
./src/draw/Text.d \
./src/draw/TextAttr.d \
./src/draw/TextBox.d 


# Each subdirectory must supply rules for building sources it contributes
src/draw/%.o: ../src/draw/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-eabi-g++ -D__ -D__StratifyOS__ -D__armv7em -I"/Users/tgil/git/StratifyAPI/include" -Os -Wall -c -mlong-calls -ffunction-sections -fomit-frame-pointer -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -U__SOFTFP__ -D__FPU_USED=1 -fno-rtti -fno-exceptions -fno-threadsafe-statics -fno-unwind-tables -fno-use-cxa-atexit -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


