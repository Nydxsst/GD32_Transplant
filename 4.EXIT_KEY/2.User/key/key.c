#include "key.h"

/**
  * @brief  Configure Key in interrupt mode
  * @param  None
  * @retval None
  */
void Key_Init(void)
{
	/* enable the key wakeup clock */
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_AF);

	/* configure button pin as input */
	gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_0);

	/* enable and set key wakeup EXTI interrupt to the lowest priority */
	nvic_irq_enable(EXTI0_IRQn, 2U, 0U);

	/* connect key wakeup EXTI line to key GPIO pin */
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_0);

	/* configure key wakeup EXTI line */
	exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	exti_interrupt_flag_clear(EXTI_0);
}


/**
  * @brief  This function handles External line interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	/* check the key wakeup is pressed or not */
	if(RESET != exti_interrupt_flag_get(EXTI_0))
	{
		if(PAin(0) == 0)
		{
			SYSTEM_LED = !SYSTEM_LED;
		}
		exti_interrupt_flag_clear(EXTI_0);
	}
}
