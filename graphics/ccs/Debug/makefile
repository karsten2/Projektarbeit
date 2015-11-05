################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /opt/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./can_net.obj" \
"./dame.obj" \
"./graphics.obj" \
"./main.obj" \
"./random.obj" \
"./startup_ccs.obj" \
"./libs/drivers/rit128x96x4.obj" \
"./libs/driverlib/adc.obj" \
"./libs/driverlib/can.obj" \
"./libs/driverlib/comp.obj" \
"./libs/driverlib/cpu.obj" \
"./libs/driverlib/epi.obj" \
"./libs/driverlib/ethernet.obj" \
"./libs/driverlib/flash.obj" \
"./libs/driverlib/gpio.obj" \
"./libs/driverlib/hibernate.obj" \
"./libs/driverlib/i2c.obj" \
"./libs/driverlib/i2s.obj" \
"./libs/driverlib/interrupt.obj" \
"./libs/driverlib/mpu.obj" \
"./libs/driverlib/pwm.obj" \
"./libs/driverlib/qei.obj" \
"./libs/driverlib/ssi.obj" \
"./libs/driverlib/sysctl.obj" \
"./libs/driverlib/systick.obj" \
"./libs/driverlib/timer.obj" \
"./libs/driverlib/uart.obj" \
"./libs/driverlib/udma.obj" \
"./libs/driverlib/usb.obj" \
"./libs/driverlib/watchdog.obj" \
"/home/max_power/workspace_v6_1/graphics/graphics_ccs.cmd" \
"../libs/driverlib/rvmdk/driverlib.lib" \
"../libs/driverlib/ccs/Debug/driverlib.lib" \
$(GEN_CMDS__FLAG) \
-l"rtsv7M3_T_le_eabi.lib" \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include libs/drivers/subdir_vars.mk
-include libs/driverlib/subdir_vars.mk
-include libs/driverlib/rvmdk/subdir_vars.mk
-include libs/driverlib/gcc/subdir_vars.mk
-include libs/driverlib/ewarm/Exe/subdir_vars.mk
-include libs/driverlib/ccs/Debug/subdir_vars.mk
-include subdir_rules.mk
-include libs/drivers/subdir_rules.mk
-include libs/driverlib/subdir_rules.mk
-include libs/driverlib/rvmdk/subdir_rules.mk
-include libs/driverlib/gcc/subdir_rules.mk
-include libs/driverlib/ewarm/Exe/subdir_rules.mk
-include libs/driverlib/ccs/Debug/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
graphics.out \

EXE_OUTPUTS__QUOTED += \
"graphics.out" \

BIN_OUTPUTS += \
graphics.bin \

BIN_OUTPUTS__QUOTED += \
"graphics.bin" \


# All Target
all: graphics.out

# Tool invocations
graphics.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(A_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"/opt/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M3 --code_state=16 -me -Ooff -g --gcc --define=ccs --define=PART_LM3S8962 --define="ccs" --diag_warning=225 --gen_func_subsections=on --abi=eabi --ual -z -m"graphics_ccs.map" --heap_size=0 --stack_size=256 -i"/opt/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/lib" -i"/opt/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --reread_libs --warn_sections --xml_link_info="graphics_linkInfo.xml" --rom_model -o "graphics.out" $(ORDERED_OBJS) $(A_SRCS)
	@echo 'Finished building target: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

graphics.bin: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"/opt/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armhex"  -o "graphics.bin" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "can_net.pp" "dame.pp" "graphics.pp" "main.pp" "random.pp" "startup_ccs.pp" "libs/drivers/rit128x96x4.pp" "libs/driverlib/adc.pp" "libs/driverlib/can.pp" "libs/driverlib/comp.pp" "libs/driverlib/cpu.pp" "libs/driverlib/epi.pp" "libs/driverlib/ethernet.pp" "libs/driverlib/flash.pp" "libs/driverlib/gpio.pp" "libs/driverlib/hibernate.pp" "libs/driverlib/i2c.pp" "libs/driverlib/i2s.pp" "libs/driverlib/interrupt.pp" "libs/driverlib/mpu.pp" "libs/driverlib/pwm.pp" "libs/driverlib/qei.pp" "libs/driverlib/ssi.pp" "libs/driverlib/sysctl.pp" "libs/driverlib/systick.pp" "libs/driverlib/timer.pp" "libs/driverlib/uart.pp" "libs/driverlib/udma.pp" "libs/driverlib/usb.pp" "libs/driverlib/watchdog.pp" 
	-$(RM) "can_net.obj" "dame.obj" "graphics.obj" "main.obj" "random.obj" "startup_ccs.obj" "libs/drivers/rit128x96x4.obj" "libs/driverlib/adc.obj" "libs/driverlib/can.obj" "libs/driverlib/comp.obj" "libs/driverlib/cpu.obj" "libs/driverlib/epi.obj" "libs/driverlib/ethernet.obj" "libs/driverlib/flash.obj" "libs/driverlib/gpio.obj" "libs/driverlib/hibernate.obj" "libs/driverlib/i2c.obj" "libs/driverlib/i2s.obj" "libs/driverlib/interrupt.obj" "libs/driverlib/mpu.obj" "libs/driverlib/pwm.obj" "libs/driverlib/qei.obj" "libs/driverlib/ssi.obj" "libs/driverlib/sysctl.obj" "libs/driverlib/systick.obj" "libs/driverlib/timer.obj" "libs/driverlib/uart.obj" "libs/driverlib/udma.obj" "libs/driverlib/usb.obj" "libs/driverlib/watchdog.obj" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"/opt/ti/ccsv6/utils/tiobj2bin/tiobj2bin.bat" "graphics.out" "graphics.bin" "/opt/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/ofd470.exe" "/opt/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/hex470.exe" "/opt/ti/ccsv6/utils/tiobj2bin/mkhex4bin.exe"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets

