#include "key.h"
#include "delay.h"

/**
  * @brief  Configure Key in interrupt mode
  * @param  None
  * @retval None
  */
void Key_Init(void)
{
	rcu_periph_clock_enable(KEY1_CLK);
	gpio_init(KEY1_PORT, KEY1_MODE, GPIO_OSPEED_50MHZ, KEY1_PIN);
	rcu_periph_clock_enable(KEY2_CLK);
	gpio_init(KEY2_PORT, KEY2_MODE, GPIO_OSPEED_50MHZ, KEY2_PIN);
	rcu_periph_clock_enable(KEY3_CLK);
	gpio_init(KEY3_PORT, KEY3_MODE, GPIO_OSPEED_50MHZ, KEY3_PIN);
}


/**
  * @brief
  * @param  None
  * @retval 0/KEY1_PRES/KEY2_PRES/KEY3_PRES
  */
u8 KEY_Scan(void)
{
	static u8 key_up = 1;//按键按松开标志
	if(key_up && (KEY1_READ == 0 || KEY2_READ == 0 || KEY3_READ == 0))
	{
		Delay_Ms(10);//去抖动
		key_up = 0;
		if(KEY1_READ == 0)return KEY1_PRES;
		else if(KEY2_READ == 0)return KEY2_PRES;
		else if(KEY3_READ == 0)return KEY3_PRES;
	}
	else if(KEY1_READ == 1 && KEY2_READ == 1 && KEY3_READ == 1)key_up = 1;
	return 0;// 无按键按下
}
