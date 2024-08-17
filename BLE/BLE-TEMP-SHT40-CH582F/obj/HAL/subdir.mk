################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/WCH_Files/BLE-TEMP-SHT40-CH582F/BLE/HAL/MCU.c \
E:/WCH_Files/BLE-TEMP-SHT40-CH582F/BLE/HAL/RTC.c \
E:/WCH_Files/BLE-TEMP-SHT40-CH582F/BLE/HAL/SLEEP.c 

OBJS += \
./HAL/MCU.o \
./HAL/RTC.o \
./HAL/SLEEP.o 

C_DEPS += \
./HAL/MCU.d \
./HAL/RTC.d \
./HAL/SLEEP.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/MCU.o: E:/WCH_Files/BLE-TEMP-SHT40-CH582F/BLE/HAL/MCU.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\Startup" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\BLE-TEMP-SHT40-CH582F\APP\include" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\BLE-TEMP-SHT40-CH582F\Profile\include" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\StdPeriphDriver\inc" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\HAL\include" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\Ld" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\LIB" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
HAL/RTC.o: E:/WCH_Files/BLE-TEMP-SHT40-CH582F/BLE/HAL/RTC.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\Startup" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\BLE-TEMP-SHT40-CH582F\APP\include" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\BLE-TEMP-SHT40-CH582F\Profile\include" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\StdPeriphDriver\inc" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\HAL\include" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\Ld" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\LIB" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
HAL/SLEEP.o: E:/WCH_Files/BLE-TEMP-SHT40-CH582F/BLE/HAL/SLEEP.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\Startup" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\BLE-TEMP-SHT40-CH582F\APP\include" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\BLE-TEMP-SHT40-CH582F\Profile\include" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\StdPeriphDriver\inc" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\HAL\include" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\Ld" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\BLE\LIB" -I"E:\WCH_Files\BLE-TEMP-SHT40-CH582F\SRC\RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

