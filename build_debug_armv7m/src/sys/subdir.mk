################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sys/Appfs.cpp \
../src/sys/Assets.cpp \
../src/sys/Cli.cpp \
../src/sys/Dir.cpp \
../src/sys/File.cpp \
../src/sys/FileInfo.cpp \
../src/sys/Mq.cpp \
../src/sys/Mutex.cpp \
../src/sys/Sched.cpp \
../src/sys/Sem.cpp \
../src/sys/Signal.cpp \
../src/sys/Sys.cpp \
../src/sys/Thread.cpp \
../src/sys/Time.cpp \
../src/sys/Timer.cpp 

OBJS += \
./src/sys/Appfs.o \
./src/sys/Assets.o \
./src/sys/Cli.o \
./src/sys/Dir.o \
./src/sys/File.o \
./src/sys/FileInfo.o \
./src/sys/Mq.o \
./src/sys/Mutex.o \
./src/sys/Sched.o \
./src/sys/Sem.o \
./src/sys/Signal.o \
./src/sys/Sys.o \
./src/sys/Thread.o \
./src/sys/Time.o \
./src/sys/Timer.o 

CPP_DEPS += \
./src/sys/Appfs.d \
./src/sys/Assets.d \
./src/sys/Cli.d \
./src/sys/Dir.d \
./src/sys/File.d \
./src/sys/FileInfo.d \
./src/sys/Mq.d \
./src/sys/Mutex.d \
./src/sys/Sched.d \
./src/sys/Sem.d \
./src/sys/Signal.d \
./src/sys/Sys.d \
./src/sys/Thread.d \
./src/sys/Time.d \
./src/sys/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/sys/%.o: ../src/sys/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-eabi-g++ -D___debug -D__StratifyOS__ -D__armv7m -I"/Users/tgil/git/StratifyAPI/include" -Os -Wall -c -g -mlong-calls -ffunction-sections -fomit-frame-pointer -march=armv7-m -mthumb -fno-rtti -fno-exceptions -fno-threadsafe-statics -fno-unwind-tables -fno-use-cxa-atexit -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


