#ifndef _KEY_H
#define _KEY_H

              
#define Key0  (GPIOE->IDR & 1<<4) == 0      //位相同则为真  否则为假   //操作寄存器
#define Key1  (GPIOE->IDR & 1<<3) == 0
#define Key2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)    //操作库函数



void KEY_Init(void);
void Key(void);


#endif 
