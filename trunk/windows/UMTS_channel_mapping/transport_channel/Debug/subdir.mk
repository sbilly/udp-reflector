################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AbstractTransportChannel.cpp \
../TransportChannelXYZ.cpp \
../main.cpp 

OBJS += \
./AbstractTransportChannel.o \
./TransportChannelXYZ.o \
./main.o 

CPP_DEPS += \
./AbstractTransportChannel.d \
./TransportChannelXYZ.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I"/home/harlan/UMTS_channel_mapping/transport_channel/common" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


