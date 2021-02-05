#include "144lcd_st7735s.h"

/* 内部使用参量 --------------------------------------------------------------*/
/* 字体颜色定义 */
static sFONT* ST7735S_CPT144_Currentfonts = &Font8x16; //英文字体
static uint16_t CurrentTextColor = ST7735S_CPT144_BLACK;//前景色
static uint16_t CurrentBackColor = ST7735S_CPT144_WHITE;//背景色
/* 操作线定义 */
#define A0_H 			ST7735S_CPT144_A0_PORT->ODR|=1<<12
#define A0_L 			ST7735S_CPT144_A0_PORT->ODR&=~(1<<12)
#define CS_H 			ST7735S_CPT144_CS_PORT->ODR|=1<<11
#define CS_L 			ST7735S_CPT144_CS_PORT->ODR&=~(1<<11)
#define RES_H 		ST7735S_CPT144_RES_PORT->ODR|=1<<8
#define RES_L 		ST7735S_CPT144_RES_PORT->ODR&=~(1<<8)

/* 内部调用函数 --------------------------------------------------------------*/
void ST7735S_CPT144_WriteCommand(u8 Cmd);
void ST7735S_CPT144_WriteData(u8 Data);
void ST7735S_CPT144_WriteData16(u16 data16);
void ST7735S_CPT144_GPIO_Init(void);


/**
	* @brief	初始化ST7735S_CPT144的GPIO口
	* @param  None
	* @retval None
	*/
void ST7735S_CPT144_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(ST7735S_CPT144_A0_SCK | ST7735S_CPT144_CS_SCK |
	                       ST7735S_CPT144_RES_SCK | ST7735S_CPT144_BLK_SCK, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_InitStructure.GPIO_Pin = ST7735S_CPT144_A0_PIN;
	GPIO_Init(ST7735S_CPT144_A0_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ST7735S_CPT144_CS_PIN;
	GPIO_Init(ST7735S_CPT144_CS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ST7735S_CPT144_RES_PIN;
	GPIO_Init(ST7735S_CPT144_RES_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ST7735S_CPT144_BLK_PIN;
	GPIO_Init(ST7735S_CPT144_BLK_PORT, &GPIO_InitStructure);
}

/**
	* @brief	1.44寸TFTLCD驱动IC初始化
	* @param  None
	* @retval None
	*/
void ST7735S_CPT144_Init(void)
{
	ST7735S_CPT144_GPIO_Init();
	ST7735S_SPI_Init();
	SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
	//--ST7735R Reset Sequence--//
	RES_L;
	Delay_Ms(1);
	RES_H;
	Delay_Ms(120);
	//--End ST7735S Reset Sequence --//
	ST7735S_CPT144_WriteCommand(0x11); //Sleep out
	Delay_Ms(120);
	//--ST7735S Frame rate--//
	ST7735S_CPT144_WriteCommand(0xB1); //Frame rate 80Hz
	ST7735S_CPT144_WriteData(0x02);
	ST7735S_CPT144_WriteData(0x35);
	ST7735S_CPT144_WriteData(0x36);
	ST7735S_CPT144_WriteCommand(0xB2); //Frame rate 80Hz
	ST7735S_CPT144_WriteData(0x02);
	ST7735S_CPT144_WriteData(0x35);
	ST7735S_CPT144_WriteData(0x36);
	ST7735S_CPT144_WriteCommand(0xB3); //Frame rate 80Hz
	ST7735S_CPT144_WriteData(0x02);
	ST7735S_CPT144_WriteData(0x35);
	ST7735S_CPT144_WriteData(0x36);
	ST7735S_CPT144_WriteData(0x02);
	ST7735S_CPT144_WriteData(0x35);
	ST7735S_CPT144_WriteData(0x36);
	//--End ST7735S Frame rate--//
	ST7735S_CPT144_WriteCommand(0xB4); //Dot inversion
	ST7735S_CPT144_WriteData(0x03);
	//--ST7735S Power Sequence--//
	ST7735S_CPT144_WriteCommand(0xC0);
	ST7735S_CPT144_WriteData(0xA2);
	ST7735S_CPT144_WriteData(0x02);
	ST7735S_CPT144_WriteData(0x84);
	ST7735S_CPT144_WriteCommand(0xC1);
	ST7735S_CPT144_WriteData(0xC5);
	ST7735S_CPT144_WriteCommand(0xC2);
	ST7735S_CPT144_WriteData(0x0D);
	ST7735S_CPT144_WriteData(0x00);
	ST7735S_CPT144_WriteCommand(0xC3);
	ST7735S_CPT144_WriteData(0x8D);
	ST7735S_CPT144_WriteData(0x2A);
	ST7735S_CPT144_WriteCommand(0xC4);
	ST7735S_CPT144_WriteData(0x8D);
	ST7735S_CPT144_WriteData(0xEE);
	//--End ST7735S Power Sequence--//
	ST7735S_CPT144_WriteCommand(0xC5); //VCOM
	ST7735S_CPT144_WriteData(0x0a);
	ST7735S_CPT144_WriteCommand(0x36); //MX, MY, RGB mode
	ST7735S_CPT144_WriteData(0xC8);
	//--ST7735S Gamma Sequence--//
	ST7735S_CPT144_WriteCommand(0XE0);
	ST7735S_CPT144_WriteData(0x12);
	ST7735S_CPT144_WriteData(0x1C);
	ST7735S_CPT144_WriteData(0x10);
	ST7735S_CPT144_WriteData(0x18);
	ST7735S_CPT144_WriteData(0x33);
	ST7735S_CPT144_WriteData(0x2C);
	ST7735S_CPT144_WriteData(0x25);
	ST7735S_CPT144_WriteData(0x28);
	ST7735S_CPT144_WriteData(0x28);
	ST7735S_CPT144_WriteData(0x27);
	ST7735S_CPT144_WriteData(0x2F);
	ST7735S_CPT144_WriteData(0x3C);
	ST7735S_CPT144_WriteData(0x00);
	ST7735S_CPT144_WriteData(0x03);
	ST7735S_CPT144_WriteData(0x03);
	ST7735S_CPT144_WriteData(0x10);
	ST7735S_CPT144_WriteCommand(0XE1);
	ST7735S_CPT144_WriteData(0x12);
	ST7735S_CPT144_WriteData(0x1C);
	ST7735S_CPT144_WriteData(0x10);
	ST7735S_CPT144_WriteData(0x18);
	ST7735S_CPT144_WriteData(0x2D);
	ST7735S_CPT144_WriteData(0x28);
	ST7735S_CPT144_WriteData(0x23);
	ST7735S_CPT144_WriteData(0x28);
	ST7735S_CPT144_WriteData(0x28);
	ST7735S_CPT144_WriteData(0x26);
	ST7735S_CPT144_WriteData(0x2F);
	ST7735S_CPT144_WriteData(0x3B);
	ST7735S_CPT144_WriteData(0x00);
	ST7735S_CPT144_WriteData(0x03);
	ST7735S_CPT144_WriteData(0x03);
	ST7735S_CPT144_WriteData(0x10);
	//--End ST7735S Gamma Sequence--//
	ST7735S_CPT144_WriteCommand(0x3A); //65k mode
	ST7735S_CPT144_WriteData(0x05);
	ST7735S_CPT144_WriteCommand(0x29); //Display on

	ST7735S_CPT144_displayColor(ST7735S_CPT144_WHITE);
	ST7735S_CPT144_BLK = 1;
}

/**
	* @brief	写命令
	* @param  Cmd(命令)
	* @retval None
	*/
void ST7735S_CPT144_WriteCommand(u8 Cmd)
{
	A0_L;
	ST7735S_SPI_ReadWriteByte(Cmd);
}

/**
	* @brief	写8位数据
	* @param  Data(数据)
	* @retval None
	*/
void ST7735S_CPT144_WriteData(u8 Data)
{
	A0_H;
	ST7735S_SPI_ReadWriteByte(Data);
}

/**
	* @brief	写16位数据
	* @param  Data16
	* @retval None
	*/
void ST7735S_CPT144_WriteData16(u16 Data16)
{
	ST7735S_CPT144_WriteData(Data16 >> 8);
	ST7735S_CPT144_WriteData(Data16);
}

/**
 * @brief  设置ST7735S_CPT144_的GRAM的扫描方向
 * @param  ucOption ：选择GRAM的扫描方向
 *     @arg 0-7 :参数可选值为0-7这八个方向
 *
 *	！！！其中0、3、5、6 模式适合从左至右显示文字，
 *				不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果
 *
 *	其中 6 模式为大部分液晶例程的默认显示方向
 *	其中 3 模式为摄像头例程使用的方向
 *	其中 0 模式为BMP图片显示例程使用的方向
 *
 * @retval 无
 * @note  坐标图例：A表示向上，V表示向下，<表示向左，>表示向右
					X表示X轴，Y表示Y轴

------------------------------------------------------------
模式0：				.		模式1：		.	模式2：			.	模式3：
					A		.					A		.		A					.		A
					|		.					|		.		|					.		|
					Y		.					X		.		Y					.		X
					0		.					1		.		2					.		3
	<--- X0 o		.	<----Y1	o		.		o 2X--->  .		o 3Y--->
------------------------------------------------------------
模式4：				.	模式5：			.	模式6：			.	模式7：
	<--- X4 o		.	<--- Y5 o		.		o 6X--->  .		o 7Y--->
					4		.					5		.		6					.		7
					Y		.					X		.		Y					.		X
					|		.					|		.		|					.		|
					V		.					V		.		V					.		V
*******************************************************/
void ST7735S_CPT144_GramScan(uint8_t ucOption)
{
	//参数检查，只可输入0-7
	if(ucOption > 7)
		return;
	//具体命令待填充
}

/**
	* @brief	设置窗口
  * @param  X ：窗口的起点X坐标（1~128）
  * @param  Y ：窗口的起点Y坐标（1~128）
  * @param  Width ：窗口的宽度
  * @param  Height ：窗口的高度
	* @retval None
	*/
void ST7735S_CPT144_OpenWindow(u8 X, u8 Y, u8 Width, u8 Height)
{
	ST7735S_CPT144_WriteCommand(0x2A); 			 /* 设置行地址 */
	ST7735S_CPT144_WriteData(0);
	ST7735S_CPT144_WriteData(X + 1);
	ST7735S_CPT144_WriteData(0);
	ST7735S_CPT144_WriteData(X + Width);

	ST7735S_CPT144_WriteCommand(0x2B);	     /* 设置列地址 */
	ST7735S_CPT144_WriteData(0);
	ST7735S_CPT144_WriteData(Y + 2);
	ST7735S_CPT144_WriteData(0);
	ST7735S_CPT144_WriteData(Y + Height + 1);

	ST7735S_CPT144_WriteCommand(0x2C);
}

/**
	* @brief	清屏
	* @param: None
	* @retval None
	*/
void ST7735S_CPT144_Clear(void)
{
	ST7735S_CPT144_displayColor(CurrentBackColor);
}

/**
	* @brief  在 ST7735S_CPT144 显示器上显示一个英文字符
	* @param  X ：窗口的起点X坐标（1~128）
  * @param  Y ：窗口的起点Y坐标（1~128）
  * @param  cChar ：要显示的英文字符
  * @note   可使用ST7735S_CPT144_SetTextColor、ST7735S_CPT144_SetBackColor函数设置颜色
  * @retval 无
  */
void ST7735S_CPT144_DispChar(u8 X, u8 Y, const char cChar)
{
	u8  byteCount, bitCount, fontLength;
	u16 ucRelativePositon;
	u8* Pfont;

	//对ascii码表偏移（字模表不包含ASCII表的前32个非图形符号）
	ucRelativePositon = cChar - ' ';
	//每个字模的字节数
	fontLength = (ST7735S_CPT144_Currentfonts->Word_width * ST7735S_CPT144_Currentfonts->Word_height) / 8;
	//字模首地址
	/*ascii码表偏移值乘以每个字模的字节数，求出字模的偏移位置*/
	Pfont = (u8*)&ST7735S_CPT144_Currentfonts->table[ucRelativePositon * fontLength];
	//设置显示窗口
	ST7735S_CPT144_OpenWindow(X, Y, ST7735S_CPT144_Currentfonts->Word_width, ST7735S_CPT144_Currentfonts->Word_height);
	//按字节读取字模数据
	//由于前面直接设置了显示窗口，显示数据会自动换行
	for(byteCount = 0; byteCount < fontLength; byteCount++)
	{
		//一位一位处理要显示的颜色
		for(bitCount = 0; bitCount < 8; bitCount++)
		{
			if(Pfont[byteCount] & (0x80 >> bitCount))
				ST7735S_CPT144_WriteData16(CurrentTextColor);
			else
				ST7735S_CPT144_WriteData16(CurrentBackColor);
		}
	}
}

/**
  * @brief  在 ST7735S_CPT144 显示器上显示英文字符串(沿X轴方向)
  * @param  X ：窗口的起点X坐标（1~128）
  * @param  Y ：窗口的起点Y坐标（1~128）
  * @param  pStr ：要显示的英文字符串的首地址
  * @note   可使用ST7735S_CPT144_SetTextColor、ST7735S_CPT144_SetBackColor函数设置颜色
  * @retval 无
  */
void ST7735S_CPT144_DispString_XDir(u16 X, u16 Y, char* pStr)
{
	while(*pStr != '\0')
	{
		if((X - ST7735S_CPT144_DispWindow_X_Star + ST7735S_CPT144_Currentfonts->Word_width) > ST7735S_CPT144_Width)
		{
			X = ST7735S_CPT144_DispWindow_X_Star;
			Y += ST7735S_CPT144_Currentfonts->Word_height;
		}
		if((Y - ST7735S_CPT144_DispWindow_Y_Star + ST7735S_CPT144_Currentfonts->Word_height) > ST7735S_CPT144_Height)
		{
			X = ST7735S_CPT144_DispWindow_X_Star;
			Y = ST7735S_CPT144_DispWindow_Y_Star;
		}
		ST7735S_CPT144_DispChar(X, Y, *pStr);
		pStr ++;
		X += ST7735S_CPT144_Currentfonts->Word_width;
	}
}

/**
  * @brief  在 ST7735S_CPT144 显示器上显示英文字符串(沿Y轴方向)
  * @param  X ：窗口的起点X坐标（1~128）
  * @param  Y ：窗口的起点Y坐标（1~128）
  * @param  pStr ：要显示的英文字符串的首地址
  * @note   可使用ST7735S_CPT144_SetTextColor、ST7735S_CPT144_SetBackColor函数设置颜色
  * @retval 无
  */
void ST7735S_CPT144_DispString_YDir(u16 X, u16 Y, char* pStr)
{
	while(*pStr != '\0')
	{
		if((Y - ST7735S_CPT144_DispWindow_Y_Star + ST7735S_CPT144_Currentfonts->Word_height) > ST7735S_CPT144_Height)
		{
			Y = ST7735S_CPT144_DispWindow_Y_Star;
			X += ST7735S_CPT144_Currentfonts->Word_width;
		}
		if((X - ST7735S_CPT144_DispWindow_X_Star + ST7735S_CPT144_Currentfonts->Word_width) >  ST7735S_CPT144_Width)
		{
			X = ST7735S_CPT144_DispWindow_X_Star;
			Y = ST7735S_CPT144_DispWindow_Y_Star;
		}
		ST7735S_CPT144_DispChar(X, Y, *pStr);
		pStr ++;
		Y += ST7735S_CPT144_Currentfonts->Word_height;
	}
}

/**
  * @brief  设置英文字体类型
  * @param  fonts: 指定要选择的字体
	*		参数为以下值之一
  * 	@arg：Font24x32;
  * 	@arg：Font16x24;
  * 	@arg：Font8x16;
  * @retval None
  */
void ST7735S_CPT144_SetFont(sFONT* fonts)
{
	ST7735S_CPT144_Currentfonts = fonts;
}

/**
	* @brief	全屏显示某种颜色
	* @param: color
	* @retval None
	*/
void ST7735S_CPT144_displayColor(u16 color)
{
	u8 i, j;
	ST7735S_CPT144_OpenWindow(1, 1, ST7735S_CPT144_Width, ST7735S_CPT144_Height);
	for(i = 0; i <= ST7735S_CPT144_Height; i++)
	{
		for(j = 0; j <= ST7735S_CPT144_Width; j++)
		{
			ST7735S_CPT144_WriteData16(color);
		}
	}
}

/**
  * @brief  设置LCD的前景(字体)颜色,RGB565
  * @param  Color: 指定前景(字体)颜色
  * @retval None
  */
void ST7735S_CPT144_SetTextColor(uint16_t Color)
{
	CurrentTextColor = Color;
}

/**
  * @brief  设置LCD的背景颜色,RGB565
  * @param  Color: 指定背景颜色
  * @retval None
  */
void ST7735S_CPT144_SetBackColor(uint16_t Color)
{
	CurrentBackColor = Color;
}

/**
	* @brief	显示自定义大小的中文/图片
	* @param: picture
	* @retval None
	*/
void ST7735S_CPT144_DispChinese(u16 X, u16 Y, u8* Word, u8 Word_width, u8 Word_height)
{
	u16 i, j;
	ST7735S_CPT144_OpenWindow(X, Y, Word_width, Word_height);
	for(i = 0; i < (Word_width * Word_height / 8); i++)
	{
		for(j = 0; j < 8; j++)
		{
			if((Word[i] >> (7 - j)) & 0x01)ST7735S_CPT144_WriteData16(CurrentTextColor);
			else ST7735S_CPT144_WriteData16(CurrentBackColor);
		}
	}
}

/**
	* @brief	显示全屏图片
	* @param: picture
	* @retval None
	*/
void ST7735S_CPT144_DispPicture(u8* picture)
{
	u16 i, j;
	ST7735S_CPT144_OpenWindow(1, 1, ST7735S_CPT144_Width, ST7735S_CPT144_Height);
	for(i = 0; i < (ST7735S_CPT144_Height * ST7735S_CPT144_Width / 8); i++)
	{
		for(j = 0; j < 8; j++)
		{
			if((picture[i] >> (7 - j)) & 0x01)ST7735S_CPT144_WriteData16(CurrentTextColor);
			else ST7735S_CPT144_WriteData16(CurrentBackColor);
		}
	}
}
