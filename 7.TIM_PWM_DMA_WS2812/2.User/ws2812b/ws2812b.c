#include "ws2812b.h"

/* 外部复用参量 --------------------------------------------------------------*/
/* RGB颜色值数组 */
u32 WS2812B_ALL_COLOR[PIXEL_MAX];

/* 内部使用参量 --------------------------------------------------------------*/
/* 通讯高低电平时间 */
//TIM打拍频率 = 120M/prescaler = 60M; 18/60M = 300ns; 60/60M = 1us
const u16 T0 = 18;
const u16 T1 = 60; 
/* DMA发送完成中断标志 */
u8 WS2812B_DMA_Tx_Finish = 1;
/* 定时器阈值数组结构体 */
typedef struct
{
	u16 data[24 * PIXEL_MAX + 1];//芯片DMA BUG，会丢掉传输数据的第二位，故这里多加一位
	u16 tail;//定时器阈值清零,输出低电平用于结束传输
} WS2812B_Tx_frame;
/* 定时器阈值数组(PWM高电平时间),用于DMA传输 */
WS2812B_Tx_frame frame;

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
	nvic_irq_enable(WS2812B_NVIC_IRQn, 1, 1);
	/* TIMER Init */
	rcu_periph_clock_enable(WS2812B_TIMER_CLK);
	//timer_deinit(WS2812B_TIMER);
	timer_initpara.prescaler         = WS2812B_TIMER_PREACALER;
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = WS2812B_TIMER_RELOAD;
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0U;
	timer_init(WS2812B_TIMER, &timer_initpara);
	/* TIMER_OC Init */
	WS2812B_TIMER_REMAP;
	timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
	timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;//互补输出禁用
	timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
	timer_ocinitpara.ocnpolarity  = TIMER_OC_IDLE_STATE_HIGH;
	timer_ocinitpara.ocidlestate  = TIMER_OCN_POLARITY_LOW;
	timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
	timer_channel_output_config(WS2812B_TIMER, WS2812B_TIMER_CH, &timer_ocinitpara);
	timer_channel_output_pulse_value_config(WS2812B_TIMER, WS2812B_TIMER_CH, 0);//设定PWM输出值
	timer_channel_output_mode_config(WS2812B_TIMER, WS2812B_TIMER_CH, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(WS2812B_TIMER, WS2812B_TIMER_CH, TIMER_OC_SHADOW_ENABLE);
	
	//timer_primary_output_config(WS2812B_TIMER, ENABLE);//所有通道输出使能
	timer_auto_reload_shadow_enable(WS2812B_TIMER);
	
	/* DMA Init */
	rcu_periph_clock_enable(WS2812B_DMA_CLK);
	dma_init_struct.periph_addr  = (uint32_t)&(WS2812B_TIMER_CHCV(WS2812B_TIMER));//((uint32_t)0x040000034)
	dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	dma_init_struct.memory_addr  = (uint32_t)&frame;
	dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;
	dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
	dma_init_struct.number       = 0;
	dma_init_struct.priority     = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(WS2812B_DMA, WS2812B_DMA_CNANNLE, &dma_init_struct);
	//dma_circulation_enable(WS2812B_DMA, WS2812B_DMA_CNANNLE);//循环发送使能
	
	dma_interrupt_enable(WS2812B_DMA, WS2812B_DMA_CNANNLE, DMA_INT_FTF);//DMA中断使能
	timer_dma_enable(WS2812B_TIMER, WS2812B_TIMER_DMA_CH);
}

/**
	* @brief	中断服务函数
	* @param  None
	* @retval None
	*/
void WS2812B_NVIC_IRQH(void)
{
	if(dma_interrupt_flag_get(WS2812B_DMA, WS2812B_DMA_CNANNLE, DMA_INT_FLAG_FTF) != RESET)
	{
		//printf("WS2812B Intrrupt!\r\n");
		WS2812B_DMA_Tx_Finish = 1;
		dma_channel_disable(WS2812B_DMA, WS2812B_DMA_CNANNLE);
		timer_disable(WS2812B_TIMER);
		dma_interrupt_flag_clear(WS2812B_DMA, WS2812B_DMA_CNANNLE, DMA_INT_FLAG_FTF);
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
	u8 DMA_BUG_BIT_SET = 0;
	
	if(WS2812B_DMA_Tx_Finish == 0)return 0;//检查上次DMA传输是否已经完成
	WS2812B_DMA_Tx_Finish = 0;//完成就标志清零，再进行下一次传输
	
	for(n = 0; n < PIXEL_MAX; n++)
	{
		tmp = (u8)(WS2812B_ALL_COLOR[n] >> 8);
		for(i = 0; i < 8; i++)
		{
			if(DMA_BUG_BIT_SET == 0 && i == 0)
			{
				//将第一个数据放在data[0]，第二个数据放在data[3],第n个数据放在data[n+1]
				//至于data[2]，会随着dma的传输消失在茫茫宇宙中(笑)
				frame.data[0] = (tmp & 0x80) ? T1 : T0;	
				DMA_BUG_BIT_SET = 1;
			}
			else frame.data[i + 24 * n + 1] = (tmp & 0x80) ? T1 : T0;
			tmp <<= 1;
		}
		tmp = (u8)(WS2812B_ALL_COLOR[n] >> 16);
		for(i = 0; i < 8; i++)
		{
			frame.data[8 + i + 24 * n + 1] = (tmp & 0x80) ? T1 : T0;
			tmp <<= 1;
		}
		tmp = (u8)WS2812B_ALL_COLOR[n];
		for(i = 0; i < 8; i++)
		{
			frame.data[16 + i + 24 * n + 1] = (tmp & 0x80) ? T1 : T0;
			tmp <<= 1;
		}
	}
	frame.tail = 0;//定时器阈值清零,输出低电平用于结束传输
	timer_enable(WS2812B_TIMER);
	dma_transfer_number_config(WS2812B_DMA, WS2812B_DMA_CNANNLE, 24 * PIXEL_MAX + 2);
	dma_channel_enable(WS2812B_DMA, WS2812B_DMA_CNANNLE);
	
	return 1;
}
