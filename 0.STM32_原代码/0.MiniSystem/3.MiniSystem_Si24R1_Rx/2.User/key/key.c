#include "key.h"

/**
  * @brief  Configure Key in interrupt mode
  * @param  None
  * @retval None
  */
void Key_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(KEY1_CLK | KEY2_CLK | KEY3_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Mode = KEY1_MODE;
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
	GPIO_Init(KEY1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = KEY2_MODE;
	GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
	GPIO_Init(KEY2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = KEY3_MODE;
	GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
	GPIO_Init(KEY3_PORT, &GPIO_InitStructure);
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
