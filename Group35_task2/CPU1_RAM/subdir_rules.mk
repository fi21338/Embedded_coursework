################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"/Applications/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="C:/ti/c2000/C2000Ware_5_02_00_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/c2000/C2000Ware_5_02_00_00/driverlib/f280013x/driverlib" --include_path="C:/ti/c2000/C2000Ware_5_02_00_00/device_support/f2837xd/headers/include" --include_path="C:/ti/c2000/C2000Ware_5_02_00_00/device_support/f2837xd/common/include" --include_path="/Users/damianstone/Desktop/embedding-workspace/FIR_Timer_NR" --include_path="/Users/damianstone/Desktop/embedding-workspace/FIR_Timer_NR/device" --include_path="/Users/damianstone/ti/C2000Ware_5_03_00_00/driverlib/f2837xd/driverlib" --include_path="/Applications/ti/ccs1280/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=DEBUG --define=CPU1 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="/Users/damianstone/Desktop/embedding-workspace/FIR_Timer_NR/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


