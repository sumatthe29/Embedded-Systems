################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/Source_Files/subdir.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include CMSIS/EFM32PG12B/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := MS_2370_PWM
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: MS_2370_PWM.axf

# Tool invocations
MS_2370_PWM.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "MS_2370_PWM.ld" -Xlinker --gc-sections -Xlinker -Map="MS_2370_PWM.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o MS_2370_PWM.axf "./CMSIS/EFM32PG12B/startup_gcc_efm32pg12b.o" "./CMSIS/EFM32PG12B/system_efm32pg12b.o" "./emlib/em_acmp.o" "./emlib/em_adc.o" "./emlib/em_aes.o" "./emlib/em_assert.o" "./emlib/em_burtc.o" "./emlib/em_can.o" "./emlib/em_cmu.o" "./emlib/em_cmu_fpga.o" "./emlib/em_core.o" "./emlib/em_cryotimer.o" "./emlib/em_crypto.o" "./emlib/em_csen.o" "./emlib/em_dac.o" "./emlib/em_dbg.o" "./emlib/em_dma.o" "./emlib/em_ebi.o" "./emlib/em_emu.o" "./emlib/em_eusart.o" "./emlib/em_gpcrc.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_iadc.o" "./emlib/em_idac.o" "./emlib/em_lcd.o" "./emlib/em_ldma.o" "./emlib/em_lesense.o" "./emlib/em_letimer.o" "./emlib/em_leuart.o" "./emlib/em_msc.o" "./emlib/em_opamp.o" "./emlib/em_pcnt.o" "./emlib/em_pdm.o" "./emlib/em_prs.o" "./emlib/em_qspi.o" "./emlib/em_rmu.o" "./emlib/em_rtc.o" "./emlib/em_rtcc.o" "./emlib/em_se.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./emlib/em_usart.o" "./emlib/em_vcmp.o" "./emlib/em_vdac.o" "./emlib/em_wdog.o" "./src/Source_Files/app.o" "./src/Source_Files/cmu.o" "./src/Source_Files/gpio.o" "./src/Source_Files/letimer.o" "./src/main.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: MS_2370_PWM.hex'
	arm-none-eabi-objcopy -O ihex "MS_2370_PWM.axf" "MS_2370_PWM.hex"
	@echo ' '

	@echo 'Building bin file: MS_2370_PWM.bin'
	arm-none-eabi-objcopy -O binary "MS_2370_PWM.axf" "MS_2370_PWM.bin"
	@echo ' '

	@echo 'Building s37 file: MS_2370_PWM.s37'
	arm-none-eabi-objcopy -O srec "MS_2370_PWM.axf" "MS_2370_PWM.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "MS_2370_PWM.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) MS_2370_PWM.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
