################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32746G-Discovery/stm32746g_discovery.c \
../Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_audio.c \
../Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.c \
../Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sdram.c \
../Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_ts.c 

OBJS += \
./Drivers/BSP/STM32746G-Discovery/stm32746g_discovery.o \
./Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_audio.o \
./Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.o \
./Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sdram.o \
./Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_ts.o 

C_DEPS += \
./Drivers/BSP/STM32746G-Discovery/stm32746g_discovery.d \
./Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_audio.d \
./Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.d \
./Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_sdram.d \
./Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_ts.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32746G-Discovery/%.o: ../Drivers/BSP/STM32746G-Discovery/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -DSTM32F746G_DISCO -DUSE_STM32756G_DISCOVERY -DUSE_USB_FS -DUSE_IOEXPANDER -DSTM32F746NGHx -DSTM32F7 -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -DOS_SUPPORT -DUSB_AUDIO -DONBOARD_DAC -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Drivers/BSP/Components/wm8994 -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/inc -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Drivers/BSP/STM32746G-Discovery -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Drivers/STM32F7xx_HAL_Driver/Inc -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/CMSIS/Include -I"C:/Users/Torsten/Documents/Eclipse_WS/STM32F7_USBAudio/Middlewares/ST/STemWin/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

