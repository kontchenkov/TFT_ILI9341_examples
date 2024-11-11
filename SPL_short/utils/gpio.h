#ifndef __GPIO_H__
#define __GPIO_H__

#include <stm32f10x_gpio.h>

void InitGPIO(GPIO_TypeDef* gpio, GPIOMode_TypeDef mode, uint16_t pin, GPIOSpeed_TypeDef speed);

#endif
