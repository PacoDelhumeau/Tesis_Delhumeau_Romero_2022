################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../2837xS_Generic_RAM_lnk.cmd 

ASM_SRCS += \
D:/Program\ Files\ (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_CodeStartBranch.asm \
D:/Program\ Files\ (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_usDelay.asm 

C_SRCS += \
D:/Program\ Files\ (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_Adc.c \
D:/Program\ Files\ (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_DefaultISR.c \
D:/Program\ Files\ (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_EPwm.c \
D:/Program\ Files\ (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/headers/source/F2837xS_GlobalVariableDefs.c \
D:/Program\ Files\ (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_Gpio.c \
D:/Program\ Files\ (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_PieCtrl.c \
D:/Program\ Files\ (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_PieVect.c \
D:/Program\ Files\ (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_SysCtrl.c \
D:/Program\ Files\ (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_TempSensorConv.c \
../tesisFinalMain.c 

C_DEPS += \
./F2837xS_Adc.d \
./F2837xS_DefaultISR.d \
./F2837xS_EPwm.d \
./F2837xS_GlobalVariableDefs.d \
./F2837xS_Gpio.d \
./F2837xS_PieCtrl.d \
./F2837xS_PieVect.d \
./F2837xS_SysCtrl.d \
./F2837xS_TempSensorConv.d \
./tesisFinalMain.d 

OBJS += \
./F2837xS_Adc.obj \
./F2837xS_CodeStartBranch.obj \
./F2837xS_DefaultISR.obj \
./F2837xS_EPwm.obj \
./F2837xS_GlobalVariableDefs.obj \
./F2837xS_Gpio.obj \
./F2837xS_PieCtrl.obj \
./F2837xS_PieVect.obj \
./F2837xS_SysCtrl.obj \
./F2837xS_TempSensorConv.obj \
./F2837xS_usDelay.obj \
./tesisFinalMain.obj 

ASM_DEPS += \
./F2837xS_CodeStartBranch.d \
./F2837xS_usDelay.d 

OBJS__QUOTED += \
"F2837xS_Adc.obj" \
"F2837xS_CodeStartBranch.obj" \
"F2837xS_DefaultISR.obj" \
"F2837xS_EPwm.obj" \
"F2837xS_GlobalVariableDefs.obj" \
"F2837xS_Gpio.obj" \
"F2837xS_PieCtrl.obj" \
"F2837xS_PieVect.obj" \
"F2837xS_SysCtrl.obj" \
"F2837xS_TempSensorConv.obj" \
"F2837xS_usDelay.obj" \
"tesisFinalMain.obj" 

C_DEPS__QUOTED += \
"F2837xS_Adc.d" \
"F2837xS_DefaultISR.d" \
"F2837xS_EPwm.d" \
"F2837xS_GlobalVariableDefs.d" \
"F2837xS_Gpio.d" \
"F2837xS_PieCtrl.d" \
"F2837xS_PieVect.d" \
"F2837xS_SysCtrl.d" \
"F2837xS_TempSensorConv.d" \
"tesisFinalMain.d" 

ASM_DEPS__QUOTED += \
"F2837xS_CodeStartBranch.d" \
"F2837xS_usDelay.d" 

C_SRCS__QUOTED += \
"D:/Program Files (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_Adc.c" \
"D:/Program Files (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_DefaultISR.c" \
"D:/Program Files (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_EPwm.c" \
"D:/Program Files (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/headers/source/F2837xS_GlobalVariableDefs.c" \
"D:/Program Files (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_Gpio.c" \
"D:/Program Files (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_PieCtrl.c" \
"D:/Program Files (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_PieVect.c" \
"D:/Program Files (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_SysCtrl.c" \
"D:/Program Files (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_TempSensorConv.c" \
"../tesisFinalMain.c" 

ASM_SRCS__QUOTED += \
"D:/Program Files (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_CodeStartBranch.asm" \
"D:/Program Files (x86)/ti/C2000/C2000Ware_3_04_00_00/device_support/f2837xs/common/source/F2837xS_usDelay.asm" 


