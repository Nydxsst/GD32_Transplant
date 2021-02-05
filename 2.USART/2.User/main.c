/**
  ******************************************************************************
  * @author  Nydxsst
  * @version V1.0.0
  * @date    2020/01/24
  ******************************************************************************
	* @attention
	* 如果需用到PA13.PA14.PA15.PB3.PB4，则需要关jlink
	* rcu_periph_clock_enable(RCU_AF);
	* gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
  ******************************************************************************
  */

#include "gd32e10x.h"
#include "delay.h"
#include "led.h"
#include "usart_debug.h"


int main(void)
{
	/* 关闭JLINK */
	rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);

	Delay_Init();
	Led_Init();
	Usart_Debug_Init(115200);

	printf("Start\r\n");
	while(1)
	{
		SYSTEM_LED = !SYSTEM_LED;
		Delay_Ms(500);
	}
}
