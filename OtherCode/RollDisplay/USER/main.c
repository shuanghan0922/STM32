#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "string.h"
#include "key.h"
#include "roll_display.h"

#define MES_SIZE	10

//__use_no_semihosting was requested, but _ttywrch was 
_ttywrch(int ch)        //Ϊ�˱���malloc�Ķ������  ����������
{  
}

uint8_t n,line=50;     //line �ڼ���;s

int main(void)
{
	uint8_t i,len;
	Linkedlist *head = creatList(MES_SIZE-1); 		//����һ����10���ڵ������
	Linkedlist *temp = closeList(head);				//�պϸ�����,������ĩβָ��
							
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);      //��ʼ����ʱ����
	uart_init(9600);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					  //��ʼ��LED
 	LCD_Init();           //��ʼ��LCD FSMC�ӿ�
	POINT_COLOR=RED;      //������ɫ����ɫ
	
	LCD_ShowString(50,10,210,24,16,"Rolling Data:");
	
	while(1)
	{				
		if (USART_RX_STA&0x8000)
		{	
			len = USART_RX_STA&0x3FFF;
			LCD_Fill(20,line,240,246,WHITE);       //�Զ�̬��ʾ�������ʾ
									
			memcpy(temp->Data,USART_RX_BUF,len);	//������д��ѭ�������ĩβ
			temp = temp->next;			//ָ����һ��ָ��,һ��ѭ�����Ϊͷָ��
			
			for (i=0;i<MES_SIZE;i++)			//����ȫ��������ڵ�
			{
				LCD_ShowString(20,line,240,12,16,temp->Data);    //ÿ�ζ���һ����ʾ
				temp = temp->next;				//�ص����ָ��
				line += 20;   	
			} 
			line=50;                             //����			
			USART_RX_STA=0;
			memset(USART_RX_BUF,0,len);          //������ջ���
		}			
	}
}


