#ifndef _IIC_H
#define _IIC_H
#include "sys.h"

//设置SDA的输入输出模式(IO方向)
#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2)); GPIOB->MODER|=0<<9*2;}  //输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2)); GPIOB->MODER|=1<<9*2;}	 //输出模式

#define IIC_SCL PBout(8)
#define IIC_SDA PBout(9)
#define Read_SDA PBin(9)

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(unsigned char dat);
u8 IIC_Read_Byte(unsigned char ack);




#endif

