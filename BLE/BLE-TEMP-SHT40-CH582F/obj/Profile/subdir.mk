################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Profile/battservice.c \
../Profile/devinfoservice.c \
../Profile/heartrateservice.c 

OBJS += \
./Profile/battservice.o \
./Profile/devinfoservice.o \
./Profile/heartrateservice.o 

C_DEPS += \
./Profile/battservice.d \
./Profile/devinfoservice.d \
./Profile/heartrateservice.d 


# Each subdirectory must supply rules for building sources it contributes
Profile/%.o: ../Profile/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\Startup" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\BLE-TEMP-SHT40-CH582F\APP\include" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\BLE-TEMP-SHT40-CH582F\Profile\include" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\StdPeriphDriver\inc" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\HAL\include" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\Ld" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\LIB" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

