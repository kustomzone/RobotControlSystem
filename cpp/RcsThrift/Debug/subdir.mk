################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../RobotControlService.cpp \
../RobotService.cpp \
../UserService.cpp \
../robot_control_service_constants.cpp \
../robot_control_service_types.cpp \
../robot_service_constants.cpp \
../robot_service_types.cpp \
../user_service_constants.cpp \
../user_service_types.cpp 

OBJS += \
./RobotControlService.o \
./RobotService.o \
./UserService.o \
./robot_control_service_constants.o \
./robot_control_service_types.o \
./robot_service_constants.o \
./robot_service_types.o \
./user_service_constants.o \
./user_service_types.o 

CPP_DEPS += \
./RobotControlService.d \
./RobotService.d \
./UserService.d \
./robot_control_service_constants.d \
./robot_control_service_types.d \
./robot_service_constants.d \
./robot_service_types.d \
./user_service_constants.d \
./user_service_types.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


