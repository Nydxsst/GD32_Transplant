/**
  ******************************************************************************
  * @author  Nydxsst
  * @version V1.0.0
  * @date    2020/01/24
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
	u8 Key_Press = 0;
	
	/* �ر�JLINK */
	rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	
	Delay_Init();
	Usart_Debug_Init(115200);
	Led_Init();
	Key_Init();
	
	while(1)
	{
		Key_Press = KEY_Scan();
		if(Key_Press == KEY1_PRES)SYSTEM_LED = !SYSTEM_LED;
		else if(Key_Press == KEY2_PRES)SYSTEM_LED = !SYSTEM_LED;
		else if(Key_Press == KEY3_PRES)SYSTEM_LED = !SYSTEM_LED;
	}
}
