################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include Drivers/subdir.mk
-include CMSIS/efm32gg/subdir.mk
-include BSP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: EFM32GG_rtx_blink.axf

# Tool invocations
EFM32GG_rtx_blink.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb  --specs=nano.specs -L"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/RTX/LIB/GCC/" -Xlinker --gc-sections -Xlinker -Map="EFM32GG_rtx_blink.map" -T "EFM32GG_rtx_blink.ld" -o EFM32GG_rtx_blink.axf "./src/RTX_Conf_CM.o" "./src/rtx_blink.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_lcd.o" "./emlib/em_system.o" "./Drivers/segmentlcd.o" "./CMSIS/efm32gg/startup_gcc_efm32gg.o" "./CMSIS/efm32gg/system_efm32gg.o" "./BSP/bsp_stk_leds.o" "./BSP/bsp_trace.o" "-lRTX_CM3" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: EFM32GG_rtx_blink.hex'
	arm-none-eabi-objcopy -O ihex "EFM32GG_rtx_blink.axf" "EFM32GG_rtx_blink.hex"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "EFM32GG_rtx_blink.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) EFM32GG_rtx_blink.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
