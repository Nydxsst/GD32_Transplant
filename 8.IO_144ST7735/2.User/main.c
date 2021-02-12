/**
  ******************************************************************************
  * @author  Moment/Nydxsst
  * @version V1.0.0
  * @date    2020/02/12
  ******************************************************************************
	* @attention
	* 如果需用到PA13.PA14.PA15.PB3.PB4，则需要关jlink
	* rcu_periph_clock_enable(RCU_AF);
	* gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
  ******************************************************************************
  */

#include "gd32e10x.h"
#include "delay.h"
#include "led.h"
#include "usart_debug.h"
#include "144lcd_st7735s.h"

u8 IMAlogo[]=
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFC,0x1F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xC0,0x01,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x01,0xF8,0x00,0x00,0x0F,0xC0,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x03,0xF0,0x00,0x00,0x07,0xE0,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x0F,0xC0,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0x00,0x0F,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x03,0xF0,0x00,0x00,0x00,0x00,0x07,0xE0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0xE0,0x00,0x00,0x00,0x00,0x03,0xF0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x00,0x00,0x01,0xF0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x01,0xFC,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x80,0x00,0x00,0x00,
	0x00,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xC0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x03,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x03,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x07,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xF0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xF0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x0F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xF0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x0F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,
	0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,
	0x00,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,
	0x00,0x00,0x00,0x3F,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,
	0x00,0x00,0x00,0x3F,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,
	0x00,0x00,0x00,0x7E,0x07,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,
	0x00,0x00,0x00,0x7E,0x0F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,
	0x00,0x00,0x00,0x7C,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,
	0x00,0x00,0x00,0xFC,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,
	0x00,0x00,0x00,0xFC,0x1F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,
	0x00,0x00,0x00,0xFC,0x1F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x00,0x00,
	0x00,0x00,0x01,0xF8,0x1F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x80,0x00,0x00,
	0x00,0x00,0x01,0xF8,0x1F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xC0,0x00,0x00,
	0x00,0x00,0x01,0xF8,0x1F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xC0,0x00,0x00,
	0x00,0x00,0x03,0xF0,0x3F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xC0,0x00,0x00,
	0x00,0x00,0x03,0xF0,0x3F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE0,0x00,0x00,
	0x00,0x00,0x03,0xF0,0x3F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE0,0x00,0x00,
	0x00,0x00,0x03,0xE0,0x3F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xE0,0x00,0x00,
	0x00,0x00,0x03,0xE0,0x7F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x00,0x00,
	0x00,0x00,0x07,0xE0,0x7F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xF0,0x00,0x00,
	0x00,0x00,0x07,0xE0,0x7F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xF0,0x00,0x00,
	0x00,0x00,0x07,0xE0,0x7F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xF0,0x00,0x00,
	0x00,0x00,0x0F,0xC0,0x7F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xF0,0x00,0x00,
	0x00,0x00,0x0F,0xC0,0x7F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0xC0,0x7F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0xC0,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0xC0,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0xC0,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x80,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x80,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x81,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x81,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x81,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x81,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x83,0xFF,0xFF,0x80,0x01,0x80,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x83,0xFF,0xFF,0xC0,0x07,0xE0,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x83,0xFF,0xFF,0xC0,0x0F,0xF0,0x00,0x3C,0x00,0x00,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x83,0xFF,0xFF,0xC0,0x0F,0xF8,0x00,0xFE,0x00,0x0F,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x87,0xFF,0xFF,0xC0,0x1F,0xF8,0x01,0xFF,0x80,0x3F,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x87,0xFF,0xFF,0xE0,0x1F,0xFC,0x03,0xFF,0xC0,0xFF,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x87,0xFF,0xFF,0xE0,0x3F,0xFE,0x07,0xFF,0xF7,0xFF,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x87,0xFF,0xFF,0xE0,0x3F,0xFF,0x9F,0xFF,0xFF,0xFF,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x8F,0xFF,0xFF,0xF0,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x8F,0xFF,0xFF,0xF0,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x9F,0xFF,0xFF,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,
	0x00,0x00,0x0F,0x9F,0xFF,0xFF,0xF9,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,
	0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,
	0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,
	0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,
	0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,
	0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,
	0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,
	0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,
	0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,
	0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,
	0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,
	0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,
	0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,
	0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,
	0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,
	0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,
	0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,
	0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,
	0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,
	0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,
	0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
u8 dan[]=
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x10,0x00,0x01,0xC0,0x00,0x1F,0xC0,
0x00,0x00,0x00,0x01,0xC0,0x00,0x1E,0x00,0x00,0x00,0x00,0x01,0xC0,0x00,0x38,0x00,
0x00,0x00,0xC0,0x01,0xC0,0x00,0x30,0x00,0x00,0x00,0xE0,0x01,0xC0,0x00,0x60,0x00,
0x00,0x00,0xF8,0x01,0xC0,0x00,0xC0,0x00,0x00,0x01,0xF0,0x01,0xC0,0x02,0x00,0x00,
0x00,0x01,0xF0,0x01,0xC0,0x07,0x00,0x00,0x00,0x01,0xE0,0x01,0xFF,0xFF,0x80,0x00,
0x00,0x03,0xE0,0x01,0xFF,0xFF,0xC0,0x00,0x00,0x03,0xF0,0x01,0xC0,0x00,0x00,0x00,
0x00,0x07,0x9C,0x01,0xC0,0x00,0x00,0x00,0x00,0x07,0x8E,0x01,0xC0,0x00,0x00,0x00,
0x00,0x0F,0x03,0x81,0xC0,0x00,0x00,0x00,0x00,0x0E,0x01,0xE1,0xC0,0x00,0x00,0x00,
0x00,0x1E,0x00,0xF9,0xC0,0x00,0x00,0x00,0x00,0x1C,0x00,0x3F,0xC0,0x00,0x00,0x00,
0x00,0x38,0x00,0x0F,0xF8,0x00,0x00,0x00,0x00,0x70,0x00,0x03,0xFF,0xFC,0x00,0x78,
0x00,0xE0,0x00,0x00,0xFF,0xFF,0xFF,0xF0,0x01,0xC0,0x00,0x02,0x0F,0xFF,0xFF,0xC0,
0x03,0x80,0x00,0x03,0x80,0x3F,0xFF,0x80,0x03,0x00,0x00,0x03,0xE0,0x00,0x1F,0x00,
0x0C,0x00,0x00,0x03,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0xC0,0x00,0x40,0x00,0x00,0x06,0x00,0x03,0xC0,0x00,0xE0,0x00,
0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,
0x00,0x07,0x80,0x03,0xC0,0x00,0xF0,0x00,0x00,0x07,0x80,0x03,0xC0,0x00,0xE0,0x00,
0x00,0x07,0x80,0x03,0xC0,0x00,0xE0,0x00,0x00,0x07,0x80,0x03,0xC0,0x00,0xE0,0x00,
0x00,0x07,0x80,0x03,0xC0,0x00,0xE0,0x00,0x00,0x07,0x80,0x03,0xC0,0x00,0xE0,0x00,
0x00,0x07,0x80,0x03,0xC0,0x00,0xE0,0x00,0x00,0x07,0x80,0x03,0xC0,0x00,0xE0,0x00,
0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,
0x00,0x07,0x80,0x03,0xC0,0x00,0xE0,0x00,0x00,0x07,0x80,0x03,0xC0,0x00,0x00,0x00,
0x00,0x04,0x00,0x03,0xC0,0x0C,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x07,0x00,0x00,
0x00,0x00,0x00,0x03,0xC0,0x03,0xC0,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0xF0,0x00,
0x00,0x00,0x00,0x03,0xC0,0x00,0x7C,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x3F,0x00,
0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0x80,0x03,0xFF,0xFF,0xFF,0xFF,0xF8,0x0F,0xC0,
0x03,0xFF,0xFF,0xFF,0xF0,0x00,0x07,0xC0,0x01,0xFF,0xFF,0xC0,0x00,0x00,0x03,0xC0,
0x01,0xFF,0x80,0x00,0x00,0x00,0x03,0xC0,0x00,0xE0,0x00,0x00,0x00,0x00,0x01,0xC0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};/*"μ°",0*/
u8 xie[]=
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x20,0x00,0x00,0x00,
0x00,0x0F,0x00,0x00,0x38,0x00,0x00,0x00,0x00,0x0F,0xC0,0x00,0x3E,0x00,0x00,0x00,
0x00,0x0F,0x80,0x00,0x3E,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x3C,0x00,0x00,0x00,
0x00,0x0F,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x3C,0x00,0x00,0x00,
0x00,0x0F,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x3C,0x00,0x00,0x00,
0x00,0x0F,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x3C,0x00,0x00,0x00,
0x00,0x0F,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x3C,0x00,0x40,0x00,
0x00,0x0F,0x00,0x00,0x3C,0x00,0xE0,0x00,0x00,0x0F,0x01,0xFF,0xFF,0xFF,0xF8,0x00,
0x00,0x0F,0x00,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x0F,0x02,0x60,0x3C,0x00,0xE0,0x00,
0x00,0x0F,0x07,0x00,0x3C,0x00,0xE0,0x00,0x1F,0xFF,0xFF,0x80,0x3C,0x00,0xE0,0x00,
0x0F,0xFF,0xFF,0xC0,0x3C,0x00,0xE0,0x00,0x04,0x0F,0x00,0x00,0x3C,0x00,0xE0,0x00,
0x00,0x0F,0x00,0x00,0x3C,0x00,0xE0,0x00,0x00,0x0F,0x00,0x00,0x3C,0x00,0xE0,0x00,
0x00,0x0F,0x00,0x00,0x3C,0x00,0xE0,0x00,0x00,0x0F,0x00,0x00,0x38,0x00,0xE0,0x00,
0x00,0x0F,0x00,0x40,0x38,0x00,0xE4,0x00,0x00,0x0F,0x00,0xC0,0x38,0x01,0xE4,0x00,
0x00,0x0F,0x00,0xC0,0x38,0x01,0xE6,0x00,0x00,0x0F,0x00,0xC0,0x38,0x01,0xE3,0x00,
0x00,0x0F,0x00,0xC0,0x38,0x01,0xE3,0x80,0x00,0x0F,0x01,0xC0,0x78,0x01,0xE1,0xC0,
0x00,0x0F,0x01,0xC0,0x78,0x01,0xE1,0xE0,0x00,0x0F,0x03,0xC0,0x70,0x01,0xE1,0xE0,
0x00,0x0F,0x03,0x80,0x70,0x01,0xE0,0xF0,0x00,0x0F,0x07,0x80,0x70,0x01,0xC0,0xF0,
0x00,0x0F,0x0F,0x80,0xF0,0x01,0xC0,0xF0,0x00,0x0F,0x1F,0x80,0xE0,0x01,0xC0,0xF0,
0x00,0x0F,0x1F,0x00,0xE0,0x01,0xC0,0x70,0x00,0x0F,0x00,0x01,0xE0,0x01,0xC0,0x70,
0x00,0x0F,0x00,0x01,0xC0,0x01,0xC0,0x40,0x00,0x0F,0x00,0x01,0xC0,0x01,0xC0,0x00,
0x00,0x0F,0x00,0x03,0x80,0x01,0xC0,0x00,0x00,0x0F,0x00,0x03,0x80,0x01,0xC0,0x00,
0x00,0x0F,0x00,0x07,0x00,0x01,0xC0,0x00,0x00,0x0F,0x00,0x07,0x00,0x03,0xC0,0x00,
0x00,0x0F,0x00,0x0E,0x00,0x03,0xC0,0x00,0x00,0x0F,0x00,0x1E,0x00,0x03,0xC0,0x00,
0x00,0x0F,0x00,0x1C,0x00,0x03,0x80,0x00,0x00,0x0F,0x00,0x38,0x00,0x03,0x80,0x00,
0x00,0x0F,0x00,0x70,0x00,0x07,0x80,0x00,0x00,0x0F,0x00,0xE0,0x1F,0x0F,0x80,0x00,
0x00,0x0F,0x00,0xC0,0x07,0xFF,0x00,0x00,0x00,0x0F,0x01,0x80,0x01,0xFF,0x00,0x00,
0x00,0x0F,0x03,0x00,0x00,0x7E,0x00,0x00,0x00,0x0F,0x06,0x00,0x00,0x7C,0x00,0x00,
0x00,0x0F,0x1C,0x00,0x00,0x38,0x00,0x00,0x00,0x0E,0x30,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};/*"D-",0*/

int main(void)
{
	/* 关闭JLINK */
	rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	/* 中断优先级选组 */
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	/* 外设初始化 */
	Delay_Init();

	ST7735S_CPT144_Init();
	
	ST7735S_CPT144_SetBackColor(ST7735S_CPT144_BLACK);

	while(1)
	{
		ST7735S_CPT144_Clear();
		ST7735S_CPT144_SetTextColor(ST7735S_CPT144_BLUE);
		ST7735S_CPT144_DispPicture(IMAlogo);
		Delay_Ms(2000);
		ST7735S_CPT144_Clear();
		ST7735S_CPT144_SetTextColor(ST7735S_CPT144_YELLOW);
		ST7735S_CPT144_DispChinese(1, 1, dan, 64, 64);
		ST7735S_CPT144_DispChinese(64, 1, xie, 64, 64);
		Delay_Ms(2000);
		ST7735S_CPT144_Clear();
		ST7735S_CPT144_SetTextColor(ST7735S_CPT144_RED);
		ST7735S_CPT144_SetFont(&Font24x32);
		ST7735S_CPT144_DispString_XDir(1 ,1, "012345678");
		Delay_Ms(2000);
	}
}
