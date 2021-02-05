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

#include "stm32f10x.h"


/* ADC操作线 -----------------------------------------------------------------*/
#define ADC_GPIO_CLK		RCC_APB2Periph_GPIOA
#define ADC_GPIO_PORT		GPIOA
#define ADC_GPIO_PIN		GPIO_Pin_1

#define ADCx            ADC1
#define ADC_CLK         RCC_APB2Periph_ADC1
#define ADC_CHANNEL     ADC_Channel_1

/* 外部调用函数 --------------------------------------------------------------*/
void Adc_Init(void);
u16 Get_Adc_val(void);
u16 Get_Adc_Val_Average(u8 times);
u8 Get_Adc_BAT_Percent(u8 times);

#endif
