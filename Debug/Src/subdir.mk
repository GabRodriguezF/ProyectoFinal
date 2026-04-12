################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ILI9341_driver.c \
../Src/gpio_driver.c \
../Src/gpio_if.c \
../Src/main.c \
../Src/mmcr.c \
../Src/pwm_driver.c \
../Src/pwm_if.c \
../Src/quad_enc.c \
../Src/quad_enc_if.c \
../Src/spi_driver.c \
../Src/spi_if.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_clock_driver.c \
../Src/system_clock_if.c \
../Src/timer_driver.c \
../Src/timer_if.c \
../Src/uart_driver.c \
../Src/uart_if.c 

OBJS += \
./Src/ILI9341_driver.o \
./Src/gpio_driver.o \
./Src/gpio_if.o \
./Src/main.o \
./Src/mmcr.o \
./Src/pwm_driver.o \
./Src/pwm_if.o \
./Src/quad_enc.o \
./Src/quad_enc_if.o \
./Src/spi_driver.o \
./Src/spi_if.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_clock_driver.o \
./Src/system_clock_if.o \
./Src/timer_driver.o \
./Src/timer_if.o \
./Src/uart_driver.o \
./Src/uart_if.o 

C_DEPS += \
./Src/ILI9341_driver.d \
./Src/gpio_driver.d \
./Src/gpio_if.d \
./Src/main.d \
./Src/mmcr.d \
./Src/pwm_driver.d \
./Src/pwm_if.d \
./Src/quad_enc.d \
./Src/quad_enc_if.d \
./Src/spi_driver.d \
./Src/spi_if.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_clock_driver.d \
./Src/system_clock_if.d \
./Src/timer_driver.d \
./Src/timer_if.d \
./Src/uart_driver.d \
./Src/uart_if.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F405RGTx -c -I../Inc -I"D:/!Escuela/WIKI/CLASE/Material_Especialidad_2026/Clase_02_Chip_headers/Drivers/CMSIS/Include" -I"D:/!Escuela/WIKI/CLASE/Material_Especialidad_2026/Clase_02_Chip_headers/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/ILI9341_driver.cyclo ./Src/ILI9341_driver.d ./Src/ILI9341_driver.o ./Src/ILI9341_driver.su ./Src/gpio_driver.cyclo ./Src/gpio_driver.d ./Src/gpio_driver.o ./Src/gpio_driver.su ./Src/gpio_if.cyclo ./Src/gpio_if.d ./Src/gpio_if.o ./Src/gpio_if.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/mmcr.cyclo ./Src/mmcr.d ./Src/mmcr.o ./Src/mmcr.su ./Src/pwm_driver.cyclo ./Src/pwm_driver.d ./Src/pwm_driver.o ./Src/pwm_driver.su ./Src/pwm_if.cyclo ./Src/pwm_if.d ./Src/pwm_if.o ./Src/pwm_if.su ./Src/quad_enc.cyclo ./Src/quad_enc.d ./Src/quad_enc.o ./Src/quad_enc.su ./Src/quad_enc_if.cyclo ./Src/quad_enc_if.d ./Src/quad_enc_if.o ./Src/quad_enc_if.su ./Src/spi_driver.cyclo ./Src/spi_driver.d ./Src/spi_driver.o ./Src/spi_driver.su ./Src/spi_if.cyclo ./Src/spi_if.d ./Src/spi_if.o ./Src/spi_if.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_clock_driver.cyclo ./Src/system_clock_driver.d ./Src/system_clock_driver.o ./Src/system_clock_driver.su ./Src/system_clock_if.cyclo ./Src/system_clock_if.d ./Src/system_clock_if.o ./Src/system_clock_if.su ./Src/timer_driver.cyclo ./Src/timer_driver.d ./Src/timer_driver.o ./Src/timer_driver.su ./Src/timer_if.cyclo ./Src/timer_if.d ./Src/timer_if.o ./Src/timer_if.su ./Src/uart_driver.cyclo ./Src/uart_driver.d ./Src/uart_driver.o ./Src/uart_driver.su ./Src/uart_if.cyclo ./Src/uart_if.d ./Src/uart_if.o ./Src/uart_if.su

.PHONY: clean-Src

