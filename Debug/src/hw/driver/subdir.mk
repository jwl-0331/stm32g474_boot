################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hw/driver/button.c \
../src/hw/driver/cdc.c \
../src/hw/driver/cli.c \
../src/hw/driver/flash.c \
../src/hw/driver/led.c \
../src/hw/driver/log.c \
../src/hw/driver/reset.c \
../src/hw/driver/rtc.c \
../src/hw/driver/swtimer.c \
../src/hw/driver/uart.c \
../src/hw/driver/usb.c 

OBJS += \
./src/hw/driver/button.o \
./src/hw/driver/cdc.o \
./src/hw/driver/cli.o \
./src/hw/driver/flash.o \
./src/hw/driver/led.o \
./src/hw/driver/log.o \
./src/hw/driver/reset.o \
./src/hw/driver/rtc.o \
./src/hw/driver/swtimer.o \
./src/hw/driver/uart.o \
./src/hw/driver/usb.o 

C_DEPS += \
./src/hw/driver/button.d \
./src/hw/driver/cdc.d \
./src/hw/driver/cli.d \
./src/hw/driver/flash.d \
./src/hw/driver/led.d \
./src/hw/driver/log.d \
./src/hw/driver/reset.d \
./src/hw/driver/rtc.d \
./src/hw/driver/swtimer.d \
./src/hw/driver/uart.d \
./src/hw/driver/usb.d 


# Each subdirectory must supply rules for building sources it contributes
src/hw/driver/%.o src/hw/driver/%.su src/hw/driver/%.cyclo: ../src/hw/driver/%.c src/hw/driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G474xx -c -I"C:/CubeWorspace/stm32g474_boot/src" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/Middlewares/ST/STM32_USB_Device_Library/Class/DFU/Inc" -I"C:/CubeWorspace/stm32g474_boot/src/common/core" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/USB_Device/App" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/USB_Device/Target" -I"C:/CubeWorspace/stm32g474_boot/src/common/include" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/Drivers/CMSIS/Device/ST/STM32G4xx/Include" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/Drivers/CMSIS/Include" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/Drivers/STM32G4xx_HAL_Driver/Inc" -I"C:/CubeWorspace/stm32g474_boot/src/ap" -I"C:/CubeWorspace/stm32g474_boot/src/bsp" -I"C:/CubeWorspace/stm32g474_boot/src/common" -I"C:/CubeWorspace/stm32g474_boot/src/hw" -I"C:/CubeWorspace/stm32g474_boot/src/lib" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-hw-2f-driver

clean-src-2f-hw-2f-driver:
	-$(RM) ./src/hw/driver/button.cyclo ./src/hw/driver/button.d ./src/hw/driver/button.o ./src/hw/driver/button.su ./src/hw/driver/cdc.cyclo ./src/hw/driver/cdc.d ./src/hw/driver/cdc.o ./src/hw/driver/cdc.su ./src/hw/driver/cli.cyclo ./src/hw/driver/cli.d ./src/hw/driver/cli.o ./src/hw/driver/cli.su ./src/hw/driver/flash.cyclo ./src/hw/driver/flash.d ./src/hw/driver/flash.o ./src/hw/driver/flash.su ./src/hw/driver/led.cyclo ./src/hw/driver/led.d ./src/hw/driver/led.o ./src/hw/driver/led.su ./src/hw/driver/log.cyclo ./src/hw/driver/log.d ./src/hw/driver/log.o ./src/hw/driver/log.su ./src/hw/driver/reset.cyclo ./src/hw/driver/reset.d ./src/hw/driver/reset.o ./src/hw/driver/reset.su ./src/hw/driver/rtc.cyclo ./src/hw/driver/rtc.d ./src/hw/driver/rtc.o ./src/hw/driver/rtc.su ./src/hw/driver/swtimer.cyclo ./src/hw/driver/swtimer.d ./src/hw/driver/swtimer.o ./src/hw/driver/swtimer.su ./src/hw/driver/uart.cyclo ./src/hw/driver/uart.d ./src/hw/driver/uart.o ./src/hw/driver/uart.su ./src/hw/driver/usb.cyclo ./src/hw/driver/usb.d ./src/hw/driver/usb.o ./src/hw/driver/usb.su

.PHONY: clean-src-2f-hw-2f-driver

