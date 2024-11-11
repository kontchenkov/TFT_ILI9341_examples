#ifndef __ILI9341_H__
#define __ILI9341_H__

#include "gpio.h"
#include "delay.h"
#include "8X16.h"

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
/*******************************************/

/*******************************************/
#define LCD_VERTICAL_MAX        320
#define LCD_HORIZONTAL_MAX      240

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
/*******************************************/

void ILI9341Write(uint8_t ucRS, uint16_t ulInstruction);
void ILI9341Write8Bit(uint8_t ucData);

extern void ILI9341WriteCmd(uint8_t ucCmd);
extern void ILI9341WriteData(uint16_t ucData);
extern void ILI9341Init(void);

void ILI9341SetCurPos(uint32_t ulStartX, uint32_t ulEndX,
                  uint32_t ulStartY, uint32_t ulEndY);

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
//*****************************************************************************

// Самостоятельно разработанные функции
extern uint16_t RGB(uint8_t r, uint8_t g, uint8_t b);
extern void DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint32_t bordercolor);

#endif //__ILI9341_H__
