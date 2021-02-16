#include "144lcd_st7735s.h"

/* �ڲ�ʹ�ò��� --------------------------------------------------------------*/
/* ������ɫ���� */
static sFONT* ST7735S_CPT144_Currentfonts = &Font8x16; //Ӣ������
static uint16_t CurrentTextColor = ST7735S_CPT144_BLACK;//ǰ��ɫ
static uint16_t CurrentBackColor = ST7735S_CPT144_WHITE;//����ɫ
/* �����߲������壬���ڽ����������ø߻��õ�*/
#define A0_H 			  gpio_bit_set(ST7735S_CPT144_A0_PORT,ST7735S_CPT144_A0_PIN)
#define A0_L 			gpio_bit_reset(ST7735S_CPT144_A0_PORT,ST7735S_CPT144_A0_PIN)
#define CS_H 			  gpio_bit_set(ST7735S_CPT144_CS_PORT,ST7735S_CPT144_CS_PIN)
#define CS_L 			gpio_bit_reset(ST7735S_CPT144_CS_PORT,ST7735S_CPT144_CS_PIN)
#define RES_H 		  gpio_bit_set(ST7735S_CPT144_RES_PORT,ST7735S_CPT144_RES_PIN)
#define RES_L 		gpio_bit_reset(ST7735S_CPT144_RES_PORT,ST7735S_CPT144_RES_PIN)

/* �ڲ����ú��� --------------------------------------------------------------*/
__inline void ST7735S_CPT144_WriteCommand(u8 Cmd);
__inline void ST7735S_CPT144_WriteData(u8 Data);
__inline void ST7735S_CPT144_WriteData16(u16 data16);
void ST7735S_CPT144_GPIO_Init(void);


/**
	* @brief	д����
	* @param  Cmd(����)
	* @retval None
	*/
__inline void ST7735S_CPT144_WriteCommand(u8 Cmd)
{
	A0_L;
	CS_L;
	ST7735S_SPI_ReadWriteByte(Cmd);
	CS_H;
}

/**
	* @brief	д8λ����
	* @param  Data(����)
	* @retval None
	*/
__inline void ST7735S_CPT144_WriteData(u8 Data)
{
	A0_H;
	CS_L;
	ST7735S_SPI_ReadWriteByte(Data);
	CS_H;
}

/**
	* @brief	д16λ����
	* @param  Data16
	* @retval None
	*/
__inline void ST7735S_CPT144_WriteData16(u16 Data16)
{
	ST7735S_CPT144_WriteData(Data16 >> 8);
	ST7735S_CPT144_WriteData(Data16);
}

/**
	* @brief	��ʼ��ST7735S_CPT144��GPIO��
	* @param  None
	* @retval None
	*/
void ST7735S_CPT144_GPIO_Init(void)
{
	//�����ŵ�ʱ��
	rcu_periph_clock_enable(ST7735S_CPT144_A0_SCK);
	rcu_periph_clock_enable(ST7735S_CPT144_CS_SCK);
	rcu_periph_clock_enable(ST7735S_CPT144_RES_SCK);
	rcu_periph_clock_enable(ST7735S_CPT144_BLK_SCK);
	
	//IO�ڳ�ʼ��
	gpio_init(ST7735S_CPT144_A0_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, ST7735S_CPT144_A0_PIN);
	gpio_init(ST7735S_CPT144_CS_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, ST7735S_CPT144_CS_PIN);
	gpio_init(ST7735S_CPT144_RES_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, ST7735S_CPT144_RES_PIN);
	gpio_init(ST7735S_CPT144_BLK_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, ST7735S_CPT144_BLK_PIN);
}

/**
	* @brief	1.44��TFTLCD����IC��ʼ��
	* @param  None
	* @retval None
	*/
void ST7735S_CPT144_Init(void)
{
	ST7735S_CPT144_GPIO_Init();
	SPI1_Init();
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

	ST7735S_CPT144_displayColor(ST7735S_CPT144_RED);
	ST7735S_CPT144_BLK = 1;
}

/**
 * @brief  ����ST7735S_CPT144_��GRAM��ɨ�跽��
 * @param  ucOption ��ѡ��GRAM��ɨ�跽��
 *     @arg 0-7 :������ѡֵΪ0-7��˸�����
 *
 *	����������0��3��5��6 ģʽ�ʺϴ���������ʾ���֣�
 *				���Ƽ�ʹ������ģʽ��ʾ����	����ģʽ��ʾ���ֻ��о���Ч��
 *
 *	���� 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����
 *	���� 3 ģʽΪ����ͷ����ʹ�õķ���
 *	���� 0 ģʽΪBMPͼƬ��ʾ����ʹ�õķ���
 *
 * @retval ��
 * @note  ����ͼ����A��ʾ���ϣ�V��ʾ���£�<��ʾ����>��ʾ����
					X��ʾX�ᣬY��ʾY��

------------------------------------------------------------
ģʽ0��				.		ģʽ1��		.	ģʽ2��			.	ģʽ3��
					A		.					A		.		A					.		A
					|		.					|		.		|					.		|
					Y		.					X		.		Y					.		X
					0		.					1		.		2					.		3
	<--- X0 o		.	<----Y1	o		.		o 2X--->  .		o 3Y--->
------------------------------------------------------------
ģʽ4��				.	ģʽ5��			.	ģʽ6��			.	ģʽ7��
	<--- X4 o		.	<--- Y5 o		.		o 6X--->  .		o 7Y--->
					4		.					5		.		6					.		7
					Y		.					X		.		Y					.		X
					|		.					|		.		|					.		|
					V		.					V		.		V					.		V
*******************************************************/
void ST7735S_CPT144_GramScan(uint8_t ucOption)
{
	//������飬ֻ������0-7
	if(ucOption > 7)
		return;
	//������������
}

/**
	* @brief	���ô���
  * @param  X �����ڵ����X���꣨1~128��
  * @param  Y �����ڵ����Y���꣨1~128��
  * @param  Width �����ڵĿ��
  * @param  Height �����ڵĸ߶�
	* @retval None
	*/
void ST7735S_CPT144_OpenWindow(u8 X, u8 Y, u8 Width, u8 Height)
{
	ST7735S_CPT144_WriteCommand(0x2A); 			 /* �����е�ַ */
	ST7735S_CPT144_WriteData(0);
	ST7735S_CPT144_WriteData(X + 1);
	ST7735S_CPT144_WriteData(0);
	ST7735S_CPT144_WriteData(X + Width);

	ST7735S_CPT144_WriteCommand(0x2B);	     /* �����е�ַ */
	ST7735S_CPT144_WriteData(0);
	ST7735S_CPT144_WriteData(Y + 2);
	ST7735S_CPT144_WriteData(0);
	ST7735S_CPT144_WriteData(Y + Height + 1);

	ST7735S_CPT144_WriteCommand(0x2C);
}

/**
	* @brief	����
	* @param: None
	* @retval None
	*/
void ST7735S_CPT144_Clear(void)
{
	ST7735S_CPT144_displayColor(CurrentBackColor);
}

/**
	* @brief  �� ST7735S_CPT144 ��ʾ������ʾһ��Ӣ���ַ�
	* @param  X �����ڵ����X���꣨1~128��
  * @param  Y �����ڵ����Y���꣨1~128��
  * @param  cChar ��Ҫ��ʾ��Ӣ���ַ�
  * @note   ��ʹ��ST7735S_CPT144_SetTextColor��ST7735S_CPT144_SetBackColor����������ɫ
  * @retval ��
  */
void ST7735S_CPT144_DispChar(u8 X, u8 Y, const char cChar)
{
	u8  byteCount, bitCount, fontLength;
	u16 ucRelativePositon;
	u8* Pfont;

	//��ascii���ƫ�ƣ���ģ������ASCII���ǰ32����ͼ�η��ţ�
	ucRelativePositon = cChar - ' ';
	//ÿ����ģ���ֽ���
	fontLength = (ST7735S_CPT144_Currentfonts->Word_width * ST7735S_CPT144_Currentfonts->Word_height) / 8;
	//��ģ�׵�ַ
	/*ascii���ƫ��ֵ����ÿ����ģ���ֽ����������ģ��ƫ��λ��*/
	Pfont = (u8*)&ST7735S_CPT144_Currentfonts->table[ucRelativePositon * fontLength];
	//������ʾ����
	ST7735S_CPT144_OpenWindow(X, Y, ST7735S_CPT144_Currentfonts->Word_width, ST7735S_CPT144_Currentfonts->Word_height);
	//���ֽڶ�ȡ��ģ����
	//����ǰ��ֱ����������ʾ���ڣ���ʾ���ݻ��Զ�����
	for(byteCount = 0; byteCount < fontLength; byteCount++)
	{
		//һλһλ����Ҫ��ʾ����ɫ
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
  * @brief  �� ST7735S_CPT144 ��ʾ������ʾӢ���ַ���(��X�᷽��)
  * @param  X �����ڵ����X���꣨1~128��
  * @param  Y �����ڵ����Y���꣨1~128��
  * @param  pStr ��Ҫ��ʾ��Ӣ���ַ������׵�ַ
  * @note   ��ʹ��ST7735S_CPT144_SetTextColor��ST7735S_CPT144_SetBackColor����������ɫ
  * @retval ��
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
  * @brief  �� ST7735S_CPT144 ��ʾ������ʾӢ���ַ���(��Y�᷽��)
  * @param  X �����ڵ����X���꣨1~128��
  * @param  Y �����ڵ����Y���꣨1~128��
  * @param  pStr ��Ҫ��ʾ��Ӣ���ַ������׵�ַ
  * @note   ��ʹ��ST7735S_CPT144_SetTextColor��ST7735S_CPT144_SetBackColor����������ɫ
  * @retval ��
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
  * @brief  ����Ӣ����������
  * @param  fonts: ָ��Ҫѡ�������
	*		����Ϊ����ֵ֮һ
  * 	@arg��Font24x32;
  * 	@arg��Font16x24;
  * 	@arg��Font8x16;
  * @retval None
  */
void ST7735S_CPT144_SetFont(sFONT* fonts)
{
	ST7735S_CPT144_Currentfonts = fonts;
}

/**
	* @brief	ȫ����ʾĳ����ɫ
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
  * @brief  ����LCD��ǰ��(����)��ɫ,RGB565
  * @param  Color: ָ��ǰ��(����)��ɫ
  * @retval None
  */
void ST7735S_CPT144_SetTextColor(uint16_t Color)
{
	CurrentTextColor = Color;
}

/**
  * @brief  ����LCD�ı�����ɫ,RGB565
  * @param  Color: ָ��������ɫ
  * @retval None
  */
void ST7735S_CPT144_SetBackColor(uint16_t Color)
{
	CurrentBackColor = Color;
}

/**
	* @brief	��ʾ�Զ����С������/ͼƬ
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
	* @brief	��ʾȫ��ͼƬ
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
