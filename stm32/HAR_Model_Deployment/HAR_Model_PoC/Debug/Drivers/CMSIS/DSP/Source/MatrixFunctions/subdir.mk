################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP/Source/MatrixFunctions/arm_mat_init_f32.c 

OBJS += \
./Drivers/CMSIS/DSP/Source/MatrixFunctions/arm_mat_init_f32.o 

C_DEPS += \
./Drivers/CMSIS/DSP/Source/MatrixFunctions/arm_mat_init_f32.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/Source/MatrixFunctions/arm_mat_init_f32.o: ../Drivers/CMSIS/DSP/Source/MatrixFunctions/arm_mat_init_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DARM_MATH_CM7 '-D__FPU_PRESENT=1' -DDEBUG -c -I..//Middlewares/ST/Application/Validation/Inc -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Middlewares/ST/AI/Inc -I../Drivers/CMSIS/DSP/Source/MatrixFunctions -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/CMSIS/DSP/Source/MatrixFunctions/arm_mat_init_f32.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

