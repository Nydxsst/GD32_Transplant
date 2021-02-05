#include "key.h"

/**
  * @brief  Configure Key in interrupt mode
  * @param  None
  * @retval None
  */
void Key_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	
  /* GPIO_Init */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* NVIC_Init */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//子优先级
	NVIC_Init(&NVIC_InitStructure);
	
	/* Exti_Init */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_Init(&EXTI_InitStructure);
}


/**
  * @brief  This function handles External line interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
		if(PAin(0) == 0)
		{
			SYSTEM_LED = !SYSTEM_LED;
		}
    EXTI_ClearITPendingBit(EXTI_Line0);//Clear the EXTI line pending bit
  }
}
