/**
  ******************************************************************************
  * @author  Nydxsst
  * @version V1.0.0
  * @date    2020/02/06
  ******************************************************************************
	* @attention 
  ******************************************************************************
  */

#ifndef __TIMER_H
#define __TIMER_H

#include "gd32e10x.h"


/* TIMER配置参数 -------------------------------------------------------------*/
//TIM打拍频率 = 120M/prescaler, 周期 = LOAD/打拍频率
#define INT_TIMER_PREACALER		11999U
#define INT_TIMER_RELOAD			9999U
/* TIMER操作线 ---------------------------------------------------------------*/
#define INT_TIMER							TIMER2
#define INT_TIMER_IRQn				TIMER2_IRQn
#define INT_TIMER_CLK					RCU_TIMER2
#define INT_TIMER_IRQH				TIMER2_IRQHandler

/* 外部调用函数 --------------------------------------------------------------*/
void TIMER_Init(void);

#endif
