#include "pwm.h"



void TIM3_PWM_Init(u16 arr,u16 prer)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;        //时基
	TIM_OCInitTypeDef TIM_OCInitStructure;                   //Output Compare
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	///////GPIOA_6初始化端口复用///////////
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);   //GPIOA6复用为TIM3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;     ///复用
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/////////定时器TIM3时基初始化/////////
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = prer;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM3,ENABLE);
	
	//////初始化TIM3,Channel1 PWm模式///////      OC Output Campare
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;    //OC模式为PWM1模式      PWM1即为当TIMx_CNT在TIMx-CCRx之下则为有效
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;    //输出极性为低    periph小于CCR1时为低电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //OC使能
	TIM_OCInitStructure.TIM_Pulse = 0;          //占空比       周期为arr TIM_Period 
	
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);         //使能TIM3在CCR1上的预装载寄存器
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);                   //输出比较 CH1 初始化
	
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);    //ARR使能  (实时改变占空比)
	
}









