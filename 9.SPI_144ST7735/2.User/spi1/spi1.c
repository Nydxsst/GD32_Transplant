#include "spi1.h"

void SPI1_GPIO_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_SPI1);
//	rcu_periph_clock_enable(RCU_GPIOA);
//	rcu_periph_clock_enable(RCU_SPI0);
	
//	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7 | GPIO_PIN_5);
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_15);
}

/**
	* @brief	初始化SPI1
	* @param  None
	* @retval None
	*/
void SPI1_Init(void)
{
	//初始化结构体
	spi_parameter_struct  spi_init_struct;
	//初始化SPI1的IO
	SPI1_GPIO_Init();
	//复位SPI1
	spi_i2s_deinit(SPI1);
//	spi_i2s_deinit(SPI0);
	//初始化结构体参数
	spi_struct_para_init(&spi_init_struct);
	
	/* SPI1 parameter config */
	spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX; 	//设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	spi_init_struct.device_mode          = SPI_MASTER;								//设置SPI工作模式:设置为主SPI
	spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;				//设置SPI的数据大小:SPI发送接收8位帧结构
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;		//串行同步时钟的空闲状态为低电平,第一个跳变沿（上升或下降）数据被采样
	spi_init_struct.nss                  = SPI_NSS_SOFT;							//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理
	spi_init_struct.prescale             = SPI_PSC_2;									//定义波特率预分频的值
	spi_init_struct.endian               = SPI_ENDIAN_MSB;						//指定数据传输从MSB位还是LSB位开始
	spi_init(SPI1, &spi_init_struct);				//初始化结构体
	spi_nss_output_disable(SPI1);						//关闭nss引脚的功能输出
	spi_bidirectional_transfer_config(SPI1,SPI_BIDIRECTIONAL_TRANSMIT);  //只工作在发送模式
	spi_enable(SPI1);						//使能SPI外设
	
//	spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX; 	//设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
//	spi_init_struct.device_mode          = SPI_SLAVE;								//设置SPI工作模式:设置为主SPI
//	spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;				//设置SPI的数据大小:SPI发送接收8位帧结构
//	spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;		//串行同步时钟的空闲状态为高电平,第二个跳变沿（上升或下降）数据被采样
//	spi_init_struct.nss                  = SPI_NSS_SOFT;							//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理
//	spi_init_struct.prescale             = SPI_PSC_8;									//定义波特率预分频的值
//	spi_init_struct.endian               = SPI_ENDIAN_MSB;						//指定数据传输从MSB位还是LSB位开始
//	spi_init(SPI0, &spi_init_struct);				//初始化结构体
//	spi_nss_output_disable(SPI0);						//关闭nss引脚的功能输出
//	spi_enable(SPI0);						//使能SPI外设
	
	
	SPI1_ReadWriteByte(0xff);		//启动传输
	printf("SPI ok!\n");
}


/**
	* @brief	设置SPI2的速度
	* @param  TxData:要写入的字节
	* @retval 读取到的字节
	*/
u8 SPI1_ReadWriteByte(u8 TxData)
{
	while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE)){}
  spi_i2s_data_transmit(SPI1, TxData);
//	while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE)){}
//	printf("result:%d\n",spi_i2s_data_receive(SPI0));
	while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE)){}
	return spi_i2s_data_receive(SPI1);			    
}
