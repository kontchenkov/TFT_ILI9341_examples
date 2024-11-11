#include "gpio.h"

void InitGPIO(GPIO_TypeDef* gpio, GPIOMode_TypeDef mode, uint16_t pin, GPIOSpeed_TypeDef speed)
{
    GPIO_InitTypeDef gpio_init_structure;
    gpio_init_structure.GPIO_Mode = mode;
    gpio_init_structure.GPIO_Pin = pin;
    gpio_init_structure.GPIO_Speed = speed;
    GPIO_Init(gpio,&gpio_init_structure);
}
