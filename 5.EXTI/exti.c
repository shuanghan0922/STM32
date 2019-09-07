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
	
	//////�ⲿ�жϳ�ʼ������///////////    
	//�ܹ���16��EXIT Line  �������к�׺Ϊ0��IO����EXIT0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource3);   //E3     E3��ַ��ӳ�䵽EXIT
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);   //E4
	///////�����ⲿ�ж�//////////
	EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;       //�жϴ���   ���е�Ϊ�¼�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    //trigger - ����
	
	EXTI_Init(&EXTI_InitStructure);
	
	//////�ж���������///////////
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;          //�ⲿ�ж�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;      //��ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;             //�����ȼ�Ϊ2
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;          //�ⲿ�ж�4
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;      //��ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;             //�����ȼ�Ϊ2
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI3_IRQHandler(void)
{
	delay_ms(10);
	if (!Key1)
		LED0 = !LED0;
	EXTI_ClearITPendingBit(EXTI_Line3);      //pending ����  (����ж��ж�����Ĵ���)
}

void EXTI4_IRQHandler(void)
{
	delay_ms(10);
	if (!Key2)
		LED1 = !LED1;
	EXTI_ClearITPendingBit(EXTI_Line4);
}



