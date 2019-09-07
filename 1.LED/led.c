#include "led.h"
#include "stm32f4xx.h"


void LED_Init(void)
{
	RCC->AHB1ENR |= 1;           //��RCC��Ա���� AHB1ENR (AHB1�����ⲿʱ�Ӵ���)�ĸ�ֵ ֵΪ 0x00000020 Ϊ0ʹ��ʹ��
	
	GPIOA->MODER &= ~((3<<2*6)|(3<<2*7));      // ʹ��MODER�Ĵ���PF9��PF10λ���� ����λ���ֲ���
	GPIOA->OTYPER &= ~((1<<6)|(1<<7));    //������ͼĴ��� 9��10����
	GPIOA->OSPEEDR &= ~((3<<2*6)|(3<<2*7));
	GPIOA->PUPDR &= ~((3<<2*6)|(3<<2*7));
	
	GPIOA->MODER |= (1<<2*6)|(1<<2*7);       // PF9��PF10��Ӧ�Ĵ���λ��Ϊ 01 (ͨ�����ģʽ)
	GPIOA->OTYPER |= 0<<6|0<<7;                   // λ���Ѿ���Ϊ0 ����Ϊ�������
	GPIOA->OSPEEDR |= (2<<2*6)|(2<<2*7);
	GPIOA->PUPDR |= (1<<2*6)|(1<<2*7);
	
	GPIOA->ODR |= 1<<6|1<<7;
}

