#include "adc.h"

/**
	* @brief	初始化ADC(仅使用规则通道)
	* @param  None
	* @retval None
	*/
void Adc_Init(void)
{
	//复位ADC
	adc_deinit(ADCx);
	//使能ADC的IO口的时钟
	rcu_periph_clock_enable(ADC_GPIO_CLK);
	//使能ADC的时钟
	rcu_periph_clock_enable(ADC_CLK);
	//设置ADC时钟的分频
	rcu_adc_clock_config(ADC_CLK_DIV);
	
	//初始化ADC的GPIO
	gpio_init(ADC_GPIO_PORT, GPIO_MODE_AIN, GPIO_OSPEED_MAX, ADC_GPIO_PIN);
	
	//所有ADC运行于独立模式
	adc_mode_config(ADC_MODE_FREE);
	//ADC数据对齐方式:LSB 对齐
	adc_data_alignment_config(ADCx, ADC_DATAALIGN_RIGHT);
	//规则通道组的长度为1
	adc_channel_length_config(ADCx, ADC_REGULAR_CHANNEL, 1U);

	//软件触发
	adc_external_trigger_source_config(ADCx, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE); 
	//使能规则通道组的触发
	adc_external_trigger_config(ADCx, ADC_REGULAR_CHANNEL, ENABLE);

	//使能ADC
	adc_enable(ADCx);
	Delay_Ms(1);
	//使能ADC校准
	adc_calibration_enable(ADCx);
}

/**
	* @brief	获得ADC值
	* @param  ch: adc通道
	* @retval None
	*/
u16 Get_Adc_val(void)
{
	//配置ADC的通道与采样时间
	adc_regular_channel_config(ADC0, 0U, ADC_CHANNEL, ADC_SAMPLETIME_7POINT5);
	//软件触发ADC
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);

	//等待ADC采样完成
	while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
	//清除标志位
	adc_flag_clear(ADC0, ADC_FLAG_EOC);
	//返回ADC读数
	return (adc_regular_data_read(ADC0));
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






