#include "led.h"
#include "stm32f4xx.h"


void LED_Init(void)
{
	RCC->AHB1ENR |= 1;           //对RCC成员变量 AHB1ENR (AHB1总线外部时钟存器)寄赋值 值为 0x00000020 为0使其使能
	
	GPIOA->MODER &= ~((3<<2*6)|(3<<2*7));      // 使得MODER寄存器PF9与PF10位清零 其它位保持不变
	GPIOA->OTYPER &= ~((1<<6)|(1<<7));    //输出类型寄存器 9和10清零
	GPIOA->OSPEEDR &= ~((3<<2*6)|(3<<2*7));
	GPIOA->PUPDR &= ~((3<<2*6)|(3<<2*7));
	
	GPIOA->MODER |= (1<<2*6)|(1<<2*7);       // PF9与PF10对应寄存器位变为 01 (通用输出模式)
	GPIOA->OTYPER |= 0<<6|0<<7;                   // 位置已经变为0 设置为推挽输出
	GPIOA->OSPEEDR |= (2<<2*6)|(2<<2*7);
	GPIOA->PUPDR |= (1<<2*6)|(1<<2*7);
	
	GPIOA->ODR |= 1<<6|1<<7;
}

