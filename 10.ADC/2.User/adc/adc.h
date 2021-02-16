/**
  ******************************************************************************
  * @author  
  * @version V1.0.0
  * @date    2020/10/25
  ******************************************************************************
	* @attention 
  ******************************************************************************
  */

#ifndef __ADC_H
#define __ADC_H

#include "gd32e10x.h"


/* ADC操作线 -----------------------------------------------------------------*/
#define ADC_GPIO_CLK		RCU_GPIOA
#define ADC_GPIO_PORT		GPIOA
#define ADC_GPIO_PIN		GPIO_PIN_1

#define ADCx            ADC0
#define ADC_CLK         RCU_ADC0
#define ADC_CLK_DIV			RCU_CKADC_CKAPB2_DIV2
#define ADC_CHANNEL     ADC_CHANNEL_1

/* 外部调用函数 --------------------------------------------------------------*/
void Adc_Init(void);
u16 Get_Adc_val(void);
u16 Get_Adc_Val_Average(u8 times);
u8 Get_Adc_BAT_Percent(u8 times);

#endif
