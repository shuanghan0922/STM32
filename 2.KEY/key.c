#include "stm32f4xx.h"
#include "key.h"
#include "led.h"


void KEY_Init()
{
	RCC->AHB1ENR |= 1|1<<4;     //ʹ��GPIOAʱ�Ӻ�GPIOEʱ��
	
	GPIOE->MODER &= ~(3<<2*3|3<<2*4);      
	GPIOE->OSPEEDR &= ~(3<<2*3|3<<2*4);
	GPIOE->PUPDR &= ~(3<<2*3|3<<2*4);
	
	GPIOE->MODER |= 0<<2*3|0<<2*4;    //���ģʽ   �����������OTYPER
	GPIOE->OSPEEDR |= 2<<2*3|2<<2*4;  //����
	GPIOE->PUPDR |= 1<<2*3|1<<2*4;    //����
	
	
	GPIOA->MODER &= ~3;      
	GPIOA->OSPEEDR &= ~3;
	GPIOA->PUPDR &= ~3;
	
	GPIOA->MODER |= 0;    //���ģʽ   ���벻������OTYPER
	GPIOA->OSPEEDR |= 2;  //����
	GPIOA->PUPDR |= 2;    //����
}

void Key()
{
	
}




