################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/var/subdir.mk
-include src/ui/subdir.mk
-include src/sys/subdir.mk
-include src/sgfx/subdir.mk
-include src/hal/subdir.mk
-include src/fmt/subdir.mk
-include src/draw/subdir.mk
-include src/calc/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libapi.a

# Tool invocations
libapi.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Archiver'
	arm-none-eabi-ar -r  libapi.a $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(ARCHIVES)$(C_DEPS)$(CPP_DEPS) libapi.a
	-@echo ' '

post-build:
	-sh /Applications/StratifyLabs-SDK/Tools/gcc/arm-none-eabi/lib/ldscripts/lib-post-process.sh /Applications/StratifyLabs-SDK/Tools/gcc arm-none-eabi Project api armv7e-m fpu/fpv5-sp-d16 sapi
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
