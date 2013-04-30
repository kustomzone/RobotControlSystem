################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../logging.cpp \
../main.cpp \
../proxy_server.cpp \
../robot.cpp \
../robot_control_server.cpp \
../robot_db.cpp \
../robot_service.cpp \
../server.cpp \
../sql.cpp \
../stl_thread.cpp \
../user_service.cpp \
../users_robots_db.cpp 

OBJS += \
./logging.o \
./main.o \
./proxy_server.o \
./robot.o \
./robot_control_server.o \
./robot_db.o \
./robot_service.o \
./server.o \
./sql.o \
./stl_thread.o \
./user_service.o \
./users_robots_db.o 

CPP_DEPS += \
./logging.d \
./main.d \
./proxy_server.d \
./robot.d \
./robot_control_server.d \
./robot_db.d \
./robot_service.d \
./server.d \
./sql.d \
./stl_thread.d \
./user_service.d \
./users_robots_db.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -I/usr/include/mysql -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -DBIG_JOINS=1  -fno-strict-aliasing -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


