#include "adc.h"

/**
	* @brief	初始化ADC(仅使用规则通道)
	* @param  None
	* @retval None
	*/
void Adc_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIO_Init */
	RCC_APB2PeriphClockCmd(ADC_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_GPIO_PORT, &GPIO_InitStructure);
	
	/* ADC_Init */
	RCC_APB2PeriphClockCmd(ADC_CLK, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   												//设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	ADC_DeInit(ADCx);  																			//复位ADC
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;									//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADCx, &ADC_InitStructure);											//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

	ADC_Cmd(ADCx, ENABLE);											//使能指定的ADC1
	ADC_ResetCalibration(ADCx);									//使能复位校准
	while(ADC_GetResetCalibrationStatus(ADCx));	//等待复位校准结束
	ADC_StartCalibration(ADCx);									//开启AD校准
	while(ADC_GetCalibrationStatus(ADCx));			//等待校准结束
//	ADC_SoftwareStartConvCmd(ADCx, ENABLE);		//使能指定的ADC1的软件转换启动功能
}

/**
	* @brief	获得ADC值
	* @param  ch: adc通道
	* @retval None
	*/
u16 Get_Adc_val(void)
{
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_7Cycles5);	//ADC1,ADC通道,规则通道使用数量,采样时间
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);					//使能指定的ADC1的软件转换启动功能
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC));	//等待转换结束
	return ADC_GetConversionValue(ADCx);						//返回最近一次ADC1规则组的转换结果
}

/**
	* @brief	获得平均ADC值
	* @param  ch: adc通道
						times: 次数
	* @retval None 
	*/
u16 Get_Adc_Val_Average(u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for(t = 0; t < times; t++)
	{
		temp_val += Get_Adc_val();
		Delay_Ms(5);
	}
	return temp_val / times;
}

/**
	* @brief	获取平均电池电量（4.2V则返回420）
	* @param  ch: adc通道
						times: 次数
	* @retval None 
	*/
u8 Get_Adc_BAT_Percent(u8 times)
{
	u32 temp_val = 0;
	u16 BAT = 0;
	u8 t;
	for(t = 0; t < times; t++)
	{
		temp_val += Get_Adc_val();
//		Delay_Ms(5);
	}
	temp_val = (temp_val / times) * 330 * 2 / 4096;
	if(temp_val > 420)BAT = 99;
	else if(temp_val > 406)BAT = 90;
	else if(temp_val > 398)BAT = 80;
	else if(temp_val > 392)BAT = 70;
	else if(temp_val > 387)BAT = 60;
	else if(temp_val > 382)BAT = 50;
	else if(temp_val > 379)BAT = 40;
	else if(temp_val > 377)BAT = 30;
	else if(temp_val > 374)BAT = 20;
	else if(temp_val > 368)BAT = 10;
	else if(temp_val > 345)BAT = 5;
	else BAT = 0;
	return BAT;
}






