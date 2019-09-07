#include "timer.h"
#include "led.h"



void TIM3_Init(u16 arr,u16 psc)
{	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //ʱ��
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   //������ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = arr;                       //�Զ���װ�س�ֵ (period-����)
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                    //ʱ��Ԥ��Ƶ
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);                //��ʼ��TIM3
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);             //����TIM3�����ж�
	TIM_Cmd(TIM3,ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}


void TIM3_IRQHandler()
{
	if (TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)      //�жϻ�ȡ���Ƿ�Ϊ�����ж�
	{
		LED1 = !LED1;
		TIM_ClearFlag(TIM3,TIM_IT_Update);
	}
}





