################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --cla_support=cla1 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="C:/ti/C2000Ware_5_03_00_00/device_support/f2837xd/common/source" --include_path="C:/ti/C2000Ware_5_03_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/C2000Ware_5_03_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all -g --define=CPU1 --define=_LAUNCHXL_F28379D --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --cla_support=cla1 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="C:/ti/C2000Ware_5_03_00_00/device_support/f2837xd/common/source" --include_path="C:/ti/C2000Ware_5_03_00_00/device_support/f2837xd/common/include" --include_path="C:/ti/C2000Ware_5_03_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all -g --define=CPU1 --define=_LAUNCHXL_F28379D --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


