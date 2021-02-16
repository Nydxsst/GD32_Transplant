/**
  ******************************************************************************
  * @author  Moment/Nydxsst
  * @version V1.0.0
  * @date    2020/02/12
  ******************************************************************************
	* @attention
	* 如果需用到PA13.PA14.PA15.PB3.PB4，则需要关jlink
	* rcu_periph_clock_enable(RCU_AF);
	* gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
  ******************************************************************************
  */

#include "gd32e10x.h"
#include "144lcd_st7735s.h"
#include "adc.h"

int main(void)
{
	char Adc_Val[3];
	/* 关闭JLINK */
	rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	/* 中断优先级选组 */
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	/* 外设初始化 */
	Delay_Init();
	Led_Init();
	Usart_Debug_Init(115200);
	printf("start\n");
	ST7735S_CPT144_Init();
	Adc_Init();
	
	ST7735S_CPT144_SetBackColor(ST7735S_CPT144_BLACK);

	while(1)
	{	
		SYSTEM_LED = ~SYSTEM_LED;
		sprintf(Adc_Val,"Count : %d",Get_Adc_Val_Average(5));
		ST7735S_CPT144_Clear();
		ST7735S_CPT144_SetTextColor(ST7735S_CPT144_RED);
		ST7735S_CPT144_DispString_XDir(1 ,1, Adc_Val);
		Delay_Ms(500);
	}
}
