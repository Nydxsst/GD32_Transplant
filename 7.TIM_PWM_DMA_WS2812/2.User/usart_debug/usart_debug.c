#include "usart_debug.h"
#include <stdio.h>

/**
	* @brief	初始化USART
	* @param  BaudRate
	* @retval None
	*/
void Usart_Debug_Init(u32 BaudRate)
{
	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);
	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART0);
	/* connect port to USARTx_Tx */
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	/* connect port to USARTx_Rx */
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	/* USART interrupt configuration */
	nvic_irq_enable(USART0_IRQn, 0, 0);
	/* USART configure */
	usart_deinit(USART0);
	usart_baudrate_set(USART0, BaudRate);
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	usart_interrupt_enable(USART0, USART_INT_RBNE);
	usart_enable(USART0);
}

/**
	* @brief	USART中断服务函数
	* @param  None
	* @retval None
	*/
void USART0_IRQHandler(void)
{
	uint8_t ucTemp;
	if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE))
	{
		ucTemp = usart_data_receive(USART0);
		usart_data_transmit(USART0, ucTemp);
	}
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE* f)
{
	usart_data_transmit(USART0, (uint8_t)ch);
	while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
	return ch;
}
