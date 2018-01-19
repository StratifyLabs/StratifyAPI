################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/fmt/Bmp.cpp \
../src/fmt/Son.cpp \
../src/fmt/Wav.cpp \
../src/fmt/Xml.cpp 

OBJS += \
./src/fmt/Bmp.o \
./src/fmt/Son.o \
./src/fmt/Wav.o \
./src/fmt/Xml.o 

CPP_DEPS += \
./src/fmt/Bmp.d \
./src/fmt/Son.d \
./src/fmt/Wav.d \
./src/fmt/Xml.d 


# Each subdirectory must supply rules for building sources it contributes
src/fmt/%.o: ../src/fmt/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	clang++ -m64 -arch x86_64 -mmacosx-version-min=10.8 -D__link -D__ -I"/Users/tgil/git/StratifyAPI/include" -I"/Applications/StratifyLabs-SDK/Tools/gcc/include" -I"/Applications/StratifyLabs-SDK/Tools/gcc/include" -Os -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


