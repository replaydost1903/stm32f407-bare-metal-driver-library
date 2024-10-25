################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Bare-Metal-Driver-Library/Src/stm32f407_gpio.c \
../Drivers/Bare-Metal-Driver-Library/Src/stm32f407_rcc.c 

OBJS += \
./Drivers/Bare-Metal-Driver-Library/Src/stm32f407_gpio.o \
./Drivers/Bare-Metal-Driver-Library/Src/stm32f407_rcc.o 

C_DEPS += \
./Drivers/Bare-Metal-Driver-Library/Src/stm32f407_gpio.d \
./Drivers/Bare-Metal-Driver-Library/Src/stm32f407_rcc.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Bare-Metal-Driver-Library/Src/%.o Drivers/Bare-Metal-Driver-Library/Src/%.su Drivers/Bare-Metal-Driver-Library/Src/%.cyclo: ../Drivers/Bare-Metal-Driver-Library/Src/%.c Drivers/Bare-Metal-Driver-Library/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -DSTM32 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/Users/Kubilay/Desktop/Bare-Metal-Programming-F4/Bare-Metal-Programming-F4/Drivers/Bare-Metal-Driver-Library/Inc" -I"C:/Users/Kubilay/Desktop/Bare-Metal-Programming-F4/Bare-Metal-Programming-F4/Drivers/CMSIS/Device/Inc" -I"C:/Users/Kubilay/Desktop/Bare-Metal-Programming-F4/Bare-Metal-Programming-F4/Drivers/CMSIS/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Bare-2d-Metal-2d-Driver-2d-Library-2f-Src

clean-Drivers-2f-Bare-2d-Metal-2d-Driver-2d-Library-2f-Src:
	-$(RM) ./Drivers/Bare-Metal-Driver-Library/Src/stm32f407_gpio.cyclo ./Drivers/Bare-Metal-Driver-Library/Src/stm32f407_gpio.d ./Drivers/Bare-Metal-Driver-Library/Src/stm32f407_gpio.o ./Drivers/Bare-Metal-Driver-Library/Src/stm32f407_gpio.su ./Drivers/Bare-Metal-Driver-Library/Src/stm32f407_rcc.cyclo ./Drivers/Bare-Metal-Driver-Library/Src/stm32f407_rcc.d ./Drivers/Bare-Metal-Driver-Library/Src/stm32f407_rcc.o ./Drivers/Bare-Metal-Driver-Library/Src/stm32f407_rcc.su

.PHONY: clean-Drivers-2f-Bare-2d-Metal-2d-Driver-2d-Library-2f-Src

