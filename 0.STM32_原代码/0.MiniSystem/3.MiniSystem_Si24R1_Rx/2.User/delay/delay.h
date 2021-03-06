/**
  ******************************************************************************
  * @author  
  * @version V1.0.0
  * @date    2020/05/14
  ******************************************************************************
	* @attention 
  ******************************************************************************
  */

#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"


/* 外部调用函数 --------------------------------------------------------------*/
void Delay_Init(void);
void Delay_Ms(u16 nms);// >1864溢出
void Delay_Us(u32 nus);

#endif


