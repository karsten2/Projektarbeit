################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
uC-CPU/cpu_core.obj: E:/GitHub/Projektarbeit\ SVN/Micrium/Software/uC-CPU/cpu_core.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"E:/Program Files (x86)/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -O2 -g --include_path="E:/Program Files (x86)/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/include" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/EvalBoards/TI/EKS-LM3S8962/CCS6/SOURCE/CFG" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/EvalBoards/TI/EKS-LM3S8962/CCS6/SOURCE/uCOS-III-Ex1" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/EvalBoards/TI/EKS-LM3S8962/CCS6/SOURCE/BSP" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/EvalBoards/TI/EKS-LM3S8962/CCS6/SOURCE/BSP/OS" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/EvalBoards/TI/EKS-LM3S8962/CCS6/SOURCE/BSP/RIT" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/CPU/LuminaryMicro/DriverLib/driverlib" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/uC-CPU" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/uC-LIB" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/uCOS-III/Source" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/uC-CPU/ARM-Cortex-M3/CCS5" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/uCOS-III/Ports/ARM-Cortex-M3/Generic/CCS5" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/uC-Probe/Target/Communication/Generic/Source" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/uC-Probe/Target/Communication/Generic/RS-232/Source" --include_path="E:/GitHub/Projektarbeit SVN/Micrium/Software/uC-Probe/Target/Communication/Generic/RS-232/Ports/LuminaryMicro/LM3Sxxxx" --gcc --define=ccs="ccs" --define=PART_LM3S9B96 --define=TARGET_IS_TEMPEST_RB1 --define=UART_BUFFERED --diag_warning=225 --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="uC-CPU/cpu_core.pp" --obj_directory="uC-CPU" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


