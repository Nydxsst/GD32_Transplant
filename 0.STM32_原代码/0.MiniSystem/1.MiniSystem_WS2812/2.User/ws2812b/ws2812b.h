/**
  ******************************************************************************
  * @author  
  * @version V1.0.0
  * @date    2020/04/07
  ******************************************************************************
  * @attention
	* 空闲状态是低电平
	* 手册中：RES - 280us以上的低电平（意思是数据重装载而非2818复位）
	*					串联多个2812时，相邻两个数据之间不能有RES
	*	该驱动一次性必须同时刷新所有WS2812
	* 使用TIM_PWM与DMA资源，注意对应DMA通道选择是TIM_UP事件
  ******************************************************************************
  */

#ifndef __WS2812B_H
#define __WS2812B_H

#include "stm32f10x.h"


/* 内部定量（根据需求修改） --------------------------------------------------*/
#define PIXEL_MAX 8 //the count of all the 2812 

/* WS2812B操作线 -------------------------------------------------------------*/
#define WS2812B_GPIO_CLK				RCC_APB2Periph_GPIOB
#define WS2812B_GPIO_PORT				GPIOB
#define WS2812B_GPIO_PIN				GPIO_Pin_5

#define WS2812B_TIM							TIM3
#define WS2812B_TIM_RCC_CMD			RCC_APB1PeriphClockCmd
#define WS2812B_TIM_CLK					RCC_APB1Periph_TIM3
#define WS2812B_TIM_REMAP				GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE)
#define WS2812B_TIM_OCINIT			TIM_OC2Init
#define WS2812B_TIM_OC_PRE			TIM_OC2PreloadConfig
#define WS2812B_TIM_CCR					CCR2

#define WS2812B_NVIC_IRQn				DMA1_Channel3_IRQn
#define WS2812B_NVIC_IRQHandler	DMA1_Channel3_IRQHandler
#define WS2812B_NVIC_PRE				2 //抢占优先级
#define WS2812B_NVIC_SUB				1 //子优先级

#define WS2812B_DMA_RCC_CMD			RCC_AHBPeriphClockCmd
#define WS2812B_DMA_CLK					RCC_AHBPeriph_DMA1
#define WS2812B_DMA_CNANNLE			DMA1_Channel3
#define WS2812B_DMA_FLAG				DMA1_FLAG_TC3


/* 外部调用函数 --------------------------------------------------------------*/
void WS2812_Init(void);
u8 WS2812_Refresh(void);


#endif
