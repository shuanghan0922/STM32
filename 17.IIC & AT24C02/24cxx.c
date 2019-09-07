#include "24cxx.h"
#include "delay.h"

void AT24CXX_Init()
{
	IIC_Init();
}

u8 AT24CXX_ReadOneByte(u16 ReadAddr)		//��AT24CXX��������һ���ֽ�����
{
	u8 temp;
	IIC_Start();
	if (EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0xA0);
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);	//���͸ߵ�ַ
	}
	else 
		IIC_Send_Byte(0xA0+((ReadAddr/256)<<1));	//����������ַ0xA0,д����
	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr%256);	//���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0xA1);			//�������ģʽ
	IIC_Wait_Ack();
	temp = IIC_Read_Byte(0);
	IIC_Stop();				//ͣʱ
	return temp;
}

void AT24CXX_WriteOneByte(u16 WriteAddr,u8 WriteData)			//��AT24CXX����дһ���ֽ�����
{
	IIC_Start();
	if (EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0xA0);
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}
	else 
		IIC_Send_Byte(0xA0+((WriteAddr/256)<<1));
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr%256);		//���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteData);
	IIC_Wait_Ack();
	IIC_Stop();
	delay_ms(10);
}

void AT24CXX_WriteLenByte(u16 WriteAddr,u32 WriteData,u8 Len)		//��AT24CXXд��ָ��Len���ȵ�����
{
	u8 t;
	for (t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(WriteData>>(8*t))&0xFF);
	}
}

u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)						//��AT24CXX��ȡָ�����ȵ�����
{
	u8 t;
	u32 temp=0;
	for (t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1);
	}
	return temp;
}

u8 AT24CXX_Check(void)												//���AT24CXX�Ƿ��쳣
{
	u8 temp;
	temp = AT24CXX_ReadOneByte(255);								//��ȡ��ֹ�ڵ�ַ255�е�����,�����������˵������
	if (temp==0x55)
		return 0;
	else
	{
		AT24CXX_WriteOneByte(255,0x55);
		temp = AT24CXX_ReadOneByte(255);
		if (temp==0x55)
			return 0;
	}
	return 1;
}

void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 ReadNum)				//��AT24CXX�����д�ָ���ĵ�ַ��ʼ����ָ������������
{
	while (ReadNum)
	{
		*pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
		ReadNum--;
	}
}

void ATCXX_Write(u16 WriteAddr,u8 *pBuffer,u16 WriteNum)			//��AT24CXX�����ָ����ַ��ʼд��ָ������������
{
	while(WriteNum--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}



