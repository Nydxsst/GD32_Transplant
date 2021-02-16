/**
  ******************************************************************************
  * @author  Moment/Nydxsst
  * @version V1.0.0
  * @date    2021/02/12
  ******************************************************************************
	* @attention �޸�IO��ʱҪͬʱ�޸�"ST7735S_CPT144.c"�ļ�����ڲ��궨��
  ******************************************************************************
  */
	
#ifndef __144TFT_ST7735S_H
#define __144TFT_ST7735S_H

#include "gd32e10x.h"
#include "fonts.h"
#include "spi1.h"


/* ST7735S��ʾ�������ʼ������������� ---------------------------------------*/
#define ST7735S_CPT144_DispWindow_X_Star	0
#define ST7735S_CPT144_DispWindow_Y_Star	0
#define ST7735S_CPT144_Height 128
#define ST7735S_CPT144_Width  128

/* ST7735S������ɫ -----------------------------------------------------------*/
#define ST7735S_CPT144_RED    0xf800
#define ST7735S_CPT144_GREEN  0x07e0
#define ST7735S_CPT144_BLUE   0x001f
#define ST7735S_CPT144_YELLOW 0xffe0
#define ST7735S_CPT144_WHITE  0xffff
#define ST7735S_CPT144_BLACK  0x0000
#define ST7735S_CPT144_PURPLE 0xf81f

/* ST7735S������ -------------------------------------------------------------*/
/* SPIѡ�� */
#define ST7735S_SPI_Init						SPI1_Init
#define ST7735S_SPI_ReadWriteByte		SPI1_ReadWriteByte

/* IO���Ŷ��� */
#define ST7735S_CPT144_A0_SCK     RCU_GPIOB
#define ST7735S_CPT144_A0_PORT    GPIOB
#define ST7735S_CPT144_A0_PIN     GPIO_PIN_12

#define ST7735S_CPT144_CS_SCK     RCU_GPIOB
#define ST7735S_CPT144_CS_PORT    GPIOB
#define ST7735S_CPT144_CS_PIN     GPIO_PIN_11

#define ST7735S_CPT144_RES_SCK    RCU_GPIOA
#define ST7735S_CPT144_RES_PORT   GPIOA
#define ST7735S_CPT144_RES_PIN    GPIO_PIN_8

#define ST7735S_CPT144_BLK_SCK    RCU_GPIOB
#define ST7735S_CPT144_BLK_PORT   GPIOB
#define ST7735S_CPT144_BLK_PIN    GPIO_PIN_10
#define ST7735S_CPT144_BLK				PBout(10)


/* �ⲿ���ú��� --------------------------------------------------------------*/
void ST7735S_CPT144_Init(void);
void ST7735S_CPT144_OpenWindow(u8 X, u8 Y, u8 Width, u8 Height); //���ô���
void ST7735S_CPT144_Clear(void);
void ST7735S_CPT144_DispChar(u8 usX, u8 Y, const char cChar); //��ʾһ��Ӣ���ַ�
void ST7735S_CPT144_DispString_XDir(u16 X, u16 Y, char* pStr); //��ʾӢ���ַ���(��X�᷽��)
void ST7735S_CPT144_DispString_YDir(u16 X, u16 Y, char* pStr); //��ʾӢ���ַ���(��y�᷽��)

void ST7735S_CPT144_SetFont(sFONT* fonts); //����Ӣ����������
void ST7735S_CPT144_SetTextColor(uint16_t Color); //����LCD��ǰ��(����)��ɫ
void ST7735S_CPT144_SetBackColor(uint16_t Color); //����LCD�ı�����ɫ
void ST7735S_CPT144_displayColor(u16 color); //ȫ����ʾĳ����ɫ
void ST7735S_CPT144_DispChinese(u16 X, u16 Y, u8* Word, u8 Word_width, u8 Word_height); //��ʾ�Զ����С������/ͼƬ
void ST7735S_CPT144_DispPicture(u8* picture); //��ʾȫ��ͼƬ

#endif
