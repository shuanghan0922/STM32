#ifndef _KEY_H
#define _KEY_H

              
#define Key0  (GPIOE->IDR & 1<<4)!=0      //����û�а���ʱKey0=1 ����ʱKey0=0
#define Key1  (GPIOE->IDR & 1<<3)!=0
#define Key2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)    //�����⺯��



void KEY_Init(void);
void Key(void);


#endif 
