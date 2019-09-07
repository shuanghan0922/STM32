#include "wwdg.h"
#include "led.h"


u8 WWDG_CNT = 0x7f;   //为了使得低七位有效


void WWDG_Init(u8 tr,u8 wr,u32 prer)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	WWDG_CNT = tr & WWDG_CNT;      //初始化装载值
	
	WWDG_SetPrescaler(prer);       //设置预分频值
	WWDG_SetWindowValue(wr);       //设置窗口值
	WWDG_Enable(WWDG_CNT);         //设置计数值
	
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;     //WWDF中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_Init(&NVIC_InitStructure);         //中断初始化
	
	
	WWDG_ClearFlag();      //清除提前唤醒中断标志位
	WWDG_EnableIT();
}

void WWDG_IRQHandler(void)
{
	WWDG_SetCounter(WWDG_CNT);     //重置窗口看门狗值   与 WWDG_Enable(WWDG_CNT);相比 缺少
	WWDG_ClearFlag();
	LED1 = !LED1;
}




