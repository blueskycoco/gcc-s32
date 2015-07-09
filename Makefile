#******************************************************************************
#
# Makefile - Rules for building the uart_echo example.
#
# Copyright (c) 2013-2014 Texas Instruments Incorporated.  All rights reserved.
# Software License Agreement
# 
# Texas Instruments (TI) is supplying this software for use solely and
# exclusively on TI's microcontroller products. The software is owned by
# TI and/or its suppliers, and is protected under applicable copyright
# laws. You may not combine this software with "viral" open-source
# software in order to form a larger program.
# 
# THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
# NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
# NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
# CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
# DAMAGES, FOR ANY REASON WHATSOEVER.
# 
# This is part of revision 2.1.0.12573 of the EK-TM4C1294XL Firmware Package.
#
#******************************************************************************

#
# The base directory for TivaWare.
#
ROOT=.

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find header files that do not live in the source directory.
#
IPATH=./inc
VPATH=./driver
VPATH+=./driverlib
#
# The default rule, which causes the uart_echo example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/pstn.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}
install:
#	openocd -f openocd.cfg -c "flash_image" -d 3
	"c:\Program Files\STMicroelectronics\Software\Flash Loader Demonstrator\STMFlashLoader.exe" -c --pn 1 --br 115200 --db 8 --pr EVEN --sb 1 --ec OFF  --to 10000 -i STM32F0_16K -e --sec 13 0 1 2 3 4 5 6 7 8 9 10 11 12 -d --fn gcc/pstn.bin --a 0x08000000 --v -r --a 8000000

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the uart_echo example.
#
${COMPILER}/pstn.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/pstn.axf: ${COMPILER}/system_stm32f0xx.o
${COMPILER}/pstn.axf: ${COMPILER}/main.o
${COMPILER}/pstn.axf: ${COMPILER}/syscalls.o
${COMPILER}/pstn.axf: ${COMPILER}/delay.o
${COMPILER}/pstn.axf: ${COMPILER}/cmx865a.o
${COMPILER}/pstn.axf: ${COMPILER}/tlv320aic12k.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_adc.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_can.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_cec.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_comp.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_crc.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_crs.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_dac.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_dbgmcu.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_dma.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_exti.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_flash.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_gpio.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_i2c.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_iwdg.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_misc.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_pwr.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_rcc.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_rtc.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_spi.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_syscfg.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_tim.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_usart.o
${COMPILER}/pstn.axf: ${COMPILER}/stm32f0xx_wwdg.o
#${COMPILER}/pstn.axf: ${ROOT}/driverlib/${COMPILER}/libdriver.a
${COMPILER}/pstn.axf: stm32f030.ld
SCATTERgcc_pstn=stm32f030.ld
ENTRY_pstn=ResetISR
CFLAGSgcc=-DUSE_STDPERIPH_DRIVER

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
