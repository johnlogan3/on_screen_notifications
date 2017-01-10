################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../brightness.c \
../mainWindow.c \
../popup.c 

OBJS += \
./brightness.o \
./mainWindow.o \
./popup.o 

C_DEPS += \
./brightness.d \
./mainWindow.d \
./popup.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include/gtk-3.0 -I/usr/include/glib-2.0 -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


