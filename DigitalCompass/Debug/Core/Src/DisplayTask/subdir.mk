################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DisplayTask/DisplayTask.c 

OBJS += \
./Core/Src/DisplayTask/DisplayTask.o 

C_DEPS += \
./Core/Src/DisplayTask/DisplayTask.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/DisplayTask/%.o Core/Src/DisplayTask/%.su Core/Src/DisplayTask/%.cyclo: ../Core/Src/DisplayTask/%.c Core/Src/DisplayTask/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/1782/STM32CubeIDE/workspace_1.18.1/DigitalCompass/Core/Src/I2C" -I"C:/Users/1782/STM32CubeIDE/workspace_1.18.1/DigitalCompass/Core/Src/gpio" -I"C:/Users/1782/STM32CubeIDE/workspace_1.18.1/DigitalCompass/Core/Src/AppMain" -I"C:/Users/1782/STM32CubeIDE/workspace_1.18.1/DigitalCompass/Core/Src/OS" -I"C:/Users/1782/STM32CubeIDE/workspace_1.18.1/DigitalCompass/Core/Src/ControlTask" -I"C:/Users/1782/STM32CubeIDE/workspace_1.18.1/DigitalCompass/Core/Src/DisplayTask" -I"C:/Users/1782/STM32CubeIDE/workspace_1.18.1/DigitalCompass/Core/Src/SensorTask" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-DisplayTask

clean-Core-2f-Src-2f-DisplayTask:
	-$(RM) ./Core/Src/DisplayTask/DisplayTask.cyclo ./Core/Src/DisplayTask/DisplayTask.d ./Core/Src/DisplayTask/DisplayTask.o ./Core/Src/DisplayTask/DisplayTask.su

.PHONY: clean-Core-2f-Src-2f-DisplayTask

