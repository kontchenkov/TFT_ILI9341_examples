#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>

#include "ili9341.h"
#include "delay.h"
#include "gpio.h"
#include "usart.h"

#include <string.h>
#include <stdlib.h>

#define UNSELECTED_ITEM RGB(76,255,255)
#define SELECTED_COLOR RGB(15,139,162)

uint8_t checked=0;
int8_t current=0;


typedef struct
{
	uint16_t Height;
	uint16_t Widht;
	uint16_t X;
	uint16_t Y;
} RECT;

typedef struct
{
	char* Title;
	uint16_t Color;
	uint16_t BGColor;
	void * Next;
	void * Prev;
	RECT Rect;
}MENU_ITEM;

typedef enum
{
	NOPE,
	UP ,
	DOWN,
	LEFT,
	RIGHT,
	ALL
}  BUTTON;


void UpdateGUI();
void AddMenuItem(char* Title,uint16_t Color);
void DrawMenuItem(MENU_ITEM* Item);
void SelectedItemChanged();
void UpdateKeyState();
void CenterButtonClicked();


void InitRCC();
MENU_ITEM* MenuRoot;
MENU_ITEM* MenuLast;
MENU_ITEM* SelectedItem;
MENU_ITEM List[5];
uint16_t ItemsCount=0;
uint16_t PrevSelected;


uint8_t ReadKey()
{
	uint8_t result;

	uint8_t up = !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
	uint8_t down = !GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
	uint8_t right = !GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4);
	uint8_t left =  !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);


	if((up || down || left || right) && !checked)
	{
		checked=1;
		if(up&&down&&left&&right) return ALL;
		if(up) return UP;
		if(down) return DOWN;
		if(left)return LEFT;
		if(right)return RIGHT;
	}
	else if(!(up || down || left || right) && checked)
	{
		checked = 0;
	}

	return NOPE;
}

int main(void)
{
	uint8_t button;
	int i;

	InitRCC();
	
	USART1_init();

	//Включение тактирования
	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_GPIOE |
			RCC_APB2Periph_GPIOD |
			RCC_APB2Periph_GPIOC |
			RCC_APB2Periph_GPIOB |
			RCC_APB2Periph_GPIOA,
	ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);


	//Настройка задержки
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);
	InitTimer(clocks.SYSCLK_Frequency);

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
			ILI9341DrawOnePixel(x, y, RGB(75,153,153));
		}
	}

//	AddMenuItem("Red",RGB(255,0,0));
//	AddMenuItem("Green",RGB(0,255,0));
//	AddMenuItem("Blue",RGB(0,0,255));
//	UpdateGUI();
//	current=0;
//	PrevSelected=1;
//	SelectedItemChanged();
	unsigned short x=0, y=0;
	char buffer[256];
	USART1_state = SENDED;
    while(1)
    {
			if (USART1_state == IDLE)
			{
				//СЂРёСЃСѓРµС‚ РєР°СЂС‚РёРЅРєРё
//				uint16_t string[240];
//				for(unsigned short i=0; i<240; i++)
//				{
//					uint16_t temp=0;
//					temp=(uint16_t)rxBuffer[i*2];
//					temp<<=8;
//					temp |= (uint16_t)rxBuffer[i*2+1];
//					string[i] = temp;
//					ILI9341DrawOnePixel(i, y, string[i]);
//				}				
				
				//РїРёС€РµС‚ С‚РµРєСЃС‚
				int len=strlen(rxBuffer);
				//rxBuffer[len-2]=0x0A;
				
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
				}
				
				USART1_state = SENDED;
				//y++;
			}
			
    	//UpdateKeyState();
    }
}


void AddMenuItem(char* Title,uint16_t Color)
{
	MENU_ITEM* item=NULL;
	item = &List[ItemsCount];
	ItemsCount++;
	item->Title=Title;
	item->Color=Color;
	item->Next=NULL;
	item->Prev=NULL;
	item->BGColor=UNSELECTED_ITEM;
	if(MenuRoot==NULL)
	{
		MenuRoot=item;
		MenuLast=item;
	}
	else
	{
		MenuLast->Next	=	item;
		item->Prev		=	MenuLast;
		MenuLast		=	item;
	}
}

void DrawMenuItem(MENU_ITEM* Item)
{
	uint16_t x= Item->Rect.X;
	ILI9341DisplayString(x,Item->Rect.Y,Item->Title,Item->Color,Item->BGColor);
	ILI9341DrawHorizontalLine(x+1,Item->Rect.X+strlen(Item->Title)*8-1,Item->Rect.Y+16,RGB(128,128,128));
	ILI9341DrawHorizontalLine(x+2,Item->Rect.X+strlen(Item->Title)*8-1,Item->Rect.Y+17,RGB(128,128,128));
	ILI9341DrawVerticalLine(strlen(Item->Title)*8 + x,Item->Rect.Y,Item->Rect.Y+15,RGB(128,128,128));
	ILI9341DrawVerticalLine(strlen(Item->Title)*8 + x+1,Item->Rect.Y+1,Item->Rect.Y+16,RGB(128,128,128));
}

void UpdateGUI()
{
	MENU_ITEM* cur = MenuRoot;
	RECT rect;
	rect.Height = 20;
	rect.Widht = 200;
	rect.X = 30;
	rect.Y = 50;
	while(cur!=NULL)
	{
		cur->Rect=rect;
		DrawMenuItem(cur);
		rect.Y += rect.Height;
		cur=cur->Next;
	}
}

uint16_t Colors[3]={0x1F,0x7E0,0xF800};//{0b0000000000011111,0b0000011111100000,0b1111100000000000};

void SelectedItemChanged()
{
	int i=0;
	MENU_ITEM* cur=MenuRoot;
	while(i<ItemsCount)
	{
		if(i==PrevSelected && PrevSelected!=current)
		{
			cur->BGColor=UNSELECTED_ITEM;
			DrawMenuItem(cur);
		}
		if(i==current && PrevSelected!=current )
		{
			cur->BGColor=SELECTED_COLOR;
			DrawMenuItem(cur);
		}
		cur=cur->Next;
		i++;
	}
	PrevSelected=current;
}

void CenterButtonClicked()
{
	ILI9341DrawVerticalLine(200,50,103,Colors[current]);
	ILI9341DrawVerticalLine(199,50,103,Colors[current]);
	ILI9341DrawVerticalLine(198,50,103,Colors[current]);
	ILI9341DrawVerticalLine(197,50,103,Colors[current]);
	ILI9341DrawVerticalLine(201,51,104,RGB(128,128,128));
	ILI9341DrawVerticalLine(202,52,105,RGB(128,128,128));
	ILI9341DrawHorizontalLine(197,200,105,RGB(128,128,128));
	ILI9341DrawHorizontalLine(198,201,106,RGB(128,128,128));
}

void UpdateKeyState()
{
	uint16_t key = ReadKey();

	if(key==UP)
	{
		current--;
		if(current < 0)
		{
			current=0;
		}
		SelectedItemChanged();
	}
	else if(key==DOWN)
	{
		current++;
		if(current>=3)
		{
			current=2;
		}
		SelectedItemChanged();
	}

	if(key==RIGHT)
	{
		CenterButtonClicked();
	}


	/*if(key==UP)
		current++;
	else if(key==DOWN)
		current--;
	else if(key==LEFT)
		current+=10;
	else if(key==RIGHT)
		current-=10;*/

}

void InitRCC()
{
	RCC_DeInit();
	RCC_HSICmd(DISABLE);
	RCC_HSEConfig(RCC_HSE_ON); // разрешаем тактирование от внешнего генератора

	// ожидаем, пока внешний генератор будет готов к работе
	ErrorStatus HSEStartUpStatus = RCC_WaitForHSEStartUp();

	// если запуск внешнего генератора прошел успешно
	if (HSEStartUpStatus == SUCCESS)
	{
		/*
		 * HSE -> PREDDIV2(/5) -> PLL2MUL(*8) -> PREDDIV1(/5) -> PLL(*9) -> SYSCLK
		 * HSE / 5 * 8 / 5 * 9 = HSE * 2.88
		 *
		 * при HSE=25, SYSCLK = 72
		 */

		// предделитель 1 - делим HSE на 5
		RCC_PREDIV2Config(RCC_PREDIV2_Div5);

		// умножаем источник (PREDDIV2, не выбирается) на 8
		RCC_PLL2Config(RCC_PLL2Mul_8);
		RCC_PLL2Cmd(ENABLE);

		// устанавливаем в качестве источника PREDDIV1 PLL2MUL с предделением на 5
		RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);

		// устанавливаем к качестве источника PLL PREDDIV1, умножение на 9
		RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE);

		// выбираем в качестве источника SYSCLOCK - PLL
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		// устанавливаем делитель для сигнала HCLK (HCLK = SYSCLK)
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		// устанавливаем делитель для сигнала PCLK2 (PCLK2 = HCLK)
		RCC_PCLK2Config(RCC_HCLK_Div1);

		// устанавливаем делитель для сигнала PCLK1 (PCLK1 = HCLK/2)
		RCC_PCLK1Config(RCC_HCLK_Div2);

		// ожидаем, пока HSE не установится в качестве источника SYSCLOCK
		while (RCC_GetSYSCLKSource() != 0x08){}
	}
	else
	{   //Если HSE не смог запуститься, тактирование настроено некорректно
		//Здесь следует поместить код обработчика этой ошибки
		while (1){} // бесконечный цикл
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
