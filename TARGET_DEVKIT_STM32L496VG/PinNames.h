/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

/* MBED TARGET LIST: MTS_DRAGONFLY_L471QG */

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "PinNamesTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ALT0  = 0x100,
    ALT1  = 0x200,
    ALT2  = 0x300,
} ALTx;

typedef enum {
    PA_0       = 0x00,
    PA_0_ALT0  = PA_0  | ALT0, // same pin used for alternate HW
    PA_1       = 0x01,
    PA_1_ALT0  = PA_1  | ALT0, // same pin used for alternate HW
    PA_2       = 0x02,
    PA_2_ALT0  = PA_2  | ALT0, // same pin used for alternate HW
    PA_3       = 0x03,
    PA_3_ALT0  = PA_3  | ALT0, // same pin used for alternate HW
    PA_4       = 0x04,
    PA_4_ALT0  = PA_4  | ALT0, // same pin used for alternate HW
    PA_5       = 0x05,
    PA_5_ALT0  = PA_5  | ALT0, // same pin used for alternate HW
    PA_6       = 0x06,
    PA_6_ALT0  = PA_6  | ALT0, // same pin used for alternate HW
    PA_7       = 0x07,
    PA_7_ALT0  = PA_7  | ALT0, // same pin used for alternate HW
    PA_7_ALT1  = PA_7  | ALT1, // same pin used for alternate HW
    PA_7_ALT2  = PA_7  | ALT2, // same pin used for alternate HW
    PA_8       = 0x08,
    PA_9       = 0x09,
    PA_10      = 0x0A,
    PA_11      = 0x0B,
    PA_12      = 0x0C,
    PA_13      = 0x0D,
    PA_14      = 0x0E,
    PA_15      = 0x0F,
    PA_15_ALT0 = PA_15 | ALT0, // same pin used for alternate HW

    PB_0       = 0x10,
    PB_0_ALT0  = PB_0  | ALT0, // same pin used for alternate HW
    PB_0_ALT1  = PB_0  | ALT1, // same pin used for alternate HW
    PB_1       = 0x11,
    PB_1_ALT0  = PB_1  | ALT0, // same pin used for alternate HW
    PB_1_ALT1  = PB_1  | ALT1, // same pin used for alternate HW
    PB_2       = 0x12,
    PB_3       = 0x13,
    PB_3_ALT0  = PB_3  | ALT0, // same pin used for alternate HW
    PB_4       = 0x14,
    PB_4_ALT0  = PB_4  | ALT0, // same pin used for alternate HW
    PB_5       = 0x15,
    PB_5_ALT0  = PB_5  | ALT0, // same pin used for alternate HW
    PB_6       = 0x16,
    PB_6_ALT0  = PB_6  | ALT0, // same pin used for alternate HW
    PB_7       = 0x17,
    PB_7_ALT0  = PB_7  | ALT0, // same pin used for alternate HW
    PB_8       = 0x18,
    PB_8_ALT0  = PB_8  | ALT0, // same pin used for alternate HW
    PB_9       = 0x19,
    PB_9_ALT0  = PB_9  | ALT0, // same pin used for alternate HW
    PB_10      = 0x1A,
    PB_10_ALT0 = PB_10 | ALT0, // same pin used for alternate HW
    PB_11      = 0x1B,
    PB_11_ALT0 = PB_11 | ALT0, // same pin used for alternate HW
    PB_12      = 0x1C,
    PB_13      = 0x1D,
    PB_13_ALT0 = PB_13 | ALT0, // same pin used for alternate HW
    PB_14      = 0x1E,
    PB_14_ALT0 = PB_14 | ALT0, // same pin used for alternate HW
    PB_14_ALT1 = PB_14 | ALT1, // same pin used for alternate HW
    PB_15      = 0x1F,
    PB_15_ALT0 = PB_15 | ALT0, // same pin used for alternate HW
    PB_15_ALT1 = PB_15 | ALT1, // same pin used for alternate HW

    PC_0       = 0x20,
    PC_0_ALT0  = PC_0  | ALT0, // same pin used for alternate HW
    PC_0_ALT1  = PC_0  | ALT1, // same pin used for alternate HW
    PC_1       = 0x21,
    PC_1_ALT0  = PC_1  | ALT0, // same pin used for alternate HW
    PC_1_ALT1  = PC_1  | ALT1, // same pin used for alternate HW
    PC_2       = 0x22,
    PC_2_ALT0  = PC_2  | ALT0, // same pin used for alternate HW
    PC_2_ALT1  = PC_2  | ALT1, // same pin used for alternate HW
    PC_3       = 0x23,
    PC_3_ALT0  = PC_3  | ALT0, // same pin used for alternate HW
    PC_3_ALT1  = PC_3  | ALT1, // same pin used for alternate HW
    PC_4       = 0x24,
    PC_4_ALT0  = PC_4  | ALT0, // same pin used for alternate HW
    PC_5       = 0x25,
    PC_5_ALT0  = PC_5  | ALT0, // same pin used for alternate HW
    PC_6       = 0x26,
    PC_6_ALT0  = PC_6  | ALT0, // same pin used for alternate HW
    PC_7       = 0x27,
    PC_7_ALT0  = PC_7  | ALT0, // same pin used for alternate HW
    PC_8       = 0x28,
    PC_8_ALT0  = PC_8  | ALT0, // same pin used for alternate HW
    PC_9       = 0x29,
    PC_9_ALT0  = PC_9  | ALT0, // same pin used for alternate HW
    PC_10      = 0x2A,
    PC_10_ALT0 = PC_10 | ALT0, // same pin used for alternate HW
    PC_11      = 0x2B,
    PC_11_ALT0 = PC_11 | ALT0, // same pin used for alternate HW
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,

    PD_0 = 0x30,
    PD_1 = 0x31,
    PD_2 = 0x32,
    PD_3 = 0x33,
    PD_4 = 0x34,
    PD_5 = 0x35,
    PD_6 = 0x36,
    PD_7 = 0x37,
    PD_8 = 0x38,
    PD_9 = 0x39,
    PD_10 = 0x3A,
    PD_11 = 0x3B,
    PD_12 = 0x3C,
    PD_13 = 0x3D,
    PD_14 = 0x3E,
    PD_15 = 0x3F,

    PE_0 = 0x40,
    PE_1 = 0x41,
    PE_2 = 0x42,
    PE_3 = 0x43,
    PE_4 = 0x44,
    PE_5 = 0x45,
    PE_6 = 0x46,
    PE_7 = 0x47,
    PE_8 = 0x48,
    PE_9 = 0x49,
    PE_10 = 0x4A,
    PE_11 = 0x4B,
    PE_12 = 0x4C,
    PE_13 = 0x4D,
    PE_14 = 0x4E,
    PE_15 = 0x4F,

    PH_0 = 0x70,
    PH_1 = 0x71,

    // ADC internal channels
    ADC_TEMP = 0xF0,
    ADC_VREF = 0xF1,
    ADC_VBAT = 0xF2,

    // USB debug port
    CONSOLE_TX = PB_10,
    CONSOLE_RX = PB_11,
    UART1_TX = PA_2,
    UART1_RX = PA_3,

    // SPI1 for LCD(ST7789H2)
    SPI1_MOSI = PA_7,
    LCD_DC = PA_6,
    SPI1_SCK  = PA_5,
    SPI1_SSEL = PA_4,
    LCD_PWR_EN = PE_7,
    LCD_RST = PB_2,

    // I2C for sensors
    I2C2_SCL = PB_13,
    I2C2_SDA = PB_14,
    I2C3_SCL = PC_0,
    I2C3_SDA = PC_1,
    I2C4_SCL = PD_12,
    I2C4_SDA = PD_13,
    	
    // SDMMC
    SDMMC_CMD = PD_2,
    SDMMC_CLK = PC_12,
    SDMMC_D0 = PC_8,
    SDMMC_D1 = PC_9,
    SDMMC_D2 = PC_10,
    SDMMC_D3 = PC_11,
    
    LED1 = PB_6,
    LED2 = PE_3,
    LED3 = PD_15,

    BUTTON1 = PE_11, // M¼ü
    BUTTON2 = PE_14, // A¼ü
    BUTTON3 = PE_10, // B¼ü

    // Not connected
    NC = (int) 0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif

#endif
