################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/rtos/mqx/mqx/source/nio/src/errno.c \
../SDK/rtos/mqx/mqx/source/nio/src/nio.c \
../SDK/rtos/mqx/mqx/source/nio/src/nio_posix.c \
../SDK/rtos/mqx/mqx/source/nio/src/nio_std_adpt.c 

OBJS += \
./SDK/rtos/mqx/mqx/source/nio/src/errno.o \
./SDK/rtos/mqx/mqx/source/nio/src/nio.o \
./SDK/rtos/mqx/mqx/source/nio/src/nio_posix.o \
./SDK/rtos/mqx/mqx/source/nio/src/nio_std_adpt.o 

C_DEPS += \
./SDK/rtos/mqx/mqx/source/nio/src/errno.d \
./SDK/rtos/mqx/mqx/source/nio/src/nio.d \
./SDK/rtos/mqx/mqx/source/nio/src/nio_posix.d \
./SDK/rtos/mqx/mqx/source/nio/src/nio_std_adpt.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/rtos/mqx/mqx/source/nio/src/%.o: ../SDK/rtos/mqx/mqx/source/nio/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK64FN1M0VLL12" -D"FSL_RTOS_MQX" -D"PEX_MQX_KSDK" -I"C:/project3-colton/dd_scheduler/SDK/platform/hal/inc" -I"C:/project3-colton/dd_scheduler/SDK/platform/hal/src/sim/MK64F12" -I"C:/project3-colton/dd_scheduler/SDK/platform/system/src/clock/MK64F12" -I"C:/project3-colton/dd_scheduler/SDK/platform/system/inc" -I"C:/project3-colton/dd_scheduler/SDK/platform/osa/inc" -I"C:/project3-colton/dd_scheduler/SDK/platform/CMSIS/Include" -I"C:/project3-colton/dd_scheduler/SDK/platform/devices" -I"C:/project3-colton/dd_scheduler/SDK/platform/devices/MK64F12/include" -I"C:/project3-colton/dd_scheduler/SDK/platform/utilities/src" -I"C:/project3-colton/dd_scheduler/SDK/platform/utilities/inc" -I"C:/project3-colton/dd_scheduler/SDK/platform/devices/MK64F12/startup" -I"C:/project3-colton/dd_scheduler/Generated_Code/SDK/platform/devices/MK64F12/startup" -I"C:/project3-colton/dd_scheduler/Sources" -I"C:/project3-colton/dd_scheduler/Generated_Code" -I"C:/project3-colton/dd_scheduler/SDK/platform/drivers/inc" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/psp/cortex_m" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/psp/cortex_m/cpu" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/config/common" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/include" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/bsp" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/psp/cortex_m/compiler/gcc_arm" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/nio" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/nio/src" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/nio/fs" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/nio/drivers/nio_dummy" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/nio/drivers/nio_serial" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/nio/drivers/nio_tty" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx_stdlib/source/include" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx_stdlib/source/stdio" -I"C:/project3-colton/dd_scheduler/SDK/platform/hal/src/uart" -I"C:/project3-colton/dd_scheduler/SDK/platform/drivers/src/uart" -I"C:/project3-colton/dd_scheduler/Generated_Code/SDK/rtos/mqx/config/board" -I"C:/project3-colton/dd_scheduler/Generated_Code/SDK/rtos/mqx/config/mcu" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


