################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
libs/drivers/rit128x96x4.obj: /home/max_power/workspace_v6_1/graphics/ccs/libs/drivers/rit128x96x4.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/opt/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M3 --code_state=16 -me -Ooff --include_path="/opt/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="/home/max_power/workspace_v6_1/graphics" --include_path="/home/max_power/workspace_v6_1/graphics/ccs" --include_path="/home/max_power/workspace_v6_1/graphics/ccs/libs" --include_path="x86_64" -g --gcc --define=ccs --define=PART_LM3S8962 --define="ccs" --diag_warning=225 --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="libs/drivers/rit128x96x4.pp" --obj_directory="libs/drivers" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


