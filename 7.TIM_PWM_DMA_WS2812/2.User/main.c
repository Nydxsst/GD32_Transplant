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
#include "ws2812b.h"

extern u32 WS2812B_ALL_COLOR[PIXEL_MAX];

int main(void)
{
	/* 关闭JLINK */
	rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	/* 中断优先级选组 */
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	/* 外设初始化 */
	Delay_Init();
	Led_Init();
	Usart_Debug_Init(115200);
	WS2812_Init();
	
	printf("Start\r\n");
	
	WS2812B_ALL_COLOR[0] = 0x0F0000;//red
	WS2812B_ALL_COLOR[1] = 0x000F00;//green
	WS2812B_ALL_COLOR[2] = 0x00000F;//blue
	WS2812B_ALL_COLOR[3] = 0x0F000F;//purple
	WS2812B_ALL_COLOR[4] = 0x0F000F;
	WS2812B_ALL_COLOR[5] = 0x00000F;
	WS2812B_ALL_COLOR[6] = 0x000F00;
	WS2812B_ALL_COLOR[7] = 0x0F0000;
	WS2812_Refresh();
	while(1)
	{
		SYSTEM_LED = !SYSTEM_LED;
		//printf("GoOn\r\n");
		Delay_Ms(500);
	}
}
