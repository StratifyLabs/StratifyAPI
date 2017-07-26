################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ui/Button.cpp \
../src/ui/ButtonPin.cpp \
../src/ui/Element.cpp \
../src/ui/ElementLinked.cpp \
../src/ui/Event.cpp \
../src/ui/EventLoop.cpp \
../src/ui/List.cpp \
../src/ui/ListAttr.cpp \
../src/ui/ListItem.cpp \
../src/ui/ListItemInfo.cpp \
../src/ui/Menu.cpp \
../src/ui/MenuList.cpp \
../src/ui/Tab.cpp \
../src/ui/TabBar.cpp \
../src/ui/TabIcon.cpp \
../src/ui/TabProgress.cpp \
../src/ui/TabText.cpp \
../src/ui/TabTime.cpp 

OBJS += \
./src/ui/Button.o \
./src/ui/ButtonPin.o \
./src/ui/Element.o \
./src/ui/ElementLinked.o \
./src/ui/Event.o \
./src/ui/EventLoop.o \
./src/ui/List.o \
./src/ui/ListAttr.o \
./src/ui/ListItem.o \
./src/ui/ListItemInfo.o \
./src/ui/Menu.o \
./src/ui/MenuList.o \
./src/ui/Tab.o \
./src/ui/TabBar.o \
./src/ui/TabIcon.o \
./src/ui/TabProgress.o \
./src/ui/TabText.o \
./src/ui/TabTime.o 

CPP_DEPS += \
./src/ui/Button.d \
./src/ui/ButtonPin.d \
./src/ui/Element.d \
./src/ui/ElementLinked.d \
./src/ui/Event.d \
./src/ui/EventLoop.d \
./src/ui/List.d \
./src/ui/ListAttr.d \
./src/ui/ListItem.d \
./src/ui/ListItemInfo.d \
./src/ui/Menu.d \
./src/ui/MenuList.d \
./src/ui/Tab.d \
./src/ui/TabBar.d \
./src/ui/TabIcon.d \
./src/ui/TabProgress.d \
./src/ui/TabText.d \
./src/ui/TabTime.d 


# Each subdirectory must supply rules for building sources it contributes
src/ui/%.o: ../src/ui/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-eabi-g++ -D___debug -D__StratifyOS__ -D__armv7em -I"/Users/tgil/git/StratifyAPI/include" -Os -Wall -c -mlong-calls -ffunction-sections -fomit-frame-pointer -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -U__SOFTFP__ -D__FPU_USED=1 -fno-rtti -fno-exceptions -fno-threadsafe-statics -fno-unwind-tables -fno-use-cxa-atexit -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


