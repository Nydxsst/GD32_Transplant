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
	
	WS2812B_ALL_COLOR[0] = 0xFF00FF;
	WS2812B_ALL_COLOR[1] = 0xFF0000;
	WS2812B_ALL_COLOR[2] = 0x00FF00;
	WS2812B_ALL_COLOR[3] = 0x0000FF;
	WS2812_Refresh();
	//timer_channel_output_pulse_value_config(PWM_TIMER, PWM_TIMER_CH, 8000);
	while(1)
	{
		//SYSTEM_LED = !SYSTEM_LED;
		printf("GoOn\r\n");
		Delay_Ms(500);
	}
}
