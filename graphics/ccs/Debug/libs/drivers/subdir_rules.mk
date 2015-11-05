################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
libs/drivers/rit128x96x4.obj: E:/Program\ Files\ (x86)/ti/ccsv6/eclipse/ROOT_PARENT/ccs/libs/drivers/rit128x96x4.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"E:/Program Files (x86)/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me --gcc --define="ccs" --define=PART_LM3S8962 --preproc_with_compile --preproc_dependency="libs/drivers/rit128x96x4.pp" --obj_directory="libs/drivers" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


