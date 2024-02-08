################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ap/ap.c 

OBJS += \
./src/ap/ap.o 

C_DEPS += \
./src/ap/ap.d 


# Each subdirectory must supply rules for building sources it contributes
src/ap/%.o src/ap/%.su src/ap/%.cyclo: ../src/ap/%.c src/ap/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G474xx -c -I"C:/CubeWorspace/stm32g474_boot/src" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/Middlewares/ST/STM32_USB_Device_Library/Class/DFU/Inc" -I"C:/CubeWorspace/stm32g474_boot/src/common/core" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/USB_Device/App" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/USB_Device/Target" -I"C:/CubeWorspace/stm32g474_boot/src/common/include" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/Drivers/CMSIS/Device/ST/STM32G4xx/Include" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/Drivers/CMSIS/Include" -I"C:/CubeWorspace/stm32g474_boot/src/lib/cube_g474/Drivers/STM32G4xx_HAL_Driver/Inc" -I"C:/CubeWorspace/stm32g474_boot/src/ap" -I"C:/CubeWorspace/stm32g474_boot/src/bsp" -I"C:/CubeWorspace/stm32g474_boot/src/common" -I"C:/CubeWorspace/stm32g474_boot/src/hw" -I"C:/CubeWorspace/stm32g474_boot/src/lib" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-ap

clean-src-2f-ap:
	-$(RM) ./src/ap/ap.cyclo ./src/ap/ap.d ./src/ap/ap.o ./src/ap/ap.su

.PHONY: clean-src-2f-ap

