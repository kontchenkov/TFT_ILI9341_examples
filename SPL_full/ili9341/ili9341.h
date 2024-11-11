//*****************************************************************************
//
//! \file ili9341.h
//! \brief Prototypes for Character LCD ILI9341 Driver.
//! \version 2.1.1.0
//! \date 10/18/2011
//! \author CooCoX
//! \copy
//!
//! Copyright (c)  2011, CooCoX
//! All rights reserved.
//!
//! Redistribution and use in source and binary forms, with or without
//! modification, are permitted provided that the following conditions
//! are met:
//!
//!     * Redistributions of source code must retain the above copyright
//! notice, this list of conditions and the following disclaimer.
//!     * Redistributions in binary form must reproduce the above copyright
//! notice, this list of conditions and the following disclaimer in the
//! documentation and/or other materials provided with the distribution.
//!     * Neither the name of the <ORGANIZATION> nor the names of its
//! contributors may be used to endorse or promote products derived
//! from this software without specific prior written permission.
//!
//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//! AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//! IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//! ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
//! LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//! CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//! SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//! INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//! CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//! ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
//! THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

#ifndef __ILI9341_H__
#define __ILI9341_H__

#include "gpio.h"
#include "delay.h"
#include "8X16.h"

//*****************************************************************************
//
//! \addtogroup ILI9341S_Config_Pins ILI9341S Driver Pin Configurtion
//!
//! \brief GPIO Pins Configurtions that communication with the LCD should be
//! set before using this driver.
//!
//! @{
//
//*****************************************************************************

// Подключение дисплея
#define ILI9341_PIN_D7          GPIO_Pin_7
#define ILI9341_PIN_D6          GPIO_Pin_6
#define ILI9341_PIN_D5          GPIO_Pin_5
#define ILI9341_PIN_D4          GPIO_Pin_4
#define ILI9341_PIN_D3          GPIO_Pin_3
#define ILI9341_PIN_D2          GPIO_Pin_2
#define ILI9341_PIN_D1          GPIO_Pin_1
#define ILI9341_PIN_D0          GPIO_Pin_0

#define ILI9341_PIN_CS          GPIO_Pin_15
#define ILI9341_PIN_RS          GPIO_Pin_12
#define ILI9341_PIN_WR          GPIO_Pin_11
#define ILI9341_PIN_RD          GPIO_Pin_10
#define ILI9341_PIN_RST         GPIO_Pin_8

#define LCD_GPIO_DEF GPIOE

//*****************************************************************************
//
//! \addtogroup ILI9341S_Panel_Size ILI9341S LCD panel size
//! @{
//
//*****************************************************************************

#define LCD_VERTICAL_MAX        320
#define LCD_HORIZONTAL_MAX      240

//*****************************************************************************
//
//! \addtogroup ILI9341S_Signals ILI9341 Signals
//! @{
//
//*****************************************************************************

#define ILI9341_CS_DISABLE      1
#define ILI9341_CS_ENABLE       0

#define ILI9341_RS_COMMAND      0
#define ILI9341_RS_DATA         1

#define ILI9341_WR_LOW          0
#define ILI9341_WR_HIGH         1

#define ILI9341_RD_READ         0
#define ILI9341_RD_WRITE        1

#define ILI9341_RD_LOW          0
#define ILI9341_RD_HIGH         1

#define ILI9341_RST_ENABLE      0
#define ILI9341_RST_DISABLE     1

//*****************************************************************************
//
//! \addtogroup ILI9341S_Exported_APIs  ILI9341S Driver APIs
//! \brief API Refrence of ILI9341S Driver.
//! @{
//
//*****************************************************************************
extern void ILI9341Init(void);

extern void ILI9341DisplayClear(uint32_t ulColor);

extern void ILI9341DisplayOff(void);
extern void ILI9341DisplayOn(void);

extern void ILI9341DisplayRectFill(uint16_t usStartX, uint16_t usStartY,
                                   uint16_t usEndX, uint16_t usEndY,
                                   uint32_t ulColor);

extern void ILI9341DrawOnePixel(uint16_t usX, uint16_t usY,
                                uint32_t ulValue);

extern void ILI9341DrawHorizontalLine(uint16_t usStartX, uint16_t usEndX,
                                      uint16_t usY, uint32_t ulColor);

extern void ILI9341DrawVerticalLine(uint16_t usX, uint16_t usStartY,
                                    uint16_t usEndY, uint32_t ulColor);

extern void ILI9341PutChar8x16(uint16_t usX, uint16_t usY, char c,
                               uint32_t fColor, uint32_t bColor);

extern void ILI9341DisplayString(uint16_t usX, uint16_t usY, const char* pcString,
                                 uint32_t fColor, uint32_t bColor);

extern void ILI9341DisplayNum(uint16_t usX, uint16_t usY, uint32_t ulNum,
                              uint32_t fColor, uint32_t bColor);

extern void ILI9341DisplayBmp(uint16_t usX, uint16_t usY, uint16_t usSizeX,
                              uint16_t usSizeY, uint8_t const *Bmp);

extern uint16_t RGB(uint8_t r, uint8_t g, uint8_t b);

//*****************************************************************************

extern void ILI9341DisplayInversionOn(void);
extern void ILI9341DisplayInversionOff(void);

extern void ILI9341EnterSleepMode(void);
extern void ILI9341ExitSleepMode(void);

//
// If the LCD API can not meet your need, you can use the API below operated
// on the lowlayer.
//
extern void ILI9341WriteCmd(uint8_t ucCmd);
extern void ILI9341WriteData(uint16_t ucData);
extern void ILI9341Write8Bit(uint8_t ucData);
extern void ILI9341WritePixelData(uint32_t ulData);
extern void ILI9341SetCurPos(uint32_t ulStartX, uint32_t ulEndX,
                  uint32_t ulStartY, uint32_t ulEndY);

// Самостоятельно разработанные функции
extern uint16_t RGB(uint8_t r, uint8_t g, uint8_t b);
extern void DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t bordercolor);

#endif //__ILI9341_H__
