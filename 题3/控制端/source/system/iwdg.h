#ifndef __IWDG_H
#define __IWDG_H
//#include "sys.h"
#include "board.h"
void IWDG_Init(uint8_t prer,uint16_t rlr);//IWDG³õÊ¼»¯
void IWDG_Feed(void);			//IWDGÎ¹¹·
#endif
