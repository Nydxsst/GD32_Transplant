#include "usart_debug.h"

/**
	* @brief	初始化USART
	* @param  BaudRate
	* @retval None
	*/
void Usart_Debug_Init(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	/* Usart GPIO 配置 */
	//USART_TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Usart NVIC 配置 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

	/* USART 初始化设置 */
	USART_InitStructure.USART_BaudRate = BaudRate;																		//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;															//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//收发模式

	USART_Init(USART1, &USART_InitStructure); 		//初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口
}

/**
	* @brief	USART1中断服务函数
	* @param  None
	* @retval None
	*/
void USART1_IRQHandler(void)
{
	uint8_t ucTemp;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		ucTemp = USART_ReceiveData(USART1);
		USART_SendData(USART1, ucTemp);
	}
}

/**
	* @brief	重定向c库函数，使得printf.scanf.put.get可以使用
	* @param  None
	* @retval None
	*/
//标准库需要的支持函数
struct __FILE
{
	int handle;

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
//重定义fputc函数
int fputc(int ch, FILE* f)
{
	while((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕
	USART1->DR = (u8) ch;
	return ch;
}

