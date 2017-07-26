################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sys/Appfs.cpp \
../src/sys/Cli.cpp \
../src/sys/Dir.cpp \
../src/sys/File.cpp \
../src/sys/Kernel.cpp \
../src/sys/Mq.cpp \
../src/sys/Mutex.cpp \
../src/sys/Sem.cpp \
../src/sys/Signal.cpp \
../src/sys/Thread.cpp \
../src/sys/Time.cpp \
../src/sys/Timer.cpp 

OBJS += \
./src/sys/Appfs.o \
./src/sys/Cli.o \
./src/sys/Dir.o \
./src/sys/File.o \
./src/sys/Kernel.o \
./src/sys/Mq.o \
./src/sys/Mutex.o \
./src/sys/Sem.o \
./src/sys/Signal.o \
./src/sys/Thread.o \
./src/sys/Time.o \
./src/sys/Timer.o 

CPP_DEPS += \
./src/sys/Appfs.d \
./src/sys/Cli.d \
./src/sys/Dir.d \
./src/sys/File.d \
./src/sys/Kernel.d \
./src/sys/Mq.d \
./src/sys/Mutex.d \
./src/sys/Sem.d \
./src/sys/Signal.d \
./src/sys/Thread.d \
./src/sys/Time.d \
./src/sys/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/sys/%.o: ../src/sys/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	clang++ -m64 -arch x86_64 -mmacosx-version-min=10.8 -D__StratifyOS__ -D__link -D__ -I"/Users/tgil/git/StratifyAPI/include" -I"/Applications/StratifyLabs-SDK/Tools/gcc/include" -I"/Applications/StratifyLabs-SDK/Tools/gcc/include" -Os -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


