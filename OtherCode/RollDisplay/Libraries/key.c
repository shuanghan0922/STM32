#include "stm32f4xx.h"
#include "key.h"
#include "led.h"


void KEY_Init()
{
	RCC->AHB1ENR |= 1|1<<4;     //使能GPIOA时钟和GPIOE时钟
	
	GPIOE->MODER &= ~(3<<2*3|3<<2*4);      
	GPIOE->OSPEEDR &= ~(3<<2*3|3<<2*4);
	GPIOE->PUPDR &= ~(3<<2*3|3<<2*4);
	
	GPIOE->MODER |= 0<<2*3|0<<2*4;    //输出模式   输出不用设置OTYPER
	GPIOE->OSPEEDR |= 2<<2*3|2<<2*4;  //中速
	GPIOE->PUPDR |= 1<<2*3|1<<2*4;    //上拉
	
	
	GPIOA->MODER &= ~3;      
	GPIOA->OSPEEDR &= ~3;
	GPIOA->PUPDR &= ~3;
	
	GPIOA->MODER |= 0;    //输出模式   输入不用设置OTYPER
	GPIOA->OSPEEDR |= 2;  //中速
	GPIOA->PUPDR |= 2;    //上拉
}

void Key()
{
	
}




