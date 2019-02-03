################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/GUIConf.c \
../src/LCDConf.c \
../src/main.c \
../src/spdifrx.c \
../src/stm32f7xx_it.c \
../src/syscalls.c \
../src/system_stm32f7xx.c \
../src/touchscreen.c \
../src/usbd_audio_if.c \
../src/usbd_conf.c \
../src/usbd_desc.c 

OBJS += \
./src/GUIConf.o \
./src/LCDConf.o \
./src/main.o \
./src/spdifrx.o \
./src/stm32f7xx_it.o \
./src/syscalls.o \
./src/system_stm32f7xx.o \
./src/touchscreen.o \
./src/usbd_audio_if.o \
./src/usbd_conf.o \
./src/usbd_desc.o 

C_DEPS += \
./src/GUIConf.d \
./src/LCDConf.d \
./src/main.d \
./src/spdifrx.d \
./src/stm32f7xx_it.d \
./src/syscalls.d \
./src/system_stm32f7xx.d \
./src/touchscreen.d \
./src/usbd_audio_if.d \
./src/usbd_conf.d \
./src/usbd_desc.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -DSTM32F746G_DISCO -DUSE_STM32756G_DISCOVERY -DUSE_USB_FS -DUSE_IOEXPANDER -DSTM32F746NGHx -DSTM32F7 -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -DOS_SUPPORT -DUSB_AUDIO -DONBOARD_DAC -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Drivers/BSP/Components/wm8994 -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/inc -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Drivers/BSP/STM32746G-Discovery -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Drivers/STM32F7xx_HAL_Driver/Inc -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/CMSIS/Include -I"C:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Middlewares/ST/STemWin/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


