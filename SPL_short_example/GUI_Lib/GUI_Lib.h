#ifndef _GUI_LIB_
#define _GUI_LIB_

#include <stdio.h>
#include <string.h>
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include "ili9341.h"
#define UNSELECTED_ITEM RGB(76,255,255)
#define SELECTED_COLOR RGB(15,139,162)

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


extern void UpdateGUI(void);
extern void AddMenuItem(char* Title,uint16_t Color);
extern void DrawMenuItem(MENU_ITEM* Item);
extern void SelectRootItem(void);
extern void SelectedItemChanged(void);
extern void UpdateKeyState(void);
extern void CenterButtonClicked(void);
extern uint8_t ReadKey(void);

extern uint16_t ItemsCount;
extern uint16_t PrevSelected;
extern uint8_t checked;
extern int8_t current;

#endif
