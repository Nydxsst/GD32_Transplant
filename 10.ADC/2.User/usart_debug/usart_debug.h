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


/* 外部调用函数 --------------------------------------------------------------*/
void Usart_Debug_Init(u32 BaudRate);//初始化

extern int printf(const char * , ...);//声明printf的引用，否则会报warning
extern  int sprintf(char *str, const char *format, ...);

#endif
