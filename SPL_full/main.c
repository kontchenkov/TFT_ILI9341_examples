#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>

#include "ili9341.h"
#include "delay.h"
#include "gpio.h"
#include "usart.h"
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
	
	USART1_init();

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
	
	//GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_RESET);
	GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_SET);
	Delay(1000);


	ILI9341Init();
	//ILI9341DisplayOff();
	//Delay(1000);
	//ILI9341DisplayOn();
	//Delay(1000);

	for(uint16_t x = 0; x<240; x++)
	{
		for(uint16_t y = 0; y<320; y++)
		{
			ILI9341DrawOnePixel(x, y, RGB(250,250,250));
		}
	}
	ILI9341DisplayRectFill(0, 240, 0, 320, RGB(250,250,250));
	//ILI9341DisplayOn();
	//char buffer[256] = "Feodor";
	//ILI9341DisplayString(10, 50, buffer ,RGB(255,0,0),0xffff);
	DrawCircle(120, 160, 50, RGB(255,0,0));
  
	current=0;
	ItemsCount=0;
  PrevSelected =0;
  checked=0;

//	PrevSelected=1;
	AddMenuItem("Pink",RGB(230,25,97));
	AddMenuItem("Green",RGB(0,255,0));
	AddMenuItem("Blue",RGB(0,0,255));
	UpdateGUI();
	SelectRootItem();
	ILI9341DisplayNum(120, 220,1, RGB(255,0,0), RGB(255,255,255));
	ILI9341DisplayRectFill(0, 100, 230, 320, RGB(0,0,255));
	//SelectedItemChanged();
	//Delay(1000);
	// проверка режима сна
	//ILI9341EnterSleepMode();
	//Delay(1000);
	//ILI9341ExitSleepMode();
	//Delay(1000);
	
/*//	AddMenuItem("Green",RGB(0,255,0));
//	AddMenuItem("Blue",RGB(0,0,255));
//	UpdateGUI();
//	current=0;
//	PrevSelected=1;
//	SelectedItemChanged();*/
	//unsigned short x=0, y=0;
	//unsigned short y;
	//USART1_state = SENDED;
    while(1)
    {
			UpdateKeyState();
			Delay(100);
			/*if (USART1_state == IDLE)
			{
				//рисует картинки
				uint16_t string[240];
				for(unsigned short i=0; i<240; i++)
				{
					uint16_t temp=0;
					temp=(uint16_t)rxBuffer[i*2];
					temp<<=8;
					temp |= (uint16_t)rxBuffer[i*2+1];
					string[i] = temp;
					ILI9341DrawOnePixel(i, y, string[i]);
				}	*/			
				
				//пишет текст
				/*int len=strlen(rxBuffer);
				rxBuffer[len-2]=0x0A;
				
				if (strstr(rxBuffer, "\n") != NULL)
				{
					strcpy(buffer,rxBuffer );
					ILI9341DisplayString(x*8, y*16, buffer ,RGB(255,0,0),0xffff);
					x+=len-2;
					if(x>=30)
					{
						x=0;
						y++;
					}
					
					USART1_state = SENDED;
					continue;
				}*/
				
				/*USART1_state = SENDED;
				y++;
			}
			
    	//UpdateKeyState();
    }*/
}
		}







/*
 	for(uint16_t x = 0; x<256; x++)
    {
    	for(uint16_t y = 0; y<256; y++)
    	{
    		ILI9341DrawOnePixel(x, y, RGB(x,0,y));
    	}
    }

    DrawCircle(120, 80 + 8, 50, RGB(0,0,255));
	ILI9341DisplayString(107, 80, "MASS",RGB(0,0,255),0xffff);
	DrawCircle(120, 160, 50, RGB(0,255,0));
	ILI9341DisplayString(101, 160, "EFFECT",RGB(0,255,0),0xffff);
	DrawCircle(120, 240, 50, RGB(255,0,0));
	ILI9341DisplayString(96, 240, "ENDINGS",RGB(255,0,0),0xffff);
 */
