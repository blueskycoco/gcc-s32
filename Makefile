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

#
# The default rule, which causes the uart_echo example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/uart_echo.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}
install:
	openocd -f openocd.cfg -c "flash_image"

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the uart_echo example.
#
${COMPILER}/uart_echo.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/uart_echo.axf: ${COMPILER}/system_stm32f0xx.o
${COMPILER}/uart_echo.axf: ${COMPILER}/socket.o
${COMPILER}/uart_echo.axf: ${COMPILER}/main.o
${COMPILER}/uart_echo.axf: ${COMPILER}/delay.o
${COMPILER}/uart_echo.axf: ${COMPILER}/i2c_gpio.o
${COMPILER}/uart_echo.axf: ${ROOT}/driverlib/${COMPILER}/libdriver.a
${COMPILER}/uart_echo.axf: stm32f030.ld
SCATTERgcc_uart_echo=stm32f030.ld
ENTRY_uart_echo=ResetISR
CFLAGSgcc=-DUSE_STDPERIPH_DRIVER

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
