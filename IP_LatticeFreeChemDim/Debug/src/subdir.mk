################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/IP_LatticeFreeDim.cpp \
../src/simulation.cpp 

OBJS += \
./src/IP_LatticeFreeDim.o \
./src/simulation.o 

CPP_DEPS += \
./src/IP_LatticeFreeDim.d \
./src/simulation.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	nvcc -G -g -O0  -odir "src" -M -o "$(@:%.o=%.d)" "$<"
	nvcc -G -g -O0 --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


