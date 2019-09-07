#include "pwm.h"



void TIM3_PWM_Init(u16 arr,u16 prer)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;        //ʱ��
	TIM_OCInitTypeDef TIM_OCInitStructure;                   //Output Compare
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	///////GPIOA_6��ʼ���˿ڸ���///////////
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);   //GPIOA6����ΪTIM3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;     ///����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/////////��ʱ��TIM3ʱ����ʼ��/////////
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = prer;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM3,ENABLE);
	
	//////��ʼ��TIM3,Channel1 PWmģʽ///////      OC Output Campare
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;    //OCģʽΪPWM1ģʽ      PWM1��Ϊ��TIMx_CNT��TIMx-CCRx֮����Ϊ��Ч
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;    //�������Ϊ��    periphС��CCR1ʱΪ�͵�ƽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //OCʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;          //ռ�ձ�       ����Ϊarr TIM_Period 
	
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);         //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);                   //����Ƚ� CH1 ��ʼ��
	
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);    //ARRʹ��  (ʵʱ�ı�ռ�ձ�)
	
}









