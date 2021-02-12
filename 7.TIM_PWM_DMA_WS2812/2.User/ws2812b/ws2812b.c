#include "ws2812b.h"

/* 外部复用参量 --------------------------------------------------------------*/
/* RGB颜色值数组 */
u32 WS2812B_ALL_COLOR[PIXEL_MAX];

/* 内部使用参量 --------------------------------------------------------------*/
/* 通讯高低电平时间 */
const u16 T0 = 22;
const u16 T1 = 68;
/* DMA发送完成中断标志 */
u8 WS2812B_DMA_Tx_Finish = 1;
/* 定时器阈值数组结构体 */
typedef struct
{
	u16 data[24 * PIXEL_MAX];
	const u16 tail;//定时器阈值清零,输出低电平用于结束传输
} WS2812B_Tx_frame;
/* 定时器阈值数组(PWM高电平时间),用于DMA传输 */
WS2812B_Tx_frame frame = {.tail = 0};


/**
	* @brief	初始化WS2812的外设
	* @param  None
	* @retval None
	*/
void WS2812_Init()
{
	timer_parameter_struct timer_initpara;
	timer_oc_parameter_struct timer_ocinitpara;
	dma_parameter_struct dma_init_struct;

	/* GPIO Init */
	rcu_periph_clock_enable(WS2812B_GPIO_CLK);
	rcu_periph_clock_enable(RCU_AF);
	gpio_init(WS2812B_GPIO_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, WS2812B_GPIO_PIN);
	/* NVIC Init */
	nvic_irq_enable(WS2812B_TIMER_IRQn, 1, 1);
	/* TIMER Init */
	rcu_periph_clock_enable(WS2812B_TIMER_CLK);
	timer_deinit(WS2812B_TIMER);
	timer_struct_para_init(&timer_initpara);
	timer_initpara.prescaler         = WS2812B_TIMER_PREACALER;
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = WS2812B_TIMER_RELOAD;
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_init(WS2812B_TIMER, &timer_initpara);
	/* TIMER_OC Init */
	WS2812B_TIMER_REMAP;
	timer_channel_output_struct_para_init(&timer_ocinitpara);
	timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
	timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
	timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
	timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
	timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
	timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
	timer_channel_output_config(WS2812B_TIMER, WS2812B_TIMER_CH, &timer_ocinitpara);
	timer_channel_output_pulse_value_config(WS2812B_TIMER, WS2812B_TIMER_CH, 0);//设定PWM输出值
	timer_channel_output_mode_config(WS2812B_TIMER, WS2812B_TIMER_CH, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(WS2812B_TIMER, WS2812B_TIMER_CH, TIMER_OC_SHADOW_DISABLE);
	/* DMA Init */
	rcu_periph_clock_enable(WS2812B_DMA_CLK);
	dma_deinit(WS2812B_DMA, WS2812B_DMA_CNANNLE);
	dma_init_struct.periph_addr  = (uint32_t)(TIMER_CH1CV(TIMER0));//TIMER0_CH0CV;
	dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	dma_init_struct.memory_addr  = (uint32_t)&frame;
	dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;
	dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
	//dma_init_struct.number       = 3;
	dma_init_struct.priority     = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(WS2812B_DMA, WS2812B_DMA_CNANNLE, &dma_init_struct);
	dma_circulation_enable(WS2812B_DMA, WS2812B_DMA_CNANNLE);
	dma_channel_enable(WS2812B_DMA, WS2812B_DMA_CNANNLE);
	timer_primary_output_config(WS2812B_TIMER, ENABLE);
	timer_dma_enable(WS2812B_TIMER, TIMER_DMA_UPD);
	//+DMA中断使能
		
	timer_auto_reload_shadow_enable(WS2812B_TIMER);
	timer_enable(WS2812B_TIMER);
}

/**
	* @brief	中断服务函数
	* @param  None
	* @retval None
	*/
void WS2812B_NVIC_IRQ(void)
{
	if(DMA_GetITStatus(WS2812B_DMA_FLAG) != RESET)
	{
		SYSTEM_LED = !SYSTEM_LED;
		//printf("WS2812B Intrrupt!\r\n");
		WS2812B_DMA_Tx_Finish = 1;
		DMA_Cmd(WS2812B_DMA_CNANNLE, DISABLE);
		TIM_Cmd(WS2812B_TIM, DISABLE);
		DMA_ClearITPendingBit(WS2812B_DMA_FLAG);
	}
}

/**
	* @brief	WS2812更新颜色, DMA传输时间为(1 + 24 * PIXEL_MAX) * T(1.25us)
	*					理论上还需要224周期的低电平完成Reset
	*					如果出现bug，两次调用该函数之间自行延时224个周期
	* @param  None
	* @retval 1为发送成功,0为发送失败(上一次DMA传输还没完成)
	*/
u8 WS2812_Refresh(void)
{
	u8 tmp = 0, n = 0, i = 0;

	if(WS2812B_DMA_Tx_Finish == 0)return 0;//检查上次DMA传输是否已经完成
	WS2812B_DMA_Tx_Finish = 0;//完成就标志清零，再进行下一次传输

	for(n = 0; n < PIXEL_MAX; n++)
	{
		tmp = (u8)(WS2812B_ALL_COLOR[n] >> 8);
		for(i = 0; i < 8; i++)
		{
			frame.data[i + 24 * n] = (tmp & 0x80) ? T1 : T0;
			tmp <<= 1;
		}
		tmp = (u8)(WS2812B_ALL_COLOR[n] >> 16);
		for(i = 0; i < 8; i++)
		{
			frame.data[8 + i + 24 * n] = (tmp & 0x80) ? T1 : T0;
			tmp <<= 1;
		}
		tmp = (u8)WS2812B_ALL_COLOR[n];
		for(i = 0; i < 8; i++)
		{
			frame.data[16 + i + 24 * n] = (tmp & 0x80) ? T1 : T0;
			tmp <<= 1;
		}
	}
	DMA_SetCurrDataCounter(WS2812B_DMA_CNANNLE, 24 * PIXEL_MAX + 1);
	DMA_Cmd(WS2812B_DMA_CNANNLE, ENABLE);
	TIM_Cmd(WS2812B_TIM, ENABLE);
	return 1;
}
