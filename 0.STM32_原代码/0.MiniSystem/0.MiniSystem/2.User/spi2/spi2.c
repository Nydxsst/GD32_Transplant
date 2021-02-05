#include "spi2.h"

/**
	* @brief	初始化SPI2
	* @param  None
	* @retval None
	*/
void SPI2_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );		//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );		//SPI2时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  					//PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);										//初始化GPIOB
 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;													//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;												//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//定义波特率预分频的值，APB1预分频而来（72M）
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									//指定数据传输从MSB位还是LSB位开始:
	SPI_InitStructure.SPI_CRCPolynomial = 7;														//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  																//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); 		//使能SPI外设
	SPI2_ReadWriteByte(0xff);	//启动传输
}

/**
	* @brief	设置SPI2的速度
	* @param  SPI_BaudRatePrescaler：SPI_BaudRatePrescaler_2   2分频   
																	 SPI_BaudRatePrescaler_8   8分频   
																	 SPI_BaudRatePrescaler_16  16分频  
																	 SPI_BaudRatePrescaler_256 256分频 
	* @retval None
	*/
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI2,ENABLE); 
}

/**
	* @brief	设置SPI2的速度
	* @param  TxData:要写入的字节
	* @retval 读取到的字节
	*/
u8 SPI2_ReadWriteByte(u8 TxData)
{
	while((SPI2->SR&SPI_I2S_FLAG_TXE) == RESET);//等待发送区空	  
	SPI2->DR = TxData;	 	//发送一个byte
	while((SPI2->SR&SPI_I2S_FLAG_RXNE) == RESET);
	return SPI2->DR; //返回通过SPIx最近接收的数据					    
}
