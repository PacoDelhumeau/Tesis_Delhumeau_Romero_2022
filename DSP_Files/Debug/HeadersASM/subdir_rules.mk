################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
HeadersASM/%.obj: ../HeadersASM/%.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/Program Files (x86)/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O0 --fp_mode=relaxed --include_path="D:/Files/users/CCS/TesisFinal1" --include_path="D:/Program Files (x86)/ti/c2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/include" --include_path="D:/Program Files (x86)/ti/c2000/C2000Ware_3_04_00_00/device_support/f2837xs/headers/include" --include_path="D:/Program Files (x86)/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="HeadersASM/$(basename $(<F)).d_raw" --obj_directory="HeadersASM" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


