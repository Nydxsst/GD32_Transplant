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
#define PWM_TIMER_PREACALER		119
#define PWM_TIMER_RELOAD			15999

/* TIMER操作线 ---------------------------------------------------------------*/
#define PWM_GPIO_CLK					RCU_GPIOA
#define PWM_GPIO_PORT					GPIOA
#define PWM_GPIO_PIN					GPIO_PIN_15

#define PWM_TIMER							TIMER1
#define PWM_TIMER_CLK					RCU_TIMER1
#define PWM_TIMER_CH					TIMER_CH_0
#define PWM_TIMER_REMAP				gpio_pin_remap_config(GPIO_TIMER1_FULL_REMAP, ENABLE)


/* 外部调用函数 --------------------------------------------------------------*/
void TIMER_Init(void);

#endif
