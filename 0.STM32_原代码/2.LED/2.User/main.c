/**
  ******************************************************************************
  * @author  
  * @version V1.0.0
  * @date    2020/05/14
  ******************************************************************************
	* @attention 
	* 如果需用到PA13.PA14.PA15.PB3.PB4，则需要关jlink
	* RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	* GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
  ******************************************************************************
  */
	
#include "stm32f10x.h"


int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	Delay_Init();
	Usart_Debug_Init(115200);
	Led_Init();
	
	while(1)
	{
		Delay_Ms(500);
		SYSTEM_LED = !SYSTEM_LED;
	}
}

