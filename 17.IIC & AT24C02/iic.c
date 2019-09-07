#include "iic.h"
#include "delay.h"



void IIC_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; 		//B8接SCL  B9接SDA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	IIC_SCL=1;
	IIC_SDA=1;
}

void IIC_Start()
{
	SDA_OUT();
	IIC_SDA=1; IIC_SCL=1; 
	delay_us(4);
	IIC_SDA=0;        //开始信号
	delay_us(4);
	IIC_SCL=0;        //准备发送信号或者接收数据
}

void IIC_Stop()
{
	SDA_OUT();
	IIC_SCL=0; IIC_SDA=0;
	delay_us(4);
	IIC_SCL=1;
	IIC_SDA=1;        //结束信号
	delay_us(4);
}

u8 IIC_Wait_Ack()
{
	u8 ucErrTime=0;
	SDA_IN();				//设置为输入
	IIC_SDA=1; delay_us(1);
	IIC_SCL=1; delay_us(1);
	while (Read_SDA)
	{
		ucErrTime++;
		if (ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;           //时钟输出0
	return 0;
}

void IIC_Ack()			//产生ACK应答
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

void IIC_NAck()			//不产生ACK应答
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

void IIC_Send_Byte(u8 dat)		//IIC发送一个字节的数据
{
	u8 t;
	SDA_OUT();
	IIC_SCL=0;	//拉低时钟开始数据传输
	for (t=0;t<8;t++)
	{
		IIC_SDA=(dat&0x80)>>7;  //从数据的最高位开始传输
		dat<<=1;
		delay_us(2);
		IIC_SCL=1;
		delay_us(2);
		IIC_SCL=0;
		delay_us(2);
	}
}

u8 IIC_Read_Byte(unsigned char ack)	//IIC读取一个字节的数据并发送应答; ack=1时发送ACK, ack=0时发送NACK
{
	unsigned char i, receive=0;
	SDA_IN();	//设置为输入
	for (i=0;i<8;i++)
	{
		IIC_SCL=0;
		delay_us(2);
		IIC_SCL=1;
		receive<<=1;
		if (Read_SDA)
			receive++;
		delay_us(1);
	}
	if (!ack)
		IIC_NAck();
	else 
		IIC_Ack();
	return receive;
}





