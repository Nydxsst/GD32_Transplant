/**
  ******************************************************************************
  * @author  
  * @version V1.0.1
  * @date    2020/04/07
  ******************************************************************************
  * @attention
	*
  * ---------------------------- 驱动使用说明 ---------------------------------*
  * 需要用到的引脚：3个SPI,CE，CSN引脚（宏定义处可修改）,IRQ中断引脚
  *								 （除了修改宏定义还要改exti.c文件中的外部中断服务函数名称和函
  *									 数里清除中断标志的中断线）
  * 驱动过程：1.extern u8 Rxbuf[]（数据接收缓存区）
  *							extern u8 TX_RX_status
  *						 （产生的中断类型：接收成功TX_OK，发送成功RX_OK，
  *							发送失败MAX_TX‘即达到最大重发次数’”）
  *						2.初始化nrf2401（NRF24L01_Init）和外部中断（Exti_Init）
  *						3.使能发送模式（NRF24L01_RX_Mode）或接受模式（NRF24L01_TX_Mode）
  *						4.发送模式：调用NRF24L01_TxPacket发送
  *							接受模式：接收到的数据将保存在里Rxbuf[]
  *
  * ---------------------------- 基本功能介绍 ---------------------------------*
  * 有126个可选的频道
  * 可实现1对6的通讯，1收6发，每一个通道地址不同但是共用同个地址
  * 通道0可以配置40位自身地址，1~5具有32位高位共用地址
  *											（通道1设置）和8位自身地址
  * 最高工作速率2Mpbs
  * 具有内部CRC
  * 具有应答
  * 可低速写入，高速发送
  * 中断（中断时产生低电平）：发送完成并且接收到应答信号
  *												接收到数据
  *												达到最大重发次数
  * 两种模式：Enhanced ShockBurstTM:有应答信号，支持自动重发
  *				ShockBurstTM				 :无应答信号，不支持自动重发
  * 发送数据：数据写入寄存器时，CE位置高至少10us发射
  * 通信频率：2400+RF_CH(0~125Mhz)
  * 使能接收模式时CE位置高
  *
  * 通道0.1认数据接收使能，2~5默认为0，不使能数据接收
  * 发送地址填入时从低位写起、
  *
  * 尚未实现功能：1对6通信
  *								 DMA数据
	*
  ******************************************************************************
  */

#ifndef __NRF24L01_H
#define __NRF24L01_H

#include "stm32f10x.h"
#include "spi1.h"

/* 外部调用（勿改动）---------------------------------------------------------*/
#define MAX_TX  				0x10  //达到最大发送次数中断
#define TX_OK   				0x20  //TX发送完成中断
#define RX_OK   				0x40  //接收到数据中断				

/* 指令集 --------------------------------------------------------------------*/
/* NRF24L01寄存器操作命令 */
#define NRF_READ_REG    0x00  //读配置寄存器,低5位为寄存器地址
#define NRF_WRITE_REG   0x20  //写配置寄存器,低5位为寄存器地址（加上寄存器地址成为最终的写入地址）
#define RD_RX_PLOAD     0x61  //读RX有效数据,1~32字节
#define WR_TX_PLOAD     0xA0  //写TX有效数据,1~32字节
#define FLUSH_TX        0xE1  //清除TX FIFO寄存器.发射模式下用
#define FLUSH_RX        0xE2  //清除RX FIFO寄存器.接收模式下用
#define REUSE_TX_PL     0xE3  //重新使用上一包数据,CE为高,数据包被不断发送.
#define NOP             0xFF  //空操作,可以用来读状态寄存器	 
/* SPI(NRF24L01)寄存器地址 */
#define CONFIG          0x00  //配置寄存器地址;bit0:1接收模式,0发射模式;bit1:电选择;bit2:CRC模式;bit3:CRC使能;
															//bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使能;bit6:中断RX_DR使能
#define EN_AA           0x01  //使能自动应答功能  bit0~5,对应通道0~5
#define EN_RXADDR       0x02  //接收地址允许,bit0~5,对应通道0~5
#define SETUP_AW        0x03  //设置地址宽度(所有数据通道):bit1,0:00,3字节;01,4字节;02,5字节;
#define SETUP_RETR      0x04  //建立自动重发;bit3:0,自动重发计数器;bit7:4,自动重发延时 250*x+86us
#define RF_CH           0x05  //RF通道,bit6:0,工作通道频率;
#define RF_SETUP        0x06  //RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益
#define STATUS          0x07  //状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);bit4,达到最多次重发
															//bit5:数据发送完成中断;bit6:接收数据中断
#define OBSERVE_TX      0x08  //发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器
#define CD              0x09  //载波检测寄存器,bit0,载波检测;
#define RX_ADDR_P0      0x0A  //数据通道0接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P1      0x0B  //数据通道1接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P2      0x0C  //数据通道2接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P3      0x0D  //数据通道3接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P4      0x0E  //数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P5      0x0F  //数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define TX_ADDR         0x10  //发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
#define RX_PW_P0        0x11  //接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P1        0x12  //接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P2        0x13  //接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P3        0x14  //接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P4        0x15  //接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P5        0x16  //接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define NRF_FIFO_STATUS 0x17  //FIFO状态寄存器;bit0,RX FIFO寄存器空标志;bit1,RX FIFO满标志;bit2,3,保留
															//bit4,TX FIFO空标志;bit5,TX FIFO满标志;bit6,1,循环发送上一数据包.0,不循环

/* 内部定量（根据需求修改） --------------------------------------------------*/
/* 24L01发送接收数据宽度定义 */
#define TX_PLOAD_WIDTH  32  	//32字节的用户数据宽度（最大32）
#define RX_PLOAD_WIDTH  32  	//32字节的用户数据宽度（最大32）
#define TX_ADR_WIDTH    5   	//5字节的地址宽度
#define RX_ADR_WIDTH    5   	//5字节的地址宽度	

/* 24L01操作线 ---------------------------------------------------------------*/
/* SPI选择 */
#define NRF24L01_SPI_Init						SPI1_Init
#define NRF24L01_SPI_ReadWriteByte	SPI1_ReadWriteByte
/* IO引脚定义 */
#define NRF24L01_CLOCK					RCC_APB2Periph_GPIOA
#define NRF24L01_CE_PIN					GPIO_Pin_2
#define NRF24L01_CSN_PIN				GPIO_Pin_3
#define NRF24L01_CE_GPIO				GPIOA
#define NRF24L01_CSN_GPIO				GPIOA
#define NRF24L01_CE							PAout(2) //用来启动发送或启动接收
#define NRF24L01_CSN						PAout(3) //SPI片选信号
/* 中断输入引脚定义 */
#define NRF24L01_GPIO_CLK          		(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define NRF24L01_GPIO_PORT         		GPIOA
#define NRF24L01_GPIO_PIN          		GPIO_Pin_4
#define NRF24L01_GPIO_MODE						GPIO_Mode_IPU
#define NRF24L01_EXTI_PORTSOURCE   		GPIO_PortSourceGPIOA
#define NRF24L01_EXTI_PINSOURCE    		GPIO_PinSource4
#define NRF24L01_EXTI_LINE         		EXTI_Line4
#define NRF24L01_EXTI_Trigger					EXTI_Trigger_Falling
#define NRF24L01_EXTI_IRQ          		EXTI4_IRQn
#define NRF24L01_EXTI_IRQHandler			EXTI4_IRQHandler
#define NRF24L01_PreemptionPriority		1
#define NRF24L01_SubPriority					1


/* 外部调用函数 --------------------------------------------------------------*/
void NRF24L01_Init(void);						//初始化
u8 NRF24L01_Check(void);						//检查24L01是否存在
void NRF24L01_RX_Mode(void);				//配置为接收模式
void NRF24L01_TX_Mode(void);				//配置为发送模式
void NRF24L01_TxPacket(u8* txbuf);		//发送一个包的数据


#endif
