#ifndef __DELAY_H__
#define __DELAY_H__

#include <inttypes.h>
#include <stm32f10x_tim.h>
#include <misc.h>

void InitTimer(uint32_t PCLK2);
void Delay(uint32_t millis);

#endif
