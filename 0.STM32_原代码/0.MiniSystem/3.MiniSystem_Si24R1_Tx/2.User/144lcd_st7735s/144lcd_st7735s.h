/**
  ******************************************************************************
  * @author  
  * @version V1.0.0
  * @date    2020/11/3
  ******************************************************************************
	* @attention 修改IO口时要同时修改"ST7735S_CPT144.c"文件里的内部宏定义
  ******************************************************************************
  */
	
#ifndef __144TFT_ST7735S_H
#define __144TFT_ST7735S_H

#include "stm32f10x.h"
#include "fonts.h"
#include "spi2.h"


/* ST7735S显示区域的起始坐标和总行列数 ---------------------------------------*/
#define ST7735S_CPT144_DispWindow_X_Star	0
#define ST7735S_CPT144_DispWindow_Y_Star	0
#define ST7735S_CPT144_Height 128
#define ST7735S_CPT144_Width  128

/* ST7735S常用颜色 -----------------------------------------------------------*/
#define ST7735S_CPT144_RED    0xf800
#define ST7735S_CPT144_GREEN  0x07e0
#define ST7735S_CPT144_BLUE   0x001f
#define ST7735S_CPT144_YELLOW 0xffe0
#define ST7735S_CPT144_WHITE  0xffff
#define ST7735S_CPT144_BLACK  0x0000
#define ST7735S_CPT144_PURPLE 0xf81f

/* ST7735S操作线 -------------------------------------------------------------*/
/* SPI选择 */
#define ST7735S_SPI_Init						SPI2_Init
#define ST7735S_SPI_ReadWriteByte		SPI2_ReadWriteByte
/* IO引脚定义 */
#define ST7735S_CPT144_A0_SCK     RCC_APB2Periph_GPIOB
#define ST7735S_CPT144_A0_PORT    GPIOB
#define ST7735S_CPT144_A0_PIN     GPIO_Pin_12
#define ST7735S_CPT144_CS_SCK			RCC_APB2Periph_GPIOB
#define ST7735S_CPT144_CS_PORT		GPIOB
#define ST7735S_CPT144_CS_PIN			GPIO_Pin_11
#define ST7735S_CPT144_RES_SCK    RCC_APB2Periph_GPIOA
#define ST7735S_CPT144_RES_PORT   GPIOA
#define ST7735S_CPT144_RES_PIN    GPIO_Pin_8
#define ST7735S_CPT144_BLK_SCK    RCC_APB2Periph_GPIOB
#define ST7735S_CPT144_BLK_PORT   GPIOB
#define ST7735S_CPT144_BLK_PIN    GPIO_Pin_10
#define ST7735S_CPT144_BLK				PBout(10)


/* 外部调用函数 --------------------------------------------------------------*/
void ST7735S_CPT144_Init(void);
void ST7735S_CPT144_OpenWindow(u8 X, u8 Y, u8 Width, u8 Height);
void ST7735S_CPT144_Clear(void);
void ST7735S_CPT144_DispChar(u8 X, u8 Y, const char cChar);
void ST7735S_CPT144_DispString_XDir(u16 X, u16 Y, char* pStr);
void ST7735S_CPT144_DispString_YDir(u16 X, u16 Y, char* pStr);

void ST7735S_CPT144_SetFont(sFONT* fonts);
void ST7735S_CPT144_SetTextColor(uint16_t Color);
void ST7735S_CPT144_SetBackColor(uint16_t Color);
void ST7735S_CPT144_displayColor(u16 color);
void ST7735S_CPT144_DispChinese(u16 X, u16 Y, u8* Word, u8 Word_width, u8 Word_height);
void ST7735S_CPT144_DispPicture(u8* picture);

#endif
