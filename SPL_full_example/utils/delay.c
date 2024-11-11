
#include "delay.h"

volatile uint32_t counter;

void InitTimer(uint32_t PCLK2)
{
	TIM_TimeBaseInitTypeDef base_timer;
	TIM_TimeBaseStructInit(&base_timer);
  // Настраиваем предделитель
	// Частота тактового сигнала, подаваемого на таймер,
	// определяется параметром PCLK2, поэтому, установив 
	// предделитель равным PCLK2 / 1000000 - 1,
	// получаем сигнал с частотой 1 МГц
	// Например, если PCLK2 = 72 000 000,
	// получаем предделитель 72000000/1000000-1 = 71.
	// Соответсвенно, частота тактового сигнала,
	// подаваемого с предделителя на таймер, равна 
  // 72000000/(71+1) = 1000000 Гц	
	base_timer.TIM_Prescaler = PCLK2 / 1000000 - 1;

	// Настраиваем период таймера
	// Если после предделителя частота 1 МГц, 
	// то, чтобы получить период переключения таймера
	// 1 мс, необходимо установить TIM_Period = 1000
	base_timer.TIM_Period = 1000;

	TIM_TimeBaseInit(TIM6, &base_timer);

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM6, ENABLE);
	NVIC_EnableIRQ(TIM6_IRQn);
}

// формирование временной задержки в миллисекундах
void Delay(uint32_t millis)
{
	counter = millis;
	TIM_Cmd(TIM6, ENABLE);
	while(counter>0){};
	TIM_Cmd(TIM6, DISABLE);
}

void TIM6_IRQHandler()
{
  if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

    counter--;
  }
}
