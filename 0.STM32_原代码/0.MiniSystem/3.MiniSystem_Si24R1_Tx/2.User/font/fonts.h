/**
  ******************************************************************************
  * @author  
  * @version V1.0.0
  * @date    2020/07/1
  ******************************************************************************
	* @attention 
  ******************************************************************************
  */

#ifndef __FONT_H
#define __FONT_H       

#include "stm32f10x.h"


/** @defgroup FONTS_Exported_Types
  * @{
  */ 
typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Word_width;
  uint16_t Word_height;
  
}sFONT;

extern sFONT Font24x32;
extern sFONT Font16x24;
extern sFONT Font8x16;


#endif
