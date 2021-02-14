/**
  ******************************************************************************
  * @author
  * @version V1.0.0
  * @date    2020/05/14
  ******************************************************************************
	* @attention
	* ������õ�PA13.PA14.PA15.PB3.PB4������Ҫ��jlink
	* rcu_periph_clock_enable(RCU_AF);
	* gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
  ******************************************************************************
  */

#include "gd32e10x.h"

int main(void)
{
	/* �ر�JLINK */
	rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	/* ����ϵͳʱ�� */
	Delay_Init();
	Led_Init();

	while(1)
	{
//		/* turn on LED */
//		gpio_bit_set(LED_GPIO, LED_PIN);
//		Delay_Ms(200);

//		/* turn off LED */
//		gpio_bit_reset(LED_GPIO, LED_PIN);
//		Delay_Ms(200);
		SYSTEM_LED = !SYSTEM_LED;
		Delay_Ms(500);
	}
}
