#include "hw_ili9341.h"
#include "ili9341.h"

void ILI9341Write(uint8_t ucRS, uint16_t ulInstruction)
{
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

void ILI9341WriteCmd(uint8_t ucCmd)
{
    ILI9341Write(ILI9341_RS_COMMAND, ucCmd);
}

void ILI9341WriteData(uint16_t ulData)
{
    ILI9341Write(ILI9341_RS_DATA, ulData);
}

//void ILI9341Init(void)
//{
//    uint32_t ulCount;

//    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D7, GPIO_Speed_50MHz);
//    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D6, GPIO_Speed_50MHz);
//    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D5, GPIO_Speed_50MHz);
//    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D4, GPIO_Speed_50MHz);
//    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D3, GPIO_Speed_50MHz);
//    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D2, GPIO_Speed_50MHz);
//    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D1, GPIO_Speed_50MHz);
//    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_D0, GPIO_Speed_50MHz);

//    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_RD, GPIO_Speed_50MHz);
//    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_WR, GPIO_Speed_50MHz);
//    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_CS, GPIO_Speed_50MHz);
//    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_RS, GPIO_Speed_50MHz);
//    InitGPIO(LCD_GPIO_DEF, GPIO_Mode_Out_PP, ILI9341_PIN_RST, GPIO_Speed_50MHz);

//    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_RST, Bit_SET);
//    Delay(100);
//    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_RST, Bit_RESET);
//    Delay(100);
//    GPIO_WriteBit(LCD_GPIO_DEF, ILI9341_PIN_RST, Bit_SET);
//    Delay(100);

//    ILI9341WriteCmd(ILI9341_POWERCTLB_REG);      // 0xCF
//    ILI9341WriteData(0x81);
//    ILI9341WriteData(0x30);

//    ILI9341WriteCmd(ILI9341_POWONSEQCTL_REG);    // 0xED
//    ILI9341WriteData(0x6403);
//    ILI9341WriteData(0x1281);

//    ILI9341WriteCmd(ILI9341_DIVTIMCTL_A_REG);    // 0xE8
//    ILI9341WriteData(0x8500);
//    ILI9341WriteData(0x79);

//    ILI9341WriteCmd(ILI9341_POWERCTLA_REG);      // 0xC8
//    ILI9341WriteData(0x392c);
//    ILI9341WriteData(0x34);
//    ILI9341WriteData(0x02);

//    ILI9341WriteCmd(ILI9341_PUMPRATIOCTL_REG);   // 0xF7
//    ILI9341Write8Bit(0x20);

//    ILI9341WriteCmd(ILI9341_DIVTIMCTL_B_REG);    // 0xEA set VCL,VGH,VGL,AVDD
//    ILI9341WriteData(0x00);

//    ILI9341WriteCmd(ILI9341_FRAMECTL_NOR_REG);   // 0xb1 set VCL,VGH,VGL,AVDD
//    ILI9341WriteData(0x1b);

//    ILI9341WriteCmd(ILI9341_POWERCTL1_REG);      // 0xC0
//    ILI9341Write8Bit(0x2e);//26

//    ILI9341WriteCmd(ILI9341_POWERCTL2_REG);      // 0xC1
//    ILI9341Write8Bit(0x12);

//    ILI9341WriteCmd(ILI9341_VCOMCTL1_REG);       // 0xC5
//    ILI9341WriteData(0x5019); //2b

//    ILI9341WriteCmd(ILI9341_VCOMCTL2_REG);       // 0xC7
//    ILI9341Write8Bit(0x90); //c4

//    ILI9341WriteCmd(ILI9341_MEMACCESS_REG);      // 0x36
//    ILI9341Write8Bit(0xa8);

//    ILI9341WriteCmd(ILI9341_FUNCTONCTL_REG);     // 0xB6
//    ILI9341WriteData(0x0AA2);

//    ILI9341WriteCmd(ILI9341_ENABLE_3G_REG);      // 0xF2
//    ILI9341Write8Bit(0x00);
//    ILI9341WriteCmd(ILI9341_GAMMASET_REG);       // 0x26
//    ILI9341Write8Bit(0x01);

//    ILI9341WriteCmd(ILI9341_POSGAMMACORRECTION_REG);  // 0xE0
//    ILI9341WriteData(0x0F27);
//    ILI9341WriteData(0x230B);
//    ILI9341WriteData(0x0F05);
//    ILI9341WriteData(0x5474);
//    ILI9341WriteData(0x450A);
//    ILI9341WriteData(0x170A);
//    ILI9341WriteData(0x1C0E);
//    ILI9341WriteData(0x08);//00

//    //===== gamma"-"polarity correction characteristic setting ===================
//    ILI9341WriteCmd(ILI9341_NEGGAMMACORRECTION_REG);   //0xE1
//    ILI9341WriteData(0x081A);
//    ILI9341WriteData(0x1E03);
//    ILI9341WriteData(0x0F05);
//    ILI9341WriteData(0x2E25);
//    ILI9341WriteData(0x3B01);
//    ILI9341WriteData(0x0605);
//    ILI9341WriteData(0x2533);
//    ILI9341WriteData(0x0F);

//    ILI9341WriteCmd(ILI9341_PIXFORMATSET_REG);   // 0x3A
//    ILI9341Write8Bit(0x55);                      // 16-bit
//    ILI9341WriteCmd(ILI9341_MEMACCESS_REG);      // 0x36
//    ILI9341Write8Bit(0x00);
//    ILI9341WriteCmd(ILI9341_INTERFCTL_REG);      // 0xF6
//    ILI9341WriteData(0x130);

//    ILI9341WriteCmd(ILI9341_DISPLAYON_REG);      // 0x29 Exit Sleep
//    ILI9341WriteCmd(ILI9341_SLEEP_OUT_REG);	     // 0x11
//    Delay(500);

//    //
//    // Set the display size and ensure that the GRAM window is set to allow
//    // access to the full display buffer.
//    //
//    ILI9341WriteCmd(ILI9341_COLADDRSET_REG);     // 0x2A
//    ILI9341WriteData(0);
//    ILI9341WriteData(LCD_HORIZONTAL_MAX - 1);
//    ILI9341WriteCmd(ILI9341_PAGEADDRSET_REG);    // 0x2B
//    ILI9341WriteData(0);
//    ILI9341WriteData(LCD_VERTICAL_MAX - 1);

//    //
//    // Clear the contents of the display buffer.
//    //
//    ILI9341WriteCmd(ILI9341_MEMORYWRITE_REG);    // 0x2C
//    for(ulCount = 0; ulCount < (LCD_HORIZONTAL_MAX * LCD_VERTICAL_MAX); ulCount++)
//    {
//        ILI9341WriteData(0xFFFF);
//    }
//}

void ILI9341Init(void){
    uint32_t ulCount;

    // Set Pins Type to GPIO Output
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

    //=== gamma"-"polarity correction characteristic setting ===
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

    // Set the display size and ensure that the GRAM window is set to allow
    // access to the full display buffer.
    ILI9341WriteCmd(ILI9341_COLADDRSET_REG);     // 0x2A
    ILI9341WriteData(0);
    ILI9341WriteData(LCD_HORIZONTAL_MAX - 1);
    ILI9341WriteCmd(ILI9341_PAGEADDRSET_REG);    // 0x2B
    ILI9341WriteData(0);
    ILI9341WriteData(LCD_VERTICAL_MAX - 1);

    // Clear the contents of the display buffer.
    ILI9341WriteCmd(ILI9341_MEMORYWRITE_REG);    // 0x2C
    for (ulCount = 0; ulCount < (LCD_HORIZONTAL_MAX * LCD_VERTICAL_MAX); 
         ulCount++)
        ILI9341WriteData(0xFFFF);
}


void ILI9341SetCurPos(uint32_t ulStartX, uint32_t ulEndX,
                  uint32_t ulStartY, uint32_t ulEndY)
{
    ILI9341WriteCmd(ILI9341_COLADDRSET_REG);
    ILI9341WriteData(ulStartX);
    ILI9341WriteData(ulEndX);
    ILI9341WriteCmd(ILI9341_PAGEADDRSET_REG);
    ILI9341WriteData(ulStartY);
    ILI9341WriteData(ulEndY);
    ILI9341WriteCmd(ILI9341_MEMORYWRITE_REG);
}

void ILI9341DisplayRectFill(uint16_t usStartX, uint16_t usEndX,
                        uint16_t usStartY, uint16_t usEndY, uint32_t ulColor)
{
    uint32_t ulTemp;

    ILI9341SetCurPos(usStartX, usEndX, usStartY, usEndY);

    for(ulTemp = 0; ulTemp <= (usEndX - usStartX + 1) * (usEndY - usStartY + 1); ulTemp++)
    {
        ILI9341WriteData(ulColor);
    }
}

void ILI9341DrawOnePixel(uint16_t usX, uint16_t usY, uint32_t ulColor)
{
    //
    // Write the pixel value.
    //
    ILI9341SetCurPos(usX, usX, usY, usY);
    ILI9341WriteData(ulColor);
}

void ILI9341DrawHorizontalLine(uint16_t usStartX, uint16_t usEndX,
                          uint16_t usY, uint32_t ulColor)
{
    while(usStartX++ <= usEndX)
    {
        ILI9341DrawOnePixel(usStartX, usY, ulColor);
    }
}

void ILI9341DrawVerticalLine(uint16_t usX, uint16_t usStartY,
                        uint16_t usEndY, uint32_t ulColor)
{
    while(usStartY++ <= usEndY)
    {
        ILI9341DrawOnePixel(usX, usStartY, ulColor);
    }
}

void ILI9341PutChar8x16(uint16_t usX, uint16_t usY, char c,
                         uint32_t fColor, uint32_t bColor)
{
    unsigned int i, j;
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

void ILI9341DisplayString(uint16_t usX, uint16_t usY, const char* pcString,
                     uint32_t fColor, uint32_t bColor)
{
    uint8_t ucl = 0;
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
 * Формат вывода цвета
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
