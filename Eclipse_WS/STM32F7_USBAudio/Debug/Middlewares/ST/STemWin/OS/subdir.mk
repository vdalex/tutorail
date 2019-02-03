################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STemWin/OS/GUI_X.c 

OBJS += \
./Middlewares/ST/STemWin/OS/GUI_X.o 

C_DEPS += \
./Middlewares/ST/STemWin/OS/GUI_X.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STemWin/OS/%.o: ../Middlewares/ST/STemWin/OS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -DSTM32F746G_DISCO -DUSE_STM32756G_DISCOVERY -DUSE_USB_FS -DUSE_IOEXPANDER -DSTM32F746NGHx -DSTM32F7 -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -DOS_SUPPORT -DUSB_AUDIO -DONBOARD_DAC -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Drivers/BSP/Components/wm8994 -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/inc -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Drivers/BSP/STM32746G-Discovery -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Drivers/STM32F7xx_HAL_Driver/Inc -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/CMSIS/Include -I"C:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Middlewares/ST/STemWin/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


