/**
  ******************************************************************************
  * @author  Nydxsst
  * @version V1.0.0
  * @date    2020/02/21
  ******************************************************************************
	* @attention 
	* 芯片DMA BUG，dma -> timer_CCR传输时，会丢掉传输数据的第二位
	* 本驱动采用跳过第二位的方式规避该BUG
  ******************************************************************************
  */

#ifndef __WS2812B_H
#define __WS2812B_H

#include "gd32e10x.h"


/* 内部定量（根据需求修改） --------------------------------------------------*/
#define PIXEL_MAX 8 //the count of all the 2812 

/* TIMER配置参数 -------------------------------------------------------------*/
//TIM打拍频率 = 120M/prescaler, 周期 = LOAD/打拍频率
#define WS2812B_TIMER_PREACALER		1U
#define WS2812B_TIMER_RELOAD			77U

/* TIMER操作线 ---------------------------------------------------------------*/
#define WS2812B_GPIO_CLK					RCU_GPIOA
#define WS2812B_GPIO_PORT					GPIOA
#define WS2812B_GPIO_PIN					GPIO_PIN_15

#define WS2812B_TIMER							TIMER1
#define WS2812B_TIMER_CLK					RCU_TIMER1
#define WS2812B_TIMER_CH					TIMER_CH_0
#define WS2812B_TIMER_CHCV				TIMER_CH0CV
#define WS2812B_TIMER_DMA_CH			TIMER_DMA_CH0D
#define WS2812B_TIMER_REMAP				gpio_pin_remap_config(GPIO_TIMER1_FULL_REMAP, ENABLE)

#define WS2812B_DMA								DMA0
#define WS2812B_DMA_CNANNLE				DMA_CH4
#define WS2812B_DMA_CLK						RCU_DMA0
#define WS2812B_NVIC_IRQn					DMA0_Channel4_IRQn
#define WS2812B_NVIC_IRQH					DMA0_Channel4_IRQHandler


/* 外部调用函数 --------------------------------------------------------------*/
void WS2812_Init(void);
u8 WS2812_Refresh(void);


#endif
