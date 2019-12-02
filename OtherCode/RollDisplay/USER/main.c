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
_ttywrch(int ch)        //为了避免malloc的定义错误  半主机错误
{  
}

uint8_t n,line=50;     //line 第几行;s

int main(void)
{
	uint8_t i,len;
	Linkedlist *head = creatList(MES_SIZE-1); 		//创建一个有10个节点的链表
	Linkedlist *temp = closeList(head);				//闭合该链表,并返回末尾指针
							
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);      //初始化延时函数
	uart_init(9600);		//初始化串口波特率为115200
	LED_Init();					  //初始化LED
 	LCD_Init();           //初始化LCD FSMC接口
	POINT_COLOR=RED;      //画笔颜色：红色
	
	LCD_ShowString(50,10,210,24,16,"Rolling Data:");
	
	while(1)
	{				
		if (USART_RX_STA&0x8000)
		{	
			len = USART_RX_STA&0x3FFF;
			LCD_Fill(20,line,240,246,WHITE);       //对动态显示行清除显示
									
			memcpy(temp->Data,USART_RX_BUF,len);	//将数据写入循环链表的末尾
			temp = temp->next;			//指向下一个指针,一个循环后变为头指针
			
			for (i=0;i<MES_SIZE;i++)			//遍历全部的链表节点
			{
				LCD_ShowString(20,line,240,12,16,temp->Data);    //每次对下一行显示
				temp = temp->next;				//回到起点指针
				line += 20;   	
			} 
			line=50;                             //底行			
			USART_RX_STA=0;
			memset(USART_RX_BUF,0,len);          //清除接收缓存
		}			
	}
}


