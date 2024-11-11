#include "GUI_Lib.h"

MENU_ITEM* MenuRoot;
MENU_ITEM* MenuLast;
MENU_ITEM* SelectedItem;
MENU_ITEM List[5];
uint16_t ItemsCount=0;
uint16_t PrevSelected;
uint8_t checked=0;
int8_t current=0;

void AddMenuItem(char* Title,uint16_t Color)
{
	MENU_ITEM* item=NULL;
	item = &List[ItemsCount];
	ItemsCount++;
	item->Title=Title;
	item->Color=Color;
	item->Next=NULL;
	item->Prev=NULL;
	item->BGColor= UNSELECTED_ITEM;
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

void UpdateGUI(void)
{
	MENU_ITEM* cur = MenuRoot;
	RECT rect;
	rect.Height = 20;
	rect.Widht = 200;
	rect.X = 100;
	rect.Y = 50;
	while(cur!=NULL)
	{
		cur->Rect=rect;
		DrawMenuItem(cur);
		rect.Y += rect.Height;
		cur=cur->Next;
	}
}

uint16_t Colors[3]={0x60DC,0x7E0,0xF800};//{0b0000000000011111,0b0000011111100000,0b1111100000000000};


void SelectedItemChanged(void)
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

void SelectRootItem(void)
{
	MENU_ITEM* cur=MenuRoot;
	cur->BGColor=SELECTED_COLOR;
	DrawMenuItem(cur);
}

void CenterButtonClicked(void)
{
	for(int i = 0; i<15; i++)
	{
		ILI9341DrawVerticalLine(197+i,50,103,Colors[current]);
	}
	/*ILI9341DrawVerticalLine(200,50,103,Colors[current]);
	ILI9341DrawVerticalLine(199,50,103,Colors[current]);
	ILI9341DrawVerticalLine(198,50,103,Colors[current]);
	ILI9341DrawVerticalLine(197,50,103,Colors[current]);
	ILI9341DrawVerticalLine(201,51,104,RGB(128,128,128));
	ILI9341DrawVerticalLine(202,52,105,RGB(128,128,128));
	ILI9341DrawHorizontalLine(197,200,105,RGB(128,128,128));
	ILI9341DrawHorizontalLine(198,201,106,RGB(128,128,128));*/
}

void UpdateKeyState(void)
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

uint8_t ReadKey(void)
{
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
