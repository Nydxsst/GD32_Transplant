#include "si24r1.h"
#include "delay.h"


/* 外部复用参量 --------------------------------------------------------------*/
u8 NRF24L_Tx_Rx_Sta = 0; 			//IRF24L01 产生的中断类型
u8 NRF24L_Rx_Buf[RX_PLOAD_WIDTH]; //IRF24L01 数据接收缓存区
u8 NRF24L_Tx_Buf[TX_PLOAD_WIDTH];

/* 内部使用参量 --------------------------------------------------------------*/
/* 收发地址 */
const u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x02};
const u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x02};

/* 内部调用函数 --------------------------------------------------------------*/
static void NRF24L01_Exti_Init(void);
u8 NRF24L01_Write_Buf(u8 reg, u8* pBuf, u8 u8s);//写数据区
u8 NRF24L01_Read_Buf(u8 reg, u8* pBuf, u8 u8s);	//读数据区
u8 NRF24L01_Read_Reg(u8 reg);										//读寄存器
u8 NRF24L01_Write_Reg(u8 reg, u8 value);				//写寄存器


/**
	* @brief	初始化24L01
	* @param  None
	* @retval None
	*/
void NRF24L01_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(NRF24L01_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_PIN;
	GPIO_Init(NRF24L01_CE_GPIO, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CSN_PIN;
	GPIO_Init(NRF24L01_CSN_GPIO, &GPIO_InitStructure);
	NRF24L01_CE = 0; 		//NRF使能
	NRF24L01_CSN = 1;		//SPI片选取消
	
	NRF24L01_Exti_Init();//中断线使能
	NRF24L01_SPI_Init();//初始化SPI

	while(NRF24L01_Check())
	{
		printf("NRF24L01 Error\r\n");
		Delay_Ms(500);
	}
	printf("NRF24L01 OK\r\n");
	NRF24L01_RX_Mode();
	
	//NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_AW, 0x03); //配置通信地址的长度，默认值时0x03,即地址长度为5字节
	NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (u8*)RX_ADDRESS, RX_ADR_WIDTH); //设置RX节点地址（自动应答地址）,主要为了使能ACK
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址  
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01); //使能通道0自动应答
	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度  
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40); //设置RF通道为40hz(1-64Hz都可以)
	
	NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, (u8*)TX_ADDRESS, TX_ADR_WIDTH);
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a); //设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次 0x1A
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益关闭 （注意：低噪声增益关闭/开启直接影响通信,要开启都开启，要关闭都关闭0x0f）
	
	NRF24L01_RX_Mode(); //默认为接收模式
}
//检测24L01是否存在
//返回值:0，成功;1，失败
u8 NRF24L01_Check(void)
{
	u8 buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
	u8 i;
	NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, buf, 5); //写入5个字节的地址.
	NRF24L01_Read_Buf(TX_ADDR, buf, 5); //读出写入的地址
	for(i = 0; i < 5; i++)if(buf[i] != 0XA5)break;
	if(i != 5)return 1; //检测24L01错误
	return 0;		 //检测到24L01
}
//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
u8 NRF24L01_Write_Reg(u8 reg, u8 value)
{
	u8 status;
	NRF24L01_CSN = 0;               //使能SPI传输
	status = NRF24L01_SPI_ReadWriteByte(reg); //发送寄存器号
	NRF24L01_SPI_ReadWriteByte(value);      //写入寄存器的值
	NRF24L01_CSN = 1;               //禁止SPI传输
	return(status);       			//返回状态值
}
//读取SPI寄存器值
//reg:要读的寄存器
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;
	NRF24L01_CSN = 0;          //使能SPI传输
	NRF24L01_SPI_ReadWriteByte(reg);   //发送寄存器号
	reg_val = NRF24L01_SPI_ReadWriteByte(0XFF); //读取寄存器内容
	NRF24L01_CSN = 1;          //禁止SPI传输
	return(reg_val);           //返回状态值
}
//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Read_Buf(u8 reg, u8* pBuf, u8 len)
{
	u8 status, u8_ctr;
	NRF24L01_CSN = 0;           //使能SPI传输
	status = NRF24L01_SPI_ReadWriteByte(reg); //发送寄存器值(位置),并读取状态值
	for(u8_ctr = 0; u8_ctr < len; u8_ctr++)pBuf[u8_ctr] = NRF24L01_SPI_ReadWriteByte(0XFF); //读出数据
	NRF24L01_CSN = 1;     //关闭SPI传输
	return status;        //返回读到的状态值
}
//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8* pBuf, u8 len)
{
	u8 status, u8_ctr;
	NRF24L01_CSN = 0;          //使能SPI传输
	status = NRF24L01_SPI_ReadWriteByte(reg);//发送寄存器值(位置),并读取状态值
	for(u8_ctr = 0; u8_ctr < len; u8_ctr++)NRF24L01_SPI_ReadWriteByte(*pBuf++); //写入数据
	NRF24L01_CSN = 1;       //关闭SPI传输
	return status;          //返回读到的状态值
}
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
void NRF24L01_TxPacket(u8* txbuf)
{
	NRF24L01_CE = 0;
	NRF24L01_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); //写数据到TX BUF  32个字节
	NRF24L01_CE = 1; //启动发送
	Delay_Us(10);  //CE持续高电平10us
}

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE = 0;
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0f);
	NRF24L01_CE = 1; //CE为高,进入接收模式
	Delay_Us(200);
}

//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了
//CE为高大于10us,则启动发送.
void NRF24L01_TX_Mode(void)
{
	NRF24L01_CE = 0;
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);
	NRF24L01_CE = 1; //CE为高,10us后启动发送
}
/**
	* @brief	初始化外部中断
	* @param  None
	* @retval None
	*/
static void NRF24L01_Exti_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* GPIO_Init */
	RCC_APB2PeriphClockCmd(NRF24L01_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = NRF24L01_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = NRF24L01_GPIO_MODE;
	GPIO_Init(NRF24L01_GPIO_PORT, &GPIO_InitStructure);
	/* NVIC_Init */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = NRF24L01_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NRF24L01_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NRF24L01_SubPriority;
	NVIC_Init(&NVIC_InitStructure);
	/* EXTI_Init */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	GPIO_EXTILineConfig(NRF24L01_EXTI_PORTSOURCE, NRF24L01_EXTI_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = NRF24L01_EXTI_LINE;
	EXTI_InitStructure.EXTI_Trigger = NRF24L01_EXTI_Trigger;
	EXTI_Init(&EXTI_InitStructure);
}

/**
	* @brief	中断服务函数
	* @param  None
	* @retval None
	*/
void NRF24L01_EXTI_IRQHandler(void)
{
	u8 sta;

	if(EXTI_GetITStatus(NRF24L01_EXTI_LINE) != RESET)
	{
		sta = NRF24L01_Read_Reg(STATUS); //读取状态寄存器的值
		if(sta & MAX_TX) //达到最大重发次数
		{
			NRF24L_Tx_Rx_Sta = MAX_TX;
			NRF24L01_Write_Reg(FLUSH_TX, 0xff); //清除TX FIFO寄存器
		}
		else if(sta & TX_OK) //发送完成
		{
			NRF24L_Tx_Rx_Sta = TX_OK;
			NRF24L01_RX_Mode();
		}
		else if(sta & RX_OK) //接收完成
		{
			NRF24L_Tx_Rx_Sta = RX_OK;
			NRF24L01_Read_Buf(RD_RX_PLOAD, NRF24L_Rx_Buf, RX_PLOAD_WIDTH); //读取数据
			NRF24L01_Write_Reg(FLUSH_RX, 0xff); //清除RX FIFO寄存器
			//printf("Rx: %d%d\r\n", NRF24L_Rx_Buf[0], NRF24L_Rx_Buf[1]);
			//printf("Rx: %d%d\r\n", NRF24L_Rx_Buf[0], NRF24L_Rx_Buf[1]);
			//printf("Rx: %s\r\n", NRF24L_Rx_Buf);
		}
		NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta); //清除中断标志
		EXTI_ClearITPendingBit(NRF24L01_EXTI_LINE);
	}
}
