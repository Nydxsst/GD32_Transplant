#include "adc.h"

/**
	* @brief	��ʼ��ADC(��ʹ�ù���ͨ��)
	* @param  None
	* @retval None
	*/
void Adc_Init(void)
{
	//��λADC
	adc_deinit(ADCx);
	//ʹ��ADC��IO�ڵ�ʱ��
	rcu_periph_clock_enable(ADC_GPIO_CLK);
	//ʹ��ADC��ʱ��
	rcu_periph_clock_enable(ADC_CLK);
	//����ADCʱ�ӵķ�Ƶ
	rcu_adc_clock_config(ADC_CLK_DIV);
	
	//��ʼ��ADC��GPIO
	gpio_init(ADC_GPIO_PORT, GPIO_MODE_AIN, GPIO_OSPEED_MAX, ADC_GPIO_PIN);
	
	//����ADC�����ڶ���ģʽ
	adc_mode_config(ADC_MODE_FREE);
	//ADC���ݶ��뷽ʽ:LSB ����
	adc_data_alignment_config(ADCx, ADC_DATAALIGN_RIGHT);
	//����ͨ����ĳ���Ϊ1
	adc_channel_length_config(ADCx, ADC_REGULAR_CHANNEL, 1U);

	//�������
	adc_external_trigger_source_config(ADCx, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE); 
	//ʹ�ܹ���ͨ����Ĵ���
	adc_external_trigger_config(ADCx, ADC_REGULAR_CHANNEL, ENABLE);

	//ʹ��ADC
	adc_enable(ADCx);
	Delay_Ms(1);
	//ʹ��ADCУ׼
	adc_calibration_enable(ADCx);
}

/**
	* @brief	���ADCֵ
	* @param  ch: adcͨ��
	* @retval None
	*/
u16 Get_Adc_val(void)
{
	//����ADC��ͨ�������ʱ��
	adc_regular_channel_config(ADC0, 0U, ADC_CHANNEL, ADC_SAMPLETIME_7POINT5);
	//�������ADC
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);

	//�ȴ�ADC�������
	while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
	//�����־λ
	adc_flag_clear(ADC0, ADC_FLAG_EOC);
	//����ADC����
	return (adc_regular_data_read(ADC0));
}

/**
	* @brief	���ƽ��ADCֵ
	* @param  ch: adcͨ��
						times: ����
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
	* @brief	��ȡƽ����ص�����4.2V�򷵻�420��
	* @param  ch: adcͨ��
						times: ����
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






