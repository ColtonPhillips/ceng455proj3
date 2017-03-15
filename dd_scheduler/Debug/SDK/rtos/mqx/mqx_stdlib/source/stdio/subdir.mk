################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/buf_prv.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/err.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/fclose.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/fflush.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/fopen.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/fp_prv.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/fread.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/fwrite.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/get.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/pos.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/print.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/print_prv.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/put.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/scan.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/scan_prv.c \
../SDK/rtos/mqx/mqx_stdlib/source/stdio/stdio.c 

OBJS += \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/buf_prv.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/err.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/fclose.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/fflush.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/fopen.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/fp_prv.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/fread.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/fwrite.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/get.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/pos.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/print.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/print_prv.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/put.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/scan.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/scan_prv.o \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/stdio.o 

C_DEPS += \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/buf_prv.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/err.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/fclose.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/fflush.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/fopen.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/fp_prv.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/fread.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/fwrite.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/get.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/pos.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/print.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/print_prv.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/put.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/scan.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/scan_prv.d \
./SDK/rtos/mqx/mqx_stdlib/source/stdio/stdio.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/rtos/mqx/mqx_stdlib/source/stdio/%.o: ../SDK/rtos/mqx/mqx_stdlib/source/stdio/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK64FN1M0VLL12" -D"FSL_RTOS_MQX" -D"PEX_MQX_KSDK" -I"C:/project3-colton/dd_scheduler/SDK/platform/hal/inc" -I"C:/project3-colton/dd_scheduler/SDK/platform/hal/src/sim/MK64F12" -I"C:/project3-colton/dd_scheduler/SDK/platform/system/src/clock/MK64F12" -I"C:/project3-colton/dd_scheduler/SDK/platform/system/inc" -I"C:/project3-colton/dd_scheduler/SDK/platform/osa/inc" -I"C:/project3-colton/dd_scheduler/SDK/platform/CMSIS/Include" -I"C:/project3-colton/dd_scheduler/SDK/platform/devices" -I"C:/project3-colton/dd_scheduler/SDK/platform/devices/MK64F12/include" -I"C:/project3-colton/dd_scheduler/SDK/platform/utilities/src" -I"C:/project3-colton/dd_scheduler/SDK/platform/utilities/inc" -I"C:/project3-colton/dd_scheduler/SDK/platform/devices/MK64F12/startup" -I"C:/project3-colton/dd_scheduler/Generated_Code/SDK/platform/devices/MK64F12/startup" -I"C:/project3-colton/dd_scheduler/Sources" -I"C:/project3-colton/dd_scheduler/Generated_Code" -I"C:/project3-colton/dd_scheduler/SDK/platform/drivers/inc" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/psp/cortex_m" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/psp/cortex_m/cpu" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/config/common" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/include" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/bsp" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/psp/cortex_m/compiler/gcc_arm" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/nio" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/nio/src" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/nio/fs" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/nio/drivers/nio_dummy" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/nio/drivers/nio_serial" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx/source/nio/drivers/nio_tty" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx_stdlib/source/include" -I"C:/project3-colton/dd_scheduler/SDK/rtos/mqx/mqx_stdlib/source/stdio" -I"C:/project3-colton/dd_scheduler/SDK/platform/hal/src/uart" -I"C:/project3-colton/dd_scheduler/SDK/platform/drivers/src/uart" -I"C:/project3-colton/dd_scheduler/Generated_Code/SDK/rtos/mqx/config/board" -I"C:/project3-colton/dd_scheduler/Generated_Code/SDK/rtos/mqx/config/mcu" -include"C:\project3-colton\dd_scheduler\SDK\rtos\mqx\mqx\source\psp\cortex_m\psptypes.h" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


