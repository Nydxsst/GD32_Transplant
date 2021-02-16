/**
  ******************************************************************************
  * @author  Nydxsst
  * @version V1.0.0
  * @date    2020/02/05
  ******************************************************************************
	* @attention 
  ******************************************************************************
  */

#ifndef __USART_DEBUG_H
#define	__USART_DEBUG_H

#include "gd32e10x.h"


/* �ⲿ���ú��� --------------------------------------------------------------*/
void Usart_Debug_Init(u32 BaudRate);//��ʼ��

extern int printf(const char * , ...);//����printf�����ã�����ᱨwarning
extern  int sprintf(char *str, const char *format, ...);

#endif
