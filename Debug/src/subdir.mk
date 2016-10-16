################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/mdlist_pqueue.c \
../src/mdlist_pqueue_test.c 

OBJS += \
./src/mdlist_pqueue.o \
./src/mdlist_pqueue_test.o 

C_DEPS += \
./src/mdlist_pqueue.d \
./src/mdlist_pqueue_test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


