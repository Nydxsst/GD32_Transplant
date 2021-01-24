/**
  ******************************************************************************
  * @author
  * @version V1.0.0
  * @date    2020/05/14
  ******************************************************************************
	* @attention
	* 如果需用到PA13.PA14.PA15.PB3.PB4，则需要关jlink
	* rcu_periph_clock_enable(RCU_AF);
	* gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
  ******************************************************************************
  */

#include "gd32e10x.h"
#include "systick.h"
#include "delay.h"
#include "led.h"

int main(void)
{
	/* 关闭JLINK */
	rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	/* 配置系统时钟 */
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
