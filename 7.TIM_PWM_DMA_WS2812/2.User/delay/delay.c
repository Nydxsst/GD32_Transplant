#include "delay.h"

/* 内部使用参量 --------------------------------------------------------------*/
u8  Fac_Us = 0;							//us延时倍乘数
u16 Fac_Ms = 0;							//ms延时倍乘数

void Delay_ms(u16 Nms);

/**
	* @brief	初始化延迟函数
	* @param  None
	* @retval None
	* @note		SYSTICK的时钟固定为HCLK时钟的1/8
	* @note   SYSCLK:系统时钟
	*/
void Delay_Init(void)
{
	systick_clksource_set(SYSTICK_CLKSOURCE_HCLK_DIV8);	//选择外部时钟  HCLK/8
	Fac_Us = 15;										//每1us，计数器递减n个数（120M/8）
	Fac_Ms = (u16)Fac_Us * 1000;	//每1ms，计数器递减n*1000个数
}

/**
	* @brief	us级延时
	* @param	nus:延时的us数
	* @retval None
	* @note		有一定的误差,us越小,误差比越大
	*/		    
void Delay_Us(u32 Nus)
{
	SysTick->LOAD = Fac_Us * Nus;//自动重装载值
	SysTick->VAL = 0x00;//清除计时器的值
	SysTick->CTRL |= (1 << 0);//SysTick使能，使能后定时器开始倒数
	while(!(SysTick->CTRL & (1 << 16)));//判断是否减到0，减到0时CTRL的第16位会置1，读取后会自动置0
}

/**
	* @brief	Ms级延时
	* @param	Nms:延时的Ms数
	* @retval None
	* @note		SysTick->LOAD为24位寄存器, 延时的Us数/周期 = 填入寄存器的数
	* 				即Nms * 1000 * SysTick_Clk(15M) <= 2^24, 即Nms <= 1118
	*/				    
void Delay_ms(u16 Nms)
{
	SysTick->LOAD = Fac_Ms * Nms;//自动重装载值
	SysTick->VAL = 0x00;//清除计时器的值
	SysTick->CTRL |= (1 << 0);//SysTick使能，使能后定时器开始倒数
	while(!(SysTick->CTRL & (1 << 16)));//判断是否减到0，减到0时CTRL的第16位会置1，读取后会自动置0
}

/**
	* @brief	Ms级延时
	* @param	Nms:延时的Ms数
	* @retval None
	* @note		调用Delay_ms,用于处理输入参数Nms,使Nms:0~65535
	*/	
void Delay_Ms(u16 Nms)
{	 	 
	u8 repeat=Nms/1000;	
	u16 remain=Nms%1000;
	while(repeat)
	{
		Delay_ms(1000);
		repeat--;
	}
	if(remain)Delay_ms(remain);
} 
