#ifndef _USART_H
#define _USART_H
#include "sys.h"
////PA2 USART_TX
////PA3 USART_RX

#define USART2_REC_LEN 200	   //�����������ֽ��� 200
	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		//����״̬���	

void uart2_init(u32 bound);

#endif

