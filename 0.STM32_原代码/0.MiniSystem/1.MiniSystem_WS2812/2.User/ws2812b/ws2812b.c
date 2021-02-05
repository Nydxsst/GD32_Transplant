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
	GPIO_InitTypeDef				GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef				TIM_OCInitStruct;
	DMA_InitTypeDef					DMA_InitStruct;
	NVIC_InitTypeDef				NVIC_InitStructure;

	/* GPIO_Init */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(WS2812B_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = WS2812B_GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WS2812B_GPIO_PORT, &GPIO_InitStruct);

	/* TIM_TimeBaseInit */
	WS2812B_TIM_RCC_CMD(WS2812B_TIM_CLK, ENABLE);
	WS2812B_TIM_REMAP;
	TIM_TimeBaseInitStruct.TIM_Period = 90 - 1; //(800khz, T=1.25us)
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(WS2812B_TIM, &TIM_TimeBaseInitStruct);

	/* TIM_OCInit */
	/*PWM模式1:	向上记数,TIMx_CNT<TIMx_CCR1时输出为有效电平,否则为无效电平
							向下记数,TIMx_CNT>TIMx_CCR1时输出为有效电平,否则为无效电平*/
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;		// 有效电平为高电平
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;// 输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;														// 设置初始占空比阈值
	WS2812B_TIM_OCINIT(WS2812B_TIM, &TIM_OCInitStruct);
	WS2812B_TIM_OC_PRE(WS2812B_TIM, TIM_OCPreload_Enable);

	/* NVIC_Init */
	NVIC_InitStructure.NVIC_IRQChannel = WS2812B_NVIC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = WS2812B_NVIC_PRE;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = WS2812B_NVIC_SUB;				
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);

	/* DMA_Init */
	WS2812B_DMA_RCC_CMD(WS2812B_DMA_CLK, ENABLE);
	DMA_DeInit(WS2812B_DMA_CNANNLE);
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;											// DMA模式，一次模式
	DMA_InitStruct.DMA_BufferSize = 0;															// 传输数据个数
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;									// 方向：从内存到外设
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;												// 禁止内存到内存的传输
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&frame;						// 内存地址(要传输的变量的指针)
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;// 内存数据单位
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;						// 内存地址自增
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&WS2812B_TIM->WS2812B_TIM_CCR;	// 外设地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;// 外设数据单位
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		// 外设地址不增
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;								// 优先级
	DMA_Init(WS2812B_DMA_CNANNLE, &DMA_InitStruct);

	DMA_ITConfig(WS2812B_DMA_CNANNLE, DMA_IT_TC, ENABLE);
	TIM_DMACmd(WS2812B_TIM, TIM_DMA_Update, ENABLE);
}
/**
	* @brief	中断服务函数
	* @param  None
	* @retval None
	*/
void WS2812B_NVIC_IRQHandler(void)
{
	if(DMA_GetITStatus(WS2812B_DMA_FLAG) != RESET)
	{
		//SYSTEM_LED = !SYSTEM_LED;
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
