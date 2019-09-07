#include "wwdg.h"
#include "led.h"


u8 WWDG_CNT = 0x7f;   //Ϊ��ʹ�õ���λ��Ч


void WWDG_Init(u8 tr,u8 wr,u32 prer)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	WWDG_CNT = tr & WWDG_CNT;      //��ʼ��װ��ֵ
	
	WWDG_SetPrescaler(prer);       //����Ԥ��Ƶֵ
	WWDG_SetWindowValue(wr);       //���ô���ֵ
	WWDG_Enable(WWDG_CNT);         //���ü���ֵ
	
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;     //WWDF�ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_Init(&NVIC_InitStructure);         //�жϳ�ʼ��
	
	
	WWDG_ClearFlag();      //�����ǰ�����жϱ�־λ
	WWDG_EnableIT();
}

void WWDG_IRQHandler(void)
{
	WWDG_SetCounter(WWDG_CNT);     //���ô��ڿ��Ź�ֵ   �� WWDG_Enable(WWDG_CNT);��� ȱ��
	WWDG_ClearFlag();
	LED1 = !LED1;
}




