#include "timer.h"

/**
	* @brief	初始化TIMER
	* @param  None
	* @retval None
	*/
void TIMER_Init(void)
{
	timer_parameter_struct timer_initpara;
	timer_oc_parameter_struct timer_ocinitpara;

	/* GPIO Init */
	rcu_periph_clock_enable(PWM_GPIO_CLK);
	rcu_periph_clock_enable(RCU_AF);
	gpio_init(PWM_GPIO_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, PWM_GPIO_PIN);
	/* TIMER Init */
	rcu_periph_clock_enable(PWM_TIMER_CLK);
	timer_deinit(PWM_TIMER);
	timer_struct_para_init(&timer_initpara);
	timer_initpara.prescaler         = PWM_TIMER_PREACALER;
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = PWM_TIMER_RELOAD;
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_init(PWM_TIMER, &timer_initpara);
	/* TIMER_OC Init */
	PWM_TIMER_REMAP;
	timer_channel_output_struct_para_init(&timer_ocinitpara);
	timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
	timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
	timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
	timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
	timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
	timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
	timer_channel_output_config(PWM_TIMER, PWM_TIMER_CH, &timer_ocinitpara);
	timer_channel_output_pulse_value_config(PWM_TIMER, PWM_TIMER_CH, 0);//设定PWM输出值
	timer_channel_output_mode_config(PWM_TIMER, PWM_TIMER_CH, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(PWM_TIMER, PWM_TIMER_CH, TIMER_OC_SHADOW_DISABLE);
	
	timer_auto_reload_shadow_enable(PWM_TIMER);
	timer_enable(PWM_TIMER);
}


