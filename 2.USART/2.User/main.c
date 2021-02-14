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
	/* �ر�JLINK */
	rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	/* �ж����ȼ�ѡ�� */
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	/* �����ʼ�� */
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
