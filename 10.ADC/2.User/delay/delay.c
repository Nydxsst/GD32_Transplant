#include "delay.h"

/* �ڲ�ʹ�ò��� --------------------------------------------------------------*/
u8  Fac_Us = 0;							//us��ʱ������
u16 Fac_Ms = 0;							//ms��ʱ������

void Delay_ms(u16 Nms);

/**
	* @brief	��ʼ���ӳٺ���
	* @param  None
	* @retval None
	* @note		SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
	* @note   SYSCLK:ϵͳʱ��
	*/
void Delay_Init(void)
{
	systick_clksource_set(SYSTICK_CLKSOURCE_HCLK_DIV8);	//ѡ���ⲿʱ��  HCLK/8
	Fac_Us = 15;										//ÿ1us���������ݼ�n������120M/8��
	Fac_Ms = (u16)Fac_Us * 1000;	//ÿ1ms���������ݼ�n*1000����
}

/**
	* @brief	us����ʱ
	* @param	nus:��ʱ��us��
	* @retval None
	* @note		��һ�������,usԽС,����Խ��
	*/		    
void Delay_Us(u32 Nus)
{
	SysTick->LOAD = Fac_Us * Nus;//�Զ���װ��ֵ
	SysTick->VAL = 0x00;//�����ʱ����ֵ
	SysTick->CTRL |= (1 << 0);//SysTickʹ�ܣ�ʹ�ܺ�ʱ����ʼ����
	while(!(SysTick->CTRL & (1 << 16)));//�ж��Ƿ����0������0ʱCTRL�ĵ�16λ����1����ȡ����Զ���0
}

/**
	* @brief	Ms����ʱ
	* @param	Nms:��ʱ��Ms��
	* @retval None
	* @note		SysTick->LOADΪ24λ�Ĵ���, ��ʱ��Us��/���� = ����Ĵ�������
	* 				��Nms * 1000 * SysTick_Clk(15M) <= 2^24, ��Nms <= 1118
	*/				    
void Delay_ms(u16 Nms)
{
	SysTick->LOAD = Fac_Ms * Nms;//�Զ���װ��ֵ
	SysTick->VAL = 0x00;//�����ʱ����ֵ
	SysTick->CTRL |= (1 << 0);//SysTickʹ�ܣ�ʹ�ܺ�ʱ����ʼ����
	while(!(SysTick->CTRL & (1 << 16)));//�ж��Ƿ����0������0ʱCTRL�ĵ�16λ����1����ȡ����Զ���0
}

/**
	* @brief	Ms����ʱ
	* @param	Nms:��ʱ��Ms��
	* @retval None
	* @note		����Delay_ms,���ڴ����������Nms,ʹNms:0~65535
	*/	
void Delay_Ms(u16 Nms)
{	 	 
	u8 repeat=Nms/1000;	
	u16 remain=Nms%1000;
	while(repeat)
	{
		Delay_ms(1000);
		repeat--;
	}
	if(remain)Delay_ms(remain);
} 
