/**
  ******************************************************************************
  * @author  
  * @version V1.0.0
  * @date    2020/05/14
  ******************************************************************************
	* @attention 
  ******************************************************************************
  */

#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"


/* LED操作线 -----------------------------------------------------------------*/
#define SYSTEM_LED				PBout(2)
#define SYSTEM_LED_CLK		RCC_APB2Periph_GPIOB
#define SYSTEM_LED_PORT		GPIOB
#define SYSTEM_LED_PIN		GPIO_Pin_2

#define USE_LED						PAout(15)
#define USE_LED_CLK				RCC_APB2Periph_GPIOA
#define USE_LED_PORT			GPIOA
#define USE_LED_PIN				GPIO_Pin_15


/* 外部调用函数 --------------------------------------------------------------*/
void Led_Init(void);

#endif
