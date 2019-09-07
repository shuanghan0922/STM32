#include "exti.h"
#include "key.h"
#include "led.h"
#include "delay.h"



void EXTIX_Init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	KEY_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	//////外部中断初始化配置///////////    
	//总共有16条EXIT Line  比如所有后缀为0的IO属于EXIT0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource3);   //E3     E3地址重映射到EXIT
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);   //E4
	///////配置外部中断//////////
	EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;       //中断触发   还有的为事件触发
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    //trigger - 触发
	
	EXTI_Init(&EXTI_InitStructure);
	
	//////中断向量配置///////////
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;          //外部中断3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;      //抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;             //子优先级为2
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;          //外部中断4
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;      //抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;             //子优先级为2
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI3_IRQHandler(void)
{
	delay_ms(10);
	if (!Key1)
		LED0 = !LED0;
	EXTI_ClearITPendingBit(EXTI_Line3);      //pending 挂起  (清除中断判定挂起寄存器)
}

void EXTI4_IRQHandler(void)
{
	delay_ms(10);
	if (!Key2)
		LED1 = !LED1;
	EXTI_ClearITPendingBit(EXTI_Line4);
}



