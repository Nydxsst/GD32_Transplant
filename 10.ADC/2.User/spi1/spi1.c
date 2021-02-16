#include "spi1.h"

void SPI1_GPIO_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_SPI1);
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_15);
}

/**
	* @brief	��ʼ��SPI1
	* @param  None
	* @retval None
	*/
void SPI1_Init(void)
{
	//��ʼ���ṹ��
	spi_parameter_struct  spi_init_struct;
	//��ʼ��SPI1��IO
	SPI1_GPIO_Init();
	//��λSPI1
	spi_i2s_deinit(SPI1);
	//��ʼ���ṹ�����
	spi_struct_para_init(&spi_init_struct);
	
	/* SPI1 parameter config */
	spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX; 	//����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	spi_init_struct.device_mode          = SPI_MASTER;								//����SPI����ģʽ:����Ϊ��SPI
	spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;				//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ,��һ�������أ��������½������ݱ�����
	spi_init_struct.nss                  = SPI_NSS_SOFT;							//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������
	spi_init_struct.prescale             = SPI_PSC_2;									//���岨����Ԥ��Ƶ��ֵ
	spi_init_struct.endian               = SPI_ENDIAN_MSB;						//ָ�����ݴ����MSBλ����LSBλ��ʼ
	spi_init(SPI1, &spi_init_struct);				//��ʼ���ṹ��
	spi_nss_output_disable(SPI1);						//�ر�nss���ŵĹ������
	spi_bidirectional_transfer_config(SPI1,SPI_BIDIRECTIONAL_TRANSMIT);  //ֻ�����ڷ���ģʽ
	spi_enable(SPI1);						//ʹ��SPI����
	
	SPI1_ReadWriteByte(0xff);		//��������
}


/**
	* @brief	����SPI2���ٶ�
	* @param  TxData:Ҫд����ֽ�
	* @retval ��ȡ�����ֽ�
	*/
u8 SPI1_ReadWriteByte(u8 TxData)
{
	while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE)){}
  spi_i2s_data_transmit(SPI1, TxData);
	while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE)){}
	return spi_i2s_data_receive(SPI1);			    
}
