################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Device/Src/system_stm32f4xx.c 

OBJS += \
./Drivers/CMSIS/Device/Src/system_stm32f4xx.o 

C_DEPS += \
./Drivers/CMSIS/Device/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Device/Src/%.o Drivers/CMSIS/Device/Src/%.su Drivers/CMSIS/Device/Src/%.cyclo: ../Drivers/CMSIS/Device/Src/%.c Drivers/CMSIS/Device/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F407xx -DSTM32 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/Users/Kubilay/Desktop/Bare-Metal-Programming-F4/Bare-Metal-Programming-F4/Drivers/Bare-Metal-Driver-Library/Inc" -I"C:/Users/Kubilay/Desktop/Bare-Metal-Programming-F4/Bare-Metal-Programming-F4/Drivers/CMSIS/Device/Inc" -I"C:/Users/Kubilay/Desktop/Bare-Metal-Programming-F4/Bare-Metal-Programming-F4/Drivers/CMSIS/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Device-2f-Src

clean-Drivers-2f-CMSIS-2f-Device-2f-Src:
	-$(RM) ./Drivers/CMSIS/Device/Src/system_stm32f4xx.cyclo ./Drivers/CMSIS/Device/Src/system_stm32f4xx.d ./Drivers/CMSIS/Device/Src/system_stm32f4xx.o ./Drivers/CMSIS/Device/Src/system_stm32f4xx.su

.PHONY: clean-Drivers-2f-CMSIS-2f-Device-2f-Src

