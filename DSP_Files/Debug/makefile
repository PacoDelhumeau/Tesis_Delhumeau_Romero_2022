################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/Program Files (x86)/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./F2837xS_Adc.obj" \
"./F2837xS_CodeStartBranch.obj" \
"./F2837xS_DefaultISR.obj" \
"./F2837xS_EPwm.obj" \
"./F2837xS_GlobalVariableDefs.obj" \
"./F2837xS_Gpio.obj" \
"./F2837xS_PieCtrl.obj" \
"./F2837xS_PieVect.obj" \
"./F2837xS_SysCtrl.obj" \
"./F2837xS_TempSensorConv.obj" \
"./F2837xS_usDelay.obj" \
"./tesisFinalMain.obj" \
"./HeadersASM/efectos.obj" \
"./HeadersASM/filtrosIIRFmain.obj" \
"../2837xS_Generic_RAM_lnk.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"D:/Program Files (x86)/ti/c2000/C2000Ware_3_04_00_00/device_support/f2837xs/headers/cmd/F2837xS_Headers_nonBIOS.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include HeadersASM/subdir_vars.mk
-include subdir_rules.mk
-include HeadersASM/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
TesisFinal1.out \

EXE_OUTPUTS__QUOTED += \
"TesisFinal1.out" \

BIN_OUTPUTS += \
TesisFinal1.hex \

BIN_OUTPUTS__QUOTED += \
"TesisFinal1.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "TesisFinal1.out"

# Tool invocations
TesisFinal1.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"D:/Program Files (x86)/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O0 --fp_mode=relaxed --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"TesisFinal1.map" --stack_size=0x200 --warn_sections -i"D:/Program Files (x86)/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/lib" -i"D:/Program Files (x86)/ti/c2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/cmd" -i"D:/Program Files (x86)/ti/c2000/C2000Ware_3_04_00_00/device_support/f2837xs/headers/cmd" -i"D:/Program Files (x86)/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="TesisFinal1_linkInfo.xml" --rom_model -o "TesisFinal1.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

TesisFinal1.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: C2000 Hex Utility'
	"D:/Program Files (x86)/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.8.LTS/bin/hex2000"  -o "TesisFinal1.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "F2837xS_Adc.obj" "F2837xS_CodeStartBranch.obj" "F2837xS_DefaultISR.obj" "F2837xS_EPwm.obj" "F2837xS_GlobalVariableDefs.obj" "F2837xS_Gpio.obj" "F2837xS_PieCtrl.obj" "F2837xS_PieVect.obj" "F2837xS_SysCtrl.obj" "F2837xS_TempSensorConv.obj" "F2837xS_usDelay.obj" "tesisFinalMain.obj" "HeadersASM\efectos.obj" "HeadersASM\filtrosIIRFmain.obj" 
	-$(RM) "F2837xS_Adc.d" "F2837xS_DefaultISR.d" "F2837xS_EPwm.d" "F2837xS_GlobalVariableDefs.d" "F2837xS_Gpio.d" "F2837xS_PieCtrl.d" "F2837xS_PieVect.d" "F2837xS_SysCtrl.d" "F2837xS_TempSensorConv.d" "tesisFinalMain.d" 
	-$(RM) "F2837xS_CodeStartBranch.d" "F2837xS_usDelay.d" "HeadersASM\efectos.d" "HeadersASM\filtrosIIRFmain.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

