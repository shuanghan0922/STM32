#include "iic.h"
#include "delay.h"



void IIC_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; 		//B8��SCL  B9��SDA
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
	IIC_SDA=0;        //��ʼ�ź�
	delay_us(4);
	IIC_SCL=0;        //׼�������źŻ��߽�������
}

void IIC_Stop()
{
	SDA_OUT();
	IIC_SCL=0; IIC_SDA=0;
	delay_us(4);
	IIC_SCL=1;
	IIC_SDA=1;        //�����ź�
	delay_us(4);
}

u8 IIC_Wait_Ack()
{
	u8 ucErrTime=0;
	SDA_IN();				//����Ϊ����
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
	IIC_SCL=0;           //ʱ�����0
	return 0;
}

void IIC_Ack()			//����ACKӦ��
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

void IIC_NAck()			//������ACKӦ��
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

void IIC_Send_Byte(u8 dat)		//IIC����һ���ֽڵ�����
{
	u8 t;
	SDA_OUT();
	IIC_SCL=0;	//����ʱ�ӿ�ʼ���ݴ���
	for (t=0;t<8;t++)
	{
		IIC_SDA=(dat&0x80)>>7;  //�����ݵ����λ��ʼ����
		dat<<=1;
		delay_us(2);
		IIC_SCL=1;
		delay_us(2);
		IIC_SCL=0;
		delay_us(2);
	}
}

u8 IIC_Read_Byte(unsigned char ack)	//IIC��ȡһ���ֽڵ����ݲ�����Ӧ��; ack=1ʱ����ACK, ack=0ʱ����NACK
{
	unsigned char i, receive=0;
	SDA_IN();	//����Ϊ����
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





