#include "timer.h"

/**
	* @brief	初始化TIMER
	* @param  None
	* @retval None
	*/
void TIMER_Init(void)
{
	timer_parameter_struct timer_initpara;

	/* NVIC Init */
	nvic_irq_enable(INT_TIMER_IRQn, 1, 1);
	/* TIMER Init */
	rcu_periph_clock_enable(INT_TIMER_CLK);
	//timer_deinit(INT_TIMER);
	timer_initpara.prescaler         = INT_TIMER_PREACALER;
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;//中心计数模式使能与选择
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = INT_TIMER_RELOAD;
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0U;//n+1个计数周期触发一次中断
	timer_init(INT_TIMER, &timer_initpara);

	//timer_interrupt_flag_clear(INT_TIMER, TIMER_INT_FLAG_UP);
	timer_interrupt_enable(INT_TIMER, TIMER_INT_UP);
	timer_enable(INT_TIMER);
}

/**
	* @brief	TIMER中断执行函数
	* @param  None
	* @retval None
	*/
void INT_TIMER_IRQH(void)
{
	if(timer_interrupt_flag_get(INT_TIMER, TIMER_INT_FLAG_UP) == SET)
	{
		/* clear update interrupt bit */
		timer_interrupt_flag_clear(INT_TIMER, TIMER_INT_FLAG_UP);
		SYSTEM_LED = !SYSTEM_LED;
	}
}
