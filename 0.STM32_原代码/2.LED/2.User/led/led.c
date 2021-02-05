#include "led.h"

/**
	* @brief	初始化LED的GPIO口
	* @param  None
	* @retval None
	*/
void Led_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(SYSTEM_LED_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SYSTEM_LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SYSTEM_LED_PORT, &GPIO_InitStructure);

	SYSTEM_LED = 1;
}
