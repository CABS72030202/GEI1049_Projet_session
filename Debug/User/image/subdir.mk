################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/image/image.c 

OBJS += \
./User/image/image.o 

C_DEPS += \
./User/image/image.d 


# Each subdirectory must supply rules for building sources it contributes
User/image/%.o User/image/%.su User/image/%.cyclo: ../User/image/%.c User/image/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../User/Config -I../User/example -I../User/Fonts -I../User/GUI_DEV -I../User/image -I../User/LCD -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User-2f-image

clean-User-2f-image:
	-$(RM) ./User/image/image.cyclo ./User/image/image.d ./User/image/image.o ./User/image/image.su

.PHONY: clean-User-2f-image

