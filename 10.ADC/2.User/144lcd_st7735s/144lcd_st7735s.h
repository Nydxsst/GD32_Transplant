/**
  ******************************************************************************
  * @author  Moment/Nydxsst
  * @version V1.0.0
  * @date    2021/02/12
  ******************************************************************************
	* @attention 修改IO口时要同时修改"ST7735S_CPT144.c"文件里的内部宏定义
  ******************************************************************************
  */
	
#ifndef __144TFT_ST7735S_H
#define __144TFT_ST7735S_H

#include "gd32e10x.h"
#include "fonts.h"
#include "spi1.h"


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
#define ST7735S_SPI_Init						SPI1_Init
#define ST7735S_SPI_ReadWriteByte		SPI1_ReadWriteByte

/* IO引脚定义 */
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


/* 外部调用函数 --------------------------------------------------------------*/
void ST7735S_CPT144_Init(void);
void ST7735S_CPT144_OpenWindow(u8 X, u8 Y, u8 Width, u8 Height); //设置窗口
void ST7735S_CPT144_Clear(void);
void ST7735S_CPT144_DispChar(u8 usX, u8 Y, const char cChar); //显示一个英文字符
void ST7735S_CPT144_DispString_XDir(u16 X, u16 Y, char* pStr); //显示英文字符串(沿X轴方向)
void ST7735S_CPT144_DispString_YDir(u16 X, u16 Y, char* pStr); //显示英文字符串(沿y轴方向)

void ST7735S_CPT144_SetFont(sFONT* fonts); //设置英文字体类型
void ST7735S_CPT144_SetTextColor(uint16_t Color); //设置LCD的前景(字体)颜色
void ST7735S_CPT144_SetBackColor(uint16_t Color); //设置LCD的背景颜色
void ST7735S_CPT144_displayColor(u16 color); //全屏显示某种颜色
void ST7735S_CPT144_DispChinese(u16 X, u16 Y, u8* Word, u8 Word_width, u8 Word_height); //显示自定义大小的中文/图片
void ST7735S_CPT144_DispPicture(u8* picture); //显示全屏图片

#endif
