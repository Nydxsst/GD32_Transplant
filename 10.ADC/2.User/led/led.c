#include "led.h"

/**
	* @brief	初始化LED的GPIO口
	* @param  None
	* @retval None
	*/
void Led_Init(void)
{
	rcu_periph_clock_enable(SYSTEM_LED_CLK);
	gpio_init(SYSTEM_LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SYSTEM_LED_PIN);

	SYSTEM_LED = 1;
}
