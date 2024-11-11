#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>

#include "ili9341.h"
#include "delay.h"
#include "gpio.h"
#include "GUI_Lib.h"

#include <string.h>
#include <stdlib.h>

void InitRCC()
{
	RCC_DeInit();
	RCC_HSICmd(DISABLE);
	RCC_HSEConfig(RCC_HSE_ON); 
	ErrorStatus HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS)
	{
		/*
		 * HSE -> PREDDIV2(/5) -> PLL2MUL(*8) -> PREDDIV1(/5) -> PLL(*9) -> SYSCLK
		 * HSE / 5 * 8 / 5 * 9 = HSE * 2.88
		 *
		 * HSE = 25 MHz, SYSCLK = 72 MHz
		 */

		RCC_PREDIV2Config(RCC_PREDIV2_Div5);
		RCC_PLL2Config(RCC_PLL2Mul_8);
		RCC_PLL2Cmd(ENABLE);
		RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
		RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		/* HCLK = SYSCLK = 72 MHz */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		/* PCLK1 = HCLK/2 = 36 MHz */
		RCC_PCLK1Config(RCC_HCLK_Div2);

		/* PCLK2 = HCLK = 72 MHz */
		RCC_PCLK2Config(RCC_HCLK_Div1);		

		while (RCC_GetSYSCLKSource() != 0x08){}
	}
	else
	{   
		while (1){}
	}
}

int main(void)
{
	InitRCC();

	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_GPIOE |
			RCC_APB2Periph_GPIOD |
			RCC_APB2Periph_GPIOC |
			RCC_APB2Periph_GPIOB |
			RCC_APB2Periph_GPIOA,
	ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  // Для формирования временных задержек используется таймер TIM6,
	// подключенный к шине APB1, поэтому тактирование этого
	// таймера определяется частотой PCLK2_Frequency;
	// при текущих настройках, определяемых в функции InitRCC(),
	// PCLK2_Frequency = 72 000 000 (72 МГц);
	// создаем и инициализируем структуру, содержащую занчения параметров
	// SYSCLK_Frequency, HCLK_Frequency, PCLK1_Frequency, PCLK2_Frequency, ADCCLK_Frequency,
	// получаем значения этих параметров и инициализируем таймер значением PCLK2_Frequency;
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);
	InitTimer(clocks.PCLK2_Frequency);

	//Gpio
	InitGPIO(GPIOD, GPIO_Mode_Out_PP, GPIO_Pin_0, GPIO_Speed_2MHz);


	InitGPIO(GPIOA, GPIO_Mode_IPU,GPIO_Pin_6,GPIO_Speed_50MHz);
	InitGPIO(GPIOB, GPIO_Mode_IPU,GPIO_Pin_5,GPIO_Speed_50MHz);
	InitGPIO(GPIOD, GPIO_Mode_IPU,GPIO_Pin_2 | GPIO_Pin_4,GPIO_Speed_50MHz);
	InitGPIO(GPIOC, GPIO_Mode_IPU,GPIO_Pin_13,GPIO_Speed_50MHz);
	
	
	ILI9341Init();
	

	for(uint16_t x = 0; x<240; x++)
	{
		for(uint16_t y = 0; y<320; y++)
		{
			ILI9341DrawOnePixel(x, y, RGB(250,250,250));
		}
	}
	ILI9341DisplayRectFill(0, 240, 0, 320, RGB(250,250,250));
	DrawCircle(120, 160, 50, RGB(255,0,0));
  
	current=0;
	ItemsCount=0;
  PrevSelected =0;
  checked=0;

	AddMenuItem("Pink",RGB(230,25,97));
	AddMenuItem("Green",RGB(0,255,0));
	AddMenuItem("Blue",RGB(0,0,255));
	UpdateGUI();
	SelectRootItem();
	ILI9341DisplayNum(120, 220,1, RGB(255,0,0), RGB(255,255,255));
	ILI9341DisplayRectFill(0, 100, 230, 320, RGB(0,0,255));
	
    while(1)
    {
			UpdateKeyState();
			Delay(100);
		}
			
}
