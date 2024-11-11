//*****************************************************************************
//
//! \file ili9341.c
//! \brief Driver for Character LCD ILI9341.
//! \version 2.1.1.0
//! \date 10/18/2011
//! \author CooCox
//! \copy
//!
//! Copyright (c)  2011, CooCox
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

#include "hw_ili9341.h"
#include "ili9341.h"



#define DPYCOLORTRANSLATE(c)    ((((c) & 0x00f80000) >> 8) |               \
                                 (((c) & 0x0000fc00) >> 5) |               \
                                 (((c) & 0x000000f8) >> 3))

//*****************************************************************************
//
//! \brief Write data or command to the ILI9341.
//!
//! \param ucRS determines if the IR or DR to select.
//!
//! The parameter of ucRS can be:
//! - ILI9341_RS_COMMAND - select the IR.
//! - ILI9341_RS_DATA - select the DR.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341Write(uint8_t ucRS, uint16_t ulInstruction)
{
    //TODO: Assert
    //
    // Check Arguments.
    //
    //xASSERT((ucRS == ILI9341_RS_COMMAND) || (ucDC == ILI9341_RS_DATA));
    //
    // DC:Command, CS:Select
    //

    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_CS, Bit_RESET);
    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_RS, ucRS);
    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_RD, Bit_SET);

    // Передаем на шину данных старший байт из ulInstruction
    LCD_GPIO_DEF->ODR &= 0xFF00;
    LCD_GPIO_DEF->ODR |= (ulInstruction>>8) & 0xFF;

    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_WR, Bit_RESET);
    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_WR, Bit_SET);

    // Передаем на шину данных младщий байт из ulInstruction
    LCD_GPIO_DEF->ODR &= 0xFF00;
    LCD_GPIO_DEF->ODR |= ulInstruction & 0x00FF;

    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_WR, Bit_RESET);
    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_WR, Bit_SET);

    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_CS, Bit_SET);

}

//*****************************************************************************
//
//! \brief Write a byte to the ILI9341.
//!
//! \param ucData determines which data will to be write.
//!
//! \return None.
//
//*****************************************************************************
void  ILI9341Write8Bit(uint8_t ucData)
{
    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_CS, Bit_RESET);
    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_RS, Bit_SET);
    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_RD, Bit_SET);

    // Передаем на шину данных байт
    LCD_GPIO_DEF->ODR &= 0xFF00;
    LCD_GPIO_DEF->ODR |= ucData & 0x00FF;

    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_WR, Bit_RESET);
    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_WR, Bit_SET);
    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_CS, Bit_SET);
}

//*****************************************************************************
//
//! \brief Read the state or data from the ILI9341.
//!
//! \param ucRS determines if the IR or DR to select.
//!
//! The parameter of ucDC can be:
//! - ILI9341_RS_COMMAND - select the IR.
//! - ILI9341_RS_DATA - select the DR.
//!
//! \return None.
//
//*****************************************************************************
/*uint32_t  ILI9341Read(uint8_t ucRS)
{
    uint32_t ulData = 0;

    //
    // Set D7 - D0 direction to GPIO Input
    //
    xGPIOSPinTypeGPIOInput(ILI9341_PIN_D7);
    xGPIOSPinTypeGPIOInput(ILI9341_PIN_D6);
    xGPIOSPinTypeGPIOInput(ILI9341_PIN_D5);
    xGPIOSPinTypeGPIOInput(ILI9341_PIN_D4);
    xGPIOSPinTypeGPIOInput(ILI9341_PIN_D3);
    xGPIOSPinTypeGPIOInput(ILI9341_PIN_D2);
    xGPIOSPinTypeGPIOInput(ILI9341_PIN_D1);
    xGPIOSPinTypeGPIOInput(ILI9341_PIN_D0);

    //
    // DC:Command, RD:Write/Read, CS:Enable
    //
    xGPIOSPinWrite(ILI9341_PIN_RS, ucRS);
    xGPIOSPinWrite(ILI9341_PIN_WR, ILI9341_WR_HIGH);
    xGPIOSPinWrite(ILI9341_PIN_CS, ILI9341_CS_ENABLE);

    xSysCtlDelay(100);

    //
    // Read the Data
    //
		xGPIOSPinWrite(ILI9341_PIN_RD, ILI9341_RD_LOW);
    xSysCtlDelay(100);
    xGPIOSPinWrite(ILI9341_PIN_RD, ILI9341_RD_HIGH);
    ulData |= xGPIOSPinRead(ILI9341_PIN_D7) << 7;
    ulData |= xGPIOSPinRead(ILI9341_PIN_D6) << 6;
    ulData |= xGPIOSPinRead(ILI9341_PIN_D5) << 5;
    ulData |= xGPIOSPinRead(ILI9341_PIN_D4) << 4;
    ulData |= xGPIOSPinRead(ILI9341_PIN_D3) << 3;
    ulData |= xGPIOSPinRead(ILI9341_PIN_D2) << 2;
    ulData |= xGPIOSPinRead(ILI9341_PIN_D1) << 1;
    ulData |= xGPIOSPinRead(ILI9341_PIN_D0) << 0;

    xGPIOSPinWrite(ILI9341_PIN_CS, ILI9341_CS_DISABLE);

    //
    // At the End, set D7 - D0 direction to GPIO OutPut
    //
    xGPIOSPinTypeGPIOOutput(ILI9341_PIN_D7);
    xGPIOSPinTypeGPIOOutput(ILI9341_PIN_D6);
    xGPIOSPinTypeGPIOOutput(ILI9341_PIN_D5);
    xGPIOSPinTypeGPIOOutput(ILI9341_PIN_D4);
    xGPIOSPinTypeGPIOOutput(ILI9341_PIN_D3);
    xGPIOSPinTypeGPIOOutput(ILI9341_PIN_D2);
    xGPIOSPinTypeGPIOOutput(ILI9341_PIN_D1);
    xGPIOSPinTypeGPIOOutput(ILI9341_PIN_D0);

    return ulData;
}*/

void ILI9341WriteCmd(uint8_t ucCmd)
{
    ILI9341Write(ILI9341_RS_COMMAND, ucCmd);
}

void ILI9341WriteData(uint16_t ulData)
{
    ILI9341Write(ILI9341_RS_DATA, ulData);
}

//*****************************************************************************
//
//! \brief Set the cursor location.
//!
//! \param ulStartX is the character index.
//! \param ulEndX is the line number.
//!
//! The default location is (0,0). After set the location, the strings will
//! display at the (ulCol, ulRow).
//!
//! \return None.
//
//*****************************************************************************
void ILI9341SetCurPos(uint32_t ulStartX, uint32_t ulEndX,
                  uint32_t ulStartY, uint32_t ulEndY)
{
    //
    // Check Arguments.
    //

    //xASSERT((ulStartX < ulEndX) && (ulStartY < ulEndY));
    //xASSERT((ulEndX < LCD_HORIZONTAL_MAX) && (ulEndY < LCD_VERTICAL_MAX));

    //
    // Tell the controller we are about to write data into its RAM.
    //
    ILI9341WriteCmd(ILI9341_COLADDRSET_REG);
    ILI9341WriteData(ulStartX);
    ILI9341WriteData(ulEndX);
    ILI9341WriteCmd(ILI9341_PAGEADDRSET_REG);
    ILI9341WriteData(ulStartY);
    ILI9341WriteData(ulEndY);
    ILI9341WriteCmd(ILI9341_MEMORYWRITE_REG);
}

//*****************************************************************************
//
//! \brief Init the ILI9341 LCD Device.
//!
//! \param None.
//!
//! This function is used to Init the ILI9341 Device. It must be call before
//! any other LCD function use.
//!
//! It Open the pins's GPIO peripheral port, and config the pins type to GPIO
//! output. Then config the LCD into the default state, clear the LCD and
//! Open the display. Default A Blink cursor is set on. The LCD cursor move
//! direction is config as increment default.
//!
//! The ILI9341 Initial state can be determined by the \ref ILI9341_Config.
//! - Pins that used is determined by \ref ILI9341_Config_Pins.
//! - The LCD display line is determined by \ref ILI9341_DISPLAY_LINE.
//! - The LCD character font is determined by \ref ILI9341_CHARACTER_FONT.
//! .
//!
//! \return None.
//
//*****************************************************************************
void ILI9341Init(void)
{
    uint32_t ulCount;

    //WARNING: Enable RCC for port in main

    //
    // Set Pins Type to GPIO Output
    //

    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D7, GPIO_Speed_50MHz);
    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D6, GPIO_Speed_50MHz);
    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D5, GPIO_Speed_50MHz);
    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D4, GPIO_Speed_50MHz);
    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D3, GPIO_Speed_50MHz);
    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D2, GPIO_Speed_50MHz);
    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D1, GPIO_Speed_50MHz);
    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D0, GPIO_Speed_50MHz);

    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_RD, GPIO_Speed_50MHz);
    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_WR, GPIO_Speed_50MHz);
    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_CS, GPIO_Speed_50MHz);
    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_RS, GPIO_Speed_50MHz);
    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_RST, GPIO_Speed_50MHz);

    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_RST, Bit_SET);
    Delay(100);
    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_RST, Bit_RESET);
    Delay(100);
    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_RST, Bit_SET);
    Delay(100);

    ILI9341WriteCmd(ILI9341_POWERCTLB_REG);      // 0xCF
    ILI9341WriteData(0x81);
    ILI9341WriteData(0x30);

    ILI9341WriteCmd(ILI9341_POWONSEQCTL_REG);    // 0xED
    ILI9341WriteData(0x6403);
    ILI9341WriteData(0x1281);

    ILI9341WriteCmd(ILI9341_DIVTIMCTL_A_REG);    // 0xE8
    ILI9341WriteData(0x8500);
    ILI9341WriteData(0x79);

    ILI9341WriteCmd(ILI9341_POWERCTLA_REG);      // 0xC8
    ILI9341WriteData(0x392c);
    ILI9341WriteData(0x34);
    ILI9341WriteData(0x02);

    ILI9341WriteCmd(ILI9341_PUMPRATIOCTL_REG);   // 0xF7
    ILI9341Write8Bit(0x20);

    ILI9341WriteCmd(ILI9341_DIVTIMCTL_B_REG);    // 0xEA set VCL,VGH,VGL,AVDD
    ILI9341WriteData(0x00);

    ILI9341WriteCmd(ILI9341_FRAMECTL_NOR_REG);   // 0xb1 set VCL,VGH,VGL,AVDD
    ILI9341WriteData(0x1b);

    ILI9341WriteCmd(ILI9341_POWERCTL1_REG);      // 0xC0
    ILI9341Write8Bit(0x2e);//26

    ILI9341WriteCmd(ILI9341_POWERCTL2_REG);      // 0xC1
    ILI9341Write8Bit(0x12);

    ILI9341WriteCmd(ILI9341_VCOMCTL1_REG);       // 0xC5
    ILI9341WriteData(0x5019); //2b

    ILI9341WriteCmd(ILI9341_VCOMCTL2_REG);       // 0xC7
    ILI9341Write8Bit(0x90); //c4

    ILI9341WriteCmd(ILI9341_MEMACCESS_REG);      // 0x36
    ILI9341Write8Bit(0xa8);

    ILI9341WriteCmd(ILI9341_FUNCTONCTL_REG);     // 0xB6
    ILI9341WriteData(0x0AA2);

    ILI9341WriteCmd(ILI9341_ENABLE_3G_REG);      // 0xF2
    ILI9341Write8Bit(0x00);
    ILI9341WriteCmd(ILI9341_GAMMASET_REG);       // 0x26
    ILI9341Write8Bit(0x01);

    ILI9341WriteCmd(ILI9341_POSGAMMACORRECTION_REG);  // 0xE0
    ILI9341WriteData(0x0F27);
    ILI9341WriteData(0x230B);
    ILI9341WriteData(0x0F05);
    ILI9341WriteData(0x5474);
    ILI9341WriteData(0x450A);
    ILI9341WriteData(0x170A);
    ILI9341WriteData(0x1C0E);
    ILI9341WriteData(0x08);//00

    //===== gamma"-"polarity correction characteristic setting ===================
    ILI9341WriteCmd(ILI9341_NEGGAMMACORRECTION_REG);   //0xE1
    ILI9341WriteData(0x081A);
    ILI9341WriteData(0x1E03);
    ILI9341WriteData(0x0F05);
    ILI9341WriteData(0x2E25);
    ILI9341WriteData(0x3B01);
    ILI9341WriteData(0x0605);
    ILI9341WriteData(0x2533);
    ILI9341WriteData(0x0F);

    ILI9341WriteCmd(ILI9341_PIXFORMATSET_REG);   // 0x3A
    ILI9341Write8Bit(0x55);                      // 16-bit
    ILI9341WriteCmd(ILI9341_MEMACCESS_REG);      // 0x36
    ILI9341Write8Bit(0x00);
    ILI9341WriteCmd(ILI9341_INTERFCTL_REG);      // 0xF6
    ILI9341WriteData(0x130);

    ILI9341WriteCmd(ILI9341_DISPLAYON_REG);      // 0x29 Exit Sleep
    ILI9341WriteCmd(ILI9341_SLEEP_OUT_REG);	     // 0x11
    Delay(500);

    //
    // Set the display size and ensure that the GRAM window is set to allow
    // access to the full display buffer.
    //
    ILI9341WriteCmd(ILI9341_COLADDRSET_REG);     // 0x2A
    ILI9341WriteData(0);
    ILI9341WriteData(LCD_HORIZONTAL_MAX - 1);
    ILI9341WriteCmd(ILI9341_PAGEADDRSET_REG);    // 0x2B
    ILI9341WriteData(0);
    ILI9341WriteData(LCD_VERTICAL_MAX - 1);

    //
    // Clear the contents of the display buffer.
    //
    ILI9341WriteCmd(ILI9341_MEMORYWRITE_REG);    // 0x2C
    for(ulCount = 0; ulCount < (LCD_HORIZONTAL_MAX * LCD_VERTICAL_MAX); ulCount++)
    {
        ILI9341WriteData(0xFFFF);
    }
}

//*****************************************************************************
//
//! \brief Enter the the LCD sleep mode.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341EnterSleepMode(void)
{
    //
    // Sleep set
    //
    ILI9341WriteCmd(ILI9341_SLEEP_ENTER_REG);
}

//*****************************************************************************
//
//! \brief Exit the the LCD sleep mode.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341ExitSleepMode(void)
{
    ILI9341WriteCmd(ILI9341_SLEEP_OUT_REG);
    Delay(500);
}

//*****************************************************************************
//
//! \brief Fills a rectangle (direct LCD access mode).
//!
//! \param ulStartX is the starting address of X.
//! \param ulStartY is the starting address of Y.
//! \param ulEndX is the ending address of X.
//! \param ulEndY is the ending address of Y.
//! \param ulColor is the color.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both ulStartX and
//! ulEndX are drawn, along with ulStartY and ulEndY).
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayRectFill(uint16_t usStartX, uint16_t usEndX,
                        uint16_t usStartY, uint16_t usEndY, uint32_t ulColor)
{
    uint32_t ulTemp;

    //
    // Check Arguments.
    //
    /*xASSERT((usStartX <= ulEndX)                &&
            (usStartY <= ulEndY)                &&
            ((usStartX >= 0) || (usEndX < LCD_HORIZONTAL_MAX)) &&
            ((usStartY >= 0) || (usEndY < LCD_VERTICAL_MAX)));*/

    //TODO: Assert

    //
    // Clear LCD
    //
    ILI9341SetCurPos(usStartX, usEndX, usStartY, usEndY);

    for(ulTemp = 0; ulTemp <= (usEndX - usStartX + 1) * (usEndY - usStartY + 1); ulTemp++)
    {
        ILI9341WriteData(ulColor);
    }
}

//*****************************************************************************
//
//! \brief Clears all the LCD display with the specified colour.
//!
//! \param ulColor is the specified colour.
//!
//! It clears all the LCD display with the specified colour.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayClear(uint32_t ulColor)
{
    ILI9341DisplayRectFill(0, LCD_HORIZONTAL_MAX - 1, 0, LCD_VERTICAL_MAX - 1, ulColor);
}

//*****************************************************************************
//
//! \brief Enter into display inversion mode.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void  ILI9341DisplayInversionOn(void)
{
    ILI9341WriteCmd(ILI9341_INVERSIONON_REG);
}

//*****************************************************************************
//
//! \brief Recover from display inversion mode.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayInversionOff(void)
{
    ILI9341WriteCmd(ILI9341_INVERSIONOFF_REG);
}

//*****************************************************************************
//
//! \brief Enter into display inversion mode.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayOn(void)
{
    ILI9341WriteCmd(ILI9341_DISPLAYON_REG);
}

//*****************************************************************************
//
//! \brief Recover from display inversion mode.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayOff(void)
{
    ILI9341WriteCmd(ILI9341_DISPLAYOFF_REG);
}

//*****************************************************************************
//
//! \brief Draws a pixel on the screen.
//!
//! \param ulX is the X coordinate of the pixel.
//! \param ulY is the Y coordinate of the pixel.
//! \param ulValue is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DrawOnePixel(uint16_t usX, uint16_t usY, uint32_t ulColor)
{
    //
    // Write the pixel value.
    //
    ILI9341SetCurPos(usX, usX, usY, usY);
    ILI9341WriteData(ulColor);
}

//*****************************************************************************
//
//! \brief Draws a horizontal line.
//!
//! \param usStartX is the X coordinate of the start of the line.
//! \param usEndX is the X coordinate of the end of the line.
//! \param usY is the Y coordinate of the line.
//! \param usColor is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DrawHorizontalLine(uint16_t usStartX, uint16_t usEndX,
                          uint16_t usY, uint32_t ulColor)
{
    //xASSERT((usStartX <= usEndX) && (usEndX <= LCD_HORIZONTAL_MAX) && (usStartX >= 0));
    //TODO: Assert

    while(usStartX++ <= usEndX)
    {
        ILI9341DrawOnePixel(usStartX, usY, ulColor);
    }
}

//*****************************************************************************
//
//! \brief Draws a vertical line.
//!
//! \param ulX is the X coordinate of the line.
//! \param ulStartY is the Y coordinate of the start of the line.
//! \param ulEndY is the Y coordinate of the end of the line.
//! \param ulColor is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DrawVerticalLine(uint16_t usX, uint16_t usStartY,
                        uint16_t usEndY, uint32_t ulColor)
{
    //xASSERT((usStartY <= usEndY) && (usEndY <= LCD_VERTICAL_MAX) && (usStartY >= 0));
    //TODO: Assert

    while(usStartY++ <= usEndY)
    {
        ILI9341DrawOnePixel(usX, usStartY, ulColor);
    }
}

//*****************************************************************************
//
//! \brief Display a char on the LCD.
//!
//! \param pcString is a non-terminated string that to display.
//!
//! Display a string on the cursor location.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341PutChar8x16(uint16_t usX, uint16_t usY, char c,
                         uint32_t fColor, uint32_t bColor)
{
    unsigned int i, j;

    //ASSERT((usX <= LCD_HORIZONTAL_MAX) && (usY <= LCD_VERTICAL_MAX));
    //TODO: Assert

    ILI9341SetCurPos(usX, usX + 8 - 1, usY, usY + 16 - 1);

    for(i = 0; i < 16; i++)
    {
        uint8_t m = Font8x16[(c - 0x20) * 16 + i];
        for(j = 0; j < 8; j++)
        {
            if((m & 0x80) == 0x80)
            {
                ILI9341WriteData(fColor);
            }
            else
            {
                ILI9341WriteData(bColor);
            }
            m <<= 1;
        }
    }
}

//*****************************************************************************
//
//! \brief Display a string on the LCD.
//!
//! \param pcString is a non-terminated string that to display.
//!
//! Display a string on the cursor location.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayString(uint16_t usX, uint16_t usY, const char* pcString,
                     uint32_t fColor, uint32_t bColor)
{
    uint8_t ucl = 0;

    //xASSERT(pcString != 0);
    //TODO: Assert

    while(*pcString)
    {			
				if((*pcString)==0X0A || (*pcString)==0X0D)
				{
					pcString++;
					continue;
				}
        if( *pcString < 0x80)
        {
            ILI9341PutChar8x16(usX + ucl * 8, usY, *pcString, fColor, bColor);
            pcString++; ucl++;
        }
    }
}
//#endif

//*****************************************************************************
//
//! \brief Display a decimal number on the LCD.
//!
//! \param n is the number that to display.
//!
//! Display a decimal number on the cursor location.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayNum(uint16_t usX, uint16_t usY, uint32_t ulNum,
                   uint32_t fColor, uint32_t bColor)
{
    uint8_t ucFigures = 0;
    uint32_t ulTemp = ulNum;

    if(ulNum)
    {
        while (ulTemp)
        {
            ulTemp /= 10;
            ucFigures++;
        }

        while (ulNum)
        {
            ILI9341PutChar8x16(usX + (ucFigures--) * 8 - 8, usY, ulNum%10 + '0', fColor, bColor);
            ulNum /= 10;
        }
    }
    else
    {
        ILI9341PutChar8x16(usX, usY, '0', fColor, bColor);
    }
}

//*****************************************************************************
//
//! \brief Display a bmp picture on the LCD.
//!
//! \param n is the number that to display.
//!
//! Display a decimal number on the cursor location.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayBmp(uint16_t usX, uint16_t usY, uint16_t usSizeX,
                   uint16_t usSizeY, uint8_t const *Bmp)
{
    uint16_t i,j;
    uint32_t ulBmpData;

    //xASSERT((usX < LCD_HORIZONTAL_MAX) && (usY < LCD_VERTICAL_MAX));
    //TODO: Assert

    ILI9341SetCurPos(usX, usX + usSizeX, usY, usY + usSizeY);

    for( i = usY; i <= usY + usSizeY; i++ )
    {
        for( j = usX ; j <= usX + usSizeX; j++)
        {
            ulBmpData = *Bmp++;
            ulBmpData |= (*Bmp++) << 8;
            ILI9341WriteData(ulBmpData);
        }
    }
}

//*****************************************************************************

void DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t bordercolor)
{
	int16_t x = r;
	int16_t y = 0;
	int16_t err = 0;

	while (x >= y)
	{
		ILI9341DrawOnePixel(x0 + x, y0 + y, bordercolor);
		ILI9341DrawOnePixel(x0 + y, y0 + x, bordercolor);
		ILI9341DrawOnePixel(x0 - y, y0 + x, bordercolor);
		ILI9341DrawOnePixel(x0 - x, y0 + y, bordercolor);
		ILI9341DrawOnePixel(x0 - x, y0 - y, bordercolor);
		ILI9341DrawOnePixel(x0 - y, y0 - x, bordercolor);
		ILI9341DrawOnePixel(x0 + y, y0 - x, bordercolor);
		ILI9341DrawOnePixel(x0 + x, y0 - y, bordercolor);

		if (err <= 0)
		{
			y += 1;
			err += 2*y + 1;
		}
		if (err > 0)
		{
			x -= 1;
			err -= 2*x + 1;
		}
	}
}



/******************************************************************************
 * 
 * B(5) G(6) R(5)
 * bbbbb gggggg rrrrr
 *
 ******************************************************************************/
uint16_t RGB(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t color = r >> 3;
	color|= (g >> 2) << 5;
	color|= (b >> 3) << 11;
	return color;
}
