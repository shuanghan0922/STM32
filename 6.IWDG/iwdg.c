#include "iwdg.h"



void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG->KR = 0x5555;      //寄存器使能
	IWDG->PR = prer;        //预分频值
	IWDG->RLR = rlr;        //重装载值
	
	IWDG->KR = 0xAAAA;      //初始化为使得初始值为设定值  需要先重装载
	IWDG->KR = 0xCCCC;      //开始工作
}

