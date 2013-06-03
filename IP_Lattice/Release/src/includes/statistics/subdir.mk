################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/includes/statistics/Data.cpp 

OBJS += \
./src/includes/statistics/Data.o 

CPP_DEPS += \
./src/includes/statistics/Data.d 


# Each subdirectory must supply rules for building sources it contributes
src/includes/statistics/%.o: ../src/includes/statistics/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	nvcc -O3  -odir "src/includes/statistics" -M -o "$(@:%.o=%.d)" "$<"
	nvcc -O3 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


